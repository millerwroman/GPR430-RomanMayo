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
		unsigned short MAX_CLIENTS = 2;

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

		// server-specific messages
		switch (msgID)
		{
		case ID_NEW_INCOMING_CONNECTION:
		{
			printf("A connection is incoming.\n");
			Players.push_back(sender);
			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID)FPV::ID_REQUEST_PLAYER_ID);
			int index = static_cast<int>(Players.size()) -1;
			bs.Write(index);
			printf("New Player Id: %i \n", index);
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, false);
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
		case FPV::ID_PLAYER_STATE:
		{
			FPV::PlayerStateMessage msg = FPV::PlayerStateMessage(bitstream);

			RakNet::BitStream bs;
			bs.Write((RakNet::MessageID)ID_TIMESTAMP);
			bs.Write(RakNet::GetTime() - dtSendToReceive);
			msg.WriteToBitStream(bs);
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true);
			return true;
		}
		case FPV::ID_PROJ_STATE:
		{
			FPV::ProjStateMessage msg = FPV::ProjStateMessage(bitstream);

			RakNet::BitStream bs;
			msg.WriteToBitStream(bs);
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true);
			return true;
		}
		case FPV::ID_CHAT_MSG:
		{
			FPV::ChatMessage msg = FPV::ChatMessage(bitstream);

			RakNet::BitStream bs;
			msg.WriteToBitStream(bs);
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sender, true);
			return true;
		}
		default:
		{
			printf("A default Packing ID: %d", msgID);
		}
		}//End Switch
		return false;
	}
}