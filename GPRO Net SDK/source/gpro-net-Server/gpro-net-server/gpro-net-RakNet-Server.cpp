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

	gpro-net-RakNet-Server.cpp
	Source for RakNet server management.
*/

#include "gpro-net/gpro-net-server/gpro-net-RakNet-Server.hpp"
#include "gpro-net/gpro-net/gpro-net-util/GameMessages.h"
#include <iostream>


namespace gproNet
{
	cRakNetServer::cRakNetServer()
	{
		RakNet::SocketDescriptor sd(SET_GPRO_SERVER_PORT, 0);
		unsigned short MAX_CLIENTS = 10;

		peer->Startup(MAX_CLIENTS, &sd, 1);
		peer->SetMaximumIncomingConnections(MAX_CLIENTS);
		printf("Server Is Running\n\n\n");
	}

	cRakNetServer::~cRakNetServer()
	{
		peer->Shutdown(0);
	}

	bool cRakNetServer::ProcessMessage(RakNet::BitStream& bitstream, RakNet::SystemAddress const sender, RakNet::Time const dtSendToReceive, RakNet::MessageID const msgID)
	{
		if (cRakNetManager::ProcessMessage(bitstream, sender, dtSendToReceive, msgID))
			return true;

		// server-specific messages
		switch (msgID)
		{
		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID)MNCL::GameMessageID::ID_ASSIGN_PLAYER);
			int playerIndex = -1;
			if (!playerZeroTurn)
			{
				Players[0] = sender;
				playerZeroTurn = true;
				bs.Write(0);
				peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, false);
			}
			else
			{
				Players[1] = sender;
				bs.Write(1);
				peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, false);
				//Begin Game Funtion
			}
			return true;
		}
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			//printf("The server is full.\n");
			return true;
		case ID_DISCONNECTION_NOTIFICATION:
			//printf("A client has disconnected.\n");
			return true;
		case ID_CONNECTION_LOST:
			//printf("A client lost the connection.\n");
			return true;

			// test message
		case ID_GPRO_MESSAGE_COMMON_BEGIN:
		{
			// server receives greeting, print it and send one back
			RakNet::BitStream bitstream_w;
			ReadTest(bitstream);
			WriteTest(bitstream_w, "Hello client from server");
			peer->Send(&bitstream_w, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, sender, false);
		}	return true;
		case MNCL::ID_PLAYER_SELECTION:
		{
			MNCL::PlayerSelectionMessage data = MNCL::PlayerSelectionMessage(bitstream);
			printf("%i %i \n", data.x, data.y);
			if (Players[static_cast<int>(!playerZeroTurn)] == sender) //If it is the same systemadress as the player whos turn it is
			{
				//send to other client
				RakNet::BitStream bsOut;
				data.WriteToBitStream(bsOut);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::SystemAddress(), true);
			}
			else
			{
				//Send back not your turn
			}
			return true;
		}
		}
		return false;
	}
}