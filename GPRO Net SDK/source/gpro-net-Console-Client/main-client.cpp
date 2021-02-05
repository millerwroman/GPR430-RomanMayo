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
#include "gpro-net//PackageStructs.h"

//Declare Baisc Pattern
void HandleLocalInput(GameState* gs);
void HandleRemoteInput(GameState* gs);
void Update(GameState* gs);
void HandleOutputRemote(const GameState* gs);
void HandleOutputLocal(GameState* gs);

//Others
void ReadStringToConsole(RakNet::Packet* packet);



int main(void)
{
	//char str[512];
	const unsigned short SERVER_PORT = 7777;
	const char SERVER_IP[] = "172.16.2.69";

	GameState* gs = new GameState();
	gs->peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	gs->peer->Startup(1, &sd, 1);
	gs->peer->SetMaximumIncomingConnections(0);
	gs->peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	printf("Starting the CLIENT.\n");


	while (true)
	{

		HandleLocalInput(gs);
		HandleRemoteInput(gs);
		//Update(gs);
		HandleOutputRemote(gs);
		//HandleOutputLocal(gs);
	}

	RakNet::RakPeerInterface::DestroyInstance(gs->peer);

	return 0;
}

void HandleLocalInput(GameState* gs)
{
	if (gs->madeInitalContact)
	{
		printf("Outgoing Message:\n");
		std::cin >> gs->msg;
	}

}

void HandleRemoteInput(GameState* gs)
{
	RakNet::RakPeerInterface* peer = gs->peer;
	RakNet::Packet* packet = gs->packet;

	while (packet = peer->Receive())
	{
		switch (packet->data[0])
		{
			//case ID_TIMESTAMP:
			//{

			//}
			break;
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
			bs.Write("Default Username");
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
		break;
		case ID_INITAL_CONTACT:
		{
			ReadStringToConsole(packet);
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
			ReadStringToConsole(packet);
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

void HandleOutputRemote(const GameState* gs)
{
	if (gs->msg != "")
	{

	}
}

void HandleOutputLocal(GameState* gs)
{
}

void ReadStringToConsole(RakNet::Packet* packet)
{
	RakNet::RakString rs;
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(rs);
	printf("%s\n", rs.C_String());
}

void SendChatMessage(char string[512], GameMessages id,  RakNet::RakPeerInterface* peer)
{
	RakNet::RakString rakString = RakNet::RakString(string);
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)id);



}


