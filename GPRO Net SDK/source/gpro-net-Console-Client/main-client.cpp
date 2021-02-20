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
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"

//Shared Lib
#include "gpro-net/gpro-net.h"
#include "gpro-net/GameState.h"
#include "gpro-net/GameMessages.h"
#include "gpro-net/gpro-net-common/gpro-net-console.h"

//Things that are common to all message types
// 1) Timestamp ID (constant)
// 2) Timestamp
// 3) Identifier actual
// 4) Ability to read and write

//Things that are differnt
// 1) actual message data
//		-- Any type of raw bytes
//		-- no raw or emcapsualted pointers

class cMessage
{
	RakNet::MessageID msgID;
protected:
	cMessage(RakNet::MessageID id_new) : msgID(id_new) {}
public:
	RakNet::MessageID GetID() const { return msgID; }

	virtual bool Read(RakNet::BitStream* bs)
	{
		return bs;
	}
	virtual bool Write(RakNet::BitStream* bs) const
	{
		return bs;
	}

};

class cMessageHeader
{
	RakNet::Time time;
	//Sequence
	int count;
	RakNet::MessageID* id_list;
};

class cTimeMessage : public cMessage
{
	RakNet::Time time;
public:
	cTimeMessage() :cMessage(ID_TIMESTAMP), time(RakNet::GetTime()) {}

	virtual bool Read(RakNet::BitStream* bs) override
	{
		//Read some bs id
		//bs->Read(time);
		return bs;
	}
	virtual bool Write(RakNet::BitStream* bs) const override
	{
		//bs->Write(GetID());
		//bs->Write(time);
		return bs;
	}
};

class cChatmessage : public cMessage
{
	char* cstr;
	size_t len;
public:
	cChatmessage(char* cstr_new) : cMessage(ID_CHAT_MESSAGE), cstr(cstr_new), len(strlen(cstr_new)) {}


	virtual bool Read(RakNet::BitStream* bs) override

	{
		bs->Read(len);
		bs->Read(cstr, (unsigned)len);
		return bs;
	}
	virtual bool Write(RakNet::BitStream* bs) const override
	{
		bs->Write(len);
		bs->Write(cstr, (unsigned)len);
		return bs;
	}
};


void DisplayGame(GameState* gs)
{
	//gpro_consoleClear();
	gpro_consoleColor topRow = gpro_consoleColor_blue;
	gpro_consoleColor bottomRow = gpro_consoleColor_red;
	gpro_consoleColor textColor = gpro_consoleColor_black;

	gpro_consoleColor sideTotal = gpro_consoleColor_green;

	HANDLE const stdHandle = GetStdHandle(STD_OUTPUT_HANDLE), console = GetConsoleWindow();
	if (stdHandle && console)
	{
		short x, y;
		for (y = 0; y < 2; ++y)
		{
			for (x = 0; x < 8; ++x)
			{
				gpro_consoleSetCursor(x * 10, y);
				if (y == 0) //Top row
				{
					if (x == 0) // Score
					{
						gpro_consoleSetColor(textColor, topRow);
						printf("Scr: %i", gs->playBoard[0][0]);
					}
					else if (x == 7) //Side Total
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, sideTotal);
						printf("Cnt: %i", gs->topMarbleTotal);

					}
					else
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, topRow);
						printf("[%x,%x]: %i", (int)y, (int)(x), 0);
					}
				}
				else
				{
					if (x == 7) // Score
					{
						gpro_consoleSetColor(textColor, bottomRow);
						printf("Scr: %i", gs->playBoard[0][1]);
					}
					else if (x == 0) // Side total
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, sideTotal);
						printf("Cnt: %i", gs->bottomMarbleTotal);
					}
					else
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, bottomRow);
						printf("[%x,%x]: %i", (int)y, (int)(7 - x), 0);
					}

				}
			}
		}
		gpro_consoleResetColor();
	}
	system("pause");
}


void InputLocal(GameState* gs)
{
	if (!gs->serverEstablished) return;

	if (gs->inLobby)
	{
		printf("Username: ");
		std::cin.getline(gs->username, 512);

		printf("\n\nThere are currently %i rooms \n", gs->numberGameRooms);

		bool validRoom = false;
		while (!validRoom)
		{
			printf("\nRoom number to join: ");
			char tempRoom[512];
			std::cin.getline(tempRoom, 512);
			if (int room = atoi(tempRoom) < gs->numberGameRooms)
			{
				validRoom = true;
				gs->roomSelection = room;
			}
			else
			{
				std::fill_n(tempRoom, 512, '\0');
			}
		}
		gpro_consoleClear();
		gs->inLobby = false;
	}
	else
	{
		DisplayGame(gs);
	}
}

void InputRemote(GameState* gs)
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
			//create package
			peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		}
		break;
		case ID_INITAL_CONTACT:
		{
			gs->serverAdress = packet->systemAddress;
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
			break;

		case ID_CHAT_MESSAGE:
		{
			char message[512];
			bsIn.Read(message);

			gs->msgInQueue.push(RakNet::RakString(message));
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

void OutputRemote(GameState* gs)
{

}

void OutputLocal(GameState* gs)
{

}


int main(/*int const argc, char const* const argv[]*/)
{
	bool gameRunning = true;

	const unsigned short SERVER_PORT = 7777;
	const char SERVER_IP[] = "172.16.2.186";

	GameState* gameState = new GameState();
	gameState->peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	gameState->peer->Startup(1, &sd, 1);
	gameState->peer->SetMaximumIncomingConnections(0);
	gameState->peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	printf("Starting the CLIENT.\n");

	gpro_consoleResetColor();
	while (gameRunning)
	{
		assert(gameState);
		InputLocal(gameState);
		InputRemote(gameState);
		Update(gameState);
		OutputRemote(gameState);
		OutputLocal(gameState);
	}

	RakNet::RakPeerInterface::DestroyInstance(gameState->peer);
	printf("\n\n");
	system("pause");
	return 0;
}