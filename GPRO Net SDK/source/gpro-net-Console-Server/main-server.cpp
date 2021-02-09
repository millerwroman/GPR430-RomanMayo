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

	main-server.c/.cpp
	Main source for console server application.
*/

//Provided
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> //Take this out when we can 
#include <map>

//RakNet
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h" 
#include "RakNet/GetTime.h"

//Shared Lib
#include "gpro-net/gpro-net.h"
#include "gpro-net/GameMessages.h"
#include "gpro-net/GameState.h"
#include "gpro-net/PackageStructs.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 7777

//Things to do:
// 2) Fix consistancy on loop
// 4) write messages to text file

void SendConnectedUsers(RakNet::RakPeerInterface* peer, RakNet::SystemAddress address);


std::map <RakNet::RakString, RakNet::SystemAddress> g_connectedClients;

int main(void)
{
	//GetConnectionIndex or somthing
	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);
	printf("Starting the SERVER.\n");
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
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
				//This gaves more data than just id
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_INITAL_CONTACT:
			{
				RakNet::RakString username;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(username);
				g_connectedClients[username] = packet->systemAddress;

				//Send Message back
				RakNet::BitStream bs;
				bs.Write((RakNet::MessageID)ID_INITAL_CONTACT);
				bs.Write("Welcome our server! \n\n");
				peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;

			case ID_GAME_MESSAGE_1:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s\n", rs.C_String());
			}
			break;
			case ID_CHAT_MESSAGE:
			{
				char message[512];
				bsIn.Read(message);
				//WRITE THAT SHIT TO FILE

				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_TIMESTAMP);
				bsOut.Write(sendTime);
				bsOut.Write((RakNet::MessageID)ID_CHAT_MESSAGE);
				bsOut.Write(message);

				
				printf(message);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
			}
			break;
			case ID_REQUEST_CONNECTED_USERS:
			{
				SendConnectedUsers(peer, packet->systemAddress);
			}
			break;

			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	return 0;
}

void SendConnectedUsers(RakNet::RakPeerInterface* peer, RakNet::SystemAddress address)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_REQUEST_CONNECTED_USERS);
	std::map<RakNet::RakString, RakNet::SystemAddress>::iterator it;

	RakNet::RakString userNames = "\n Connected Users: \n";
	for (it = g_connectedClients.begin(); it != g_connectedClients.end(); it++)
	{
		userNames += it->first;
		userNames += "\n";
	}
	bsOut.Write(userNames);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
}

