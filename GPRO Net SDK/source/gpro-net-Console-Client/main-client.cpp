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

#include "gpro-net/gpro-net.h"
//Provided
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> //Take this out when we can 

//RakNet
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

int main(int const argc, char const* const argv[])
{
	char str[512];
	bool isServer;

	RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::Packet* packet;


	printf("(C)lient or (S)erver? \n");
	std::cin >> str;

	if (str[0] == 'C' || str[0] == 'c')
	{
		RakNet::SocketDescriptor sd; 
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else
	{
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_CLIENTS, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server. \n");
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
	}
	else
	{
		printf("Enter Server IP or hit enter for 127.0.0.1 \n");
		std::cin >> str;
		if (str[0] == 0) // If using default IP
		{
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client. \n");
		peer->Connect(str, SERVER_PORT, 0, 0);
	}

	while (1)
	{
		//Get a packet, while we are reciving packets, let server know we are done with packet, get next packet
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive());
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("A client as disconnected :( \n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("A client has lost connection ;( \n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("A client has connected :) \n");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming! \n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full \n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) 
				{
					printf("A client has disconnected.\n");
				}
				else 
				{
					printf("We have been disconnected.\n");
				}
				printf("A client as disconnected \n");
				break;
			default:
				printf("Message with identifier %i has arrive \n", packet->data[0]);
				break;
			}
			
		}
	}



	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}
