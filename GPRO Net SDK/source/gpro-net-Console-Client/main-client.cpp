/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	main-client.c/.cpp
	Main source for console client application.
*/

//Provided
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> //Take this out when we can 

//RakNet
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h" 
#include "RakNet/GetTime.h" 

//Shared Lib
#include "gpro-net/gpro-net.h"
#include "gpro-net/GameMessages.h"
#include "gpro-net/GameState.h"
//#include "gpro-net/PackageStructs.h"

//Declare Baisc Pattern
void HandleLocalInput(GameState* gs);
void HandleRemoteInput(GameState* gs);
void Update(GameState* gs);
void HandleOutputRemote(GameState* gs);
void HandleOutputLocal(GameState* gs);

//Others
void AddRakStringToQueue(GameState* gs, RakNet::Packet* packet);
unsigned char GetPacketIdentifier(RakNet::Packet* p);
ChatMessage CreateChatMessage(RakNet::RakString msg);
void ReadAndEmptyQueue(GameState* gs);

//Packet Handelers
void UnpackChatMessageToQueue(GameState* gs, RakNet::Packet* packet);
void SendChatMessage(GameState* gs);



int main(void)
{
	//char str[512];
	const unsigned short SERVER_PORT = 7777;
	const char SERVER_IP[] = "172.16.2.218";

	GameState* gs = new GameState();
	gs->peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	gs->peer->Startup(1, &sd, 1);
	gs->peer->SetMaximumIncomingConnections(0);
	gs->peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	printf("Starting the CLIENT.\n");


	while (true)
	{

		HandleLocalInput(gs); //Write message
		HandleRemoteInput(gs); //check new Messages
		//Update(gs);
		HandleOutputRemote(gs); //Send message
		HandleOutputLocal(gs); //Show new messages
	}

	RakNet::RakPeerInterface::DestroyInstance(gs->peer);

	return 0;
}

void HandleLocalInput(GameState* gs)
{
	if (gs->madeInitalContact)
	{
		printf("Request Connected Users: 'u'\n Outgoing Message:\n");
		char msg[512];
		std::cin.getline(msg, 512);
		printf("\n");
		if (msg[0] == 'u' || msg[0] == 'U')
		{
			gs->requestUsernames = true;
		}
		else
		{
			gs->msgOut = CreateChatMessage(msg);
		}
	}

}

void HandleRemoteInput(GameState* gs)
{
	RakNet::RakPeerInterface* peer = gs->peer;
	RakNet::Packet* packet = gs->packet;

	while (packet = peer->Receive())
	{
		RakNet::MessageID msgID = packet->data[0];
		RakNet::BitStream bsIn(packet->data, packet->length, false);
		bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		RakNet::Time sendTime;

		if (packet->data[0] == ID_TIMESTAMP)
		{
			bsIn.Read(sendTime);
			bsIn.Read(msgID);
		}
		switch (msgID)
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("Another client has connected.\n");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			printf("Connection Request Accepted\n");

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID)ID_INITAL_CONTACT);
			bs.Write("Miller");
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
		break;
		case ID_INITAL_CONTACT:
		{
			AddRakStringToQueue(gs, packet);
			gs->serverAdress = packet->systemAddress;
			gs->madeInitalContact = true;
		}
		break;
		case ID_NEW_INCOMING_CONNECTION:
			printf("A connection is incoming.\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("The server is full.\n");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			printf("We have been disconnected.\n");
			break;
		case ID_CONNECTION_LOST:
			printf("Connection lost.\n");
			//break;

		case ID_GAME_MESSAGE_1:
		{
			AddRakStringToQueue(gs, packet);
		}
		break;
		case ID_CHAT_MESSAGE:
		{
			UnpackChatMessageToQueue(gs, packet);
		}
		break;
		case ID_REQUEST_CONNECTED_USERS:
		{
			AddRakStringToQueue(gs, packet);
		}
		break;
		default:
			printf("(Default) Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}//End of Switch
		peer->DeallocatePacket(packet);
	}//End of While
}

void Update(GameState* gs)
{
}

void HandleOutputRemote(GameState* gs)
{
	if (gs->requestUsernames)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_REQUEST_CONNECTED_USERS);
		gs->peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->serverAdress, false);
	}
	else if (!gs->msgOut.msg.IsEmpty())
	{
		SendChatMessage(gs);
		gs->msgOut = ChatMessage();
	}

}

void HandleOutputLocal(GameState* gs)
{
	if (gs->requestUsernames)
	{
		gs->requestUsernames = false;

	}
	if (gs->msgInQueue.size() > 0)
	{
		ReadAndEmptyQueue(gs);
	}

}

void AddRakStringToQueue(GameState* gs, RakNet::Packet* packet)
{
	RakNet::RakString rs;
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(rs);
	gs->msgInQueue.push(rs);
}

void ReadAndEmptyQueue(GameState* gs)
{
	while (gs->msgInQueue.size() > 0)
	{
		RakNet::RakString msg = gs->msgInQueue.front();
		msg.Printf();
		printf("\n");
		gs->msgInQueue.pop();
	}
}

ChatMessage CreateChatMessage(RakNet::RakString msg)
{

	ChatMessage msgOut
	{
		ID_TIMESTAMP,
		RakNet::GetTime(),
		ID_CHAT_MESSAGE,
		msg
	};

	return msgOut;
}

void SendChatMessage(GameState* gs)
{
	if (!gs) return;
	if (!gs->peer) return;

	gs->peer->Send((char*)&(gs->msgOut), sizeof(gs->msgOut), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gs->serverAdress, false);
}

unsigned char GetPacketIdentifier(RakNet::Packet* p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	else
		return (unsigned char)p->data[0];
}

void UnpackChatMessageToQueue(GameState* gs, RakNet::Packet* packet)
{
	ChatMessage* msg = (ChatMessage*)packet->data;
	assert(packet->length == sizeof(ChatMessage)); // Idk why we need this but it says to -  ask dan
	if (packet->length != sizeof(ChatMessage))
	{
		return;
	}

	gs->msgInQueue.push(msg->msg);
}
