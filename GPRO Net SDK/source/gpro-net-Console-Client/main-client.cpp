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
						printf("Cnt: %i", gs->playBoard[0][7]);

					}
					else
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, topRow);
						printf("[%x,%x]: %i", (int)y, (int)(x), gs->playBoard[y][x]);
					}
				}
				else // Bottom Row
				{
					if (x == 7) // Score
					{
						gpro_consoleSetColor(textColor, bottomRow);
						printf("Scr: %i", gs->playBoard[1][7]);
					}
					else if (x == 0) // Side total
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, sideTotal);
						printf("Cnt: %i", gs->playBoard[1][0]);
					}
					else
					{
						gpro_consoleSetCursorColor(x * 10, y, textColor, bottomRow);
						printf("[%x,%x]: %i", (int)y, (int)(x), gs->playBoard[y][x]);
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

		while (true)
		{
			printf("\nRoom number to join: ");
			char tempRoom[512];
			std::cin.getline(tempRoom, 512);
			if (int room = atoi(tempRoom) < gs->numberGameRooms)
			{
				gs->roomSelection = room;
				break;
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
		gpro_consoleClear();
		if (gs->isPlayerTurn)
		{
			while (true)
			{
				DisplayGame(gs);
				//Make sure you can only select on your side
				printf("\n Selection: ");
				char move[512];
				std::cin.getline(move, 512);
				gs->selection = atoi(move);
				if (gs->selection > 0 && gs->selection < 7)
				{
					break;
				}
			}
		}
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

void InitBoard(GameState* gs)
{
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 1; j < 7; ++j)
		{
			gs->playBoard[i][j] = 4;
		}
	}
}

bool PlayerTurn(GameState* gs)
{
	//DONE- If your last marble is in your goal - turn repeat
	//DONE- If you end on YOUR side and its empty AND the other side has marbles YOU win them
	//DONE- ALWAYS counter clockwise rotation

	int numRocks = gs->playBoard[static_cast<int>(gs->amPlayerRed)][gs->selection];
	gs->playBoard[static_cast<int>(gs->amPlayerRed)][gs->selection] = 0;

	int x = gs->selection + (gs->amPlayerRed ? 1:-1);
	int y = static_cast<int>(gs->amPlayerRed);
	gs->selection = -1;

	bool inAScore = false;
	while (numRocks > 0)
	{
		if (y == 0) //Top row
		{
			while (x >= (gs->amPlayerRed ? 1 : 0))
			{
				gs->playBoard[y][x]++;
				numRocks--;
				if (numRocks <= 0)
				{
					break;
				}
				x--;
				inAScore = false;
				if (y == static_cast<int>(gs->amPlayerRed) && x == (gs->amPlayerRed ? 7 : 0))
				{
					inAScore = true;
				}
			}
			x = 1;
			y = 1;
		}
		else if (y == 1)
		{
			while (x <= (gs->amPlayerRed ? 7 : 6))
			{
				gs->playBoard[y][x]++;
				numRocks--;
				if (numRocks <= 0)
				{
					break;
				}
				x++;
				inAScore = false;
				if (y == static_cast<int>(gs->amPlayerRed) && x == (gs->amPlayerRed ? 7 : 0))
				{
					inAScore = true;
				}
			}
			x = 6;
			y = 0;
		}
	} //End while rocks

	//Did the rock land in a empty spot with rocks on the other side
	//if( not in score or count & in a spot with only 1 rock & on your side
	y = y == 0 ? 1 : 0;
	if (x != 0 && x != 7 && gs->playBoard[(y)][x] == 1 && y == static_cast<int>(gs->amPlayerRed))
	{
		int gainedTotal = 0;
		gs->playBoard[y][x] = 0;
		gainedTotal += gs->playBoard[(y == 1 ? 0 : 1)][x] + 1;
		gs->playBoard[(y == 1 ? 0 : 1)][x] = 0;

		//add to the appropriate score 
		gs->playBoard[gs->amPlayerRed ? 1 : 0][gs->amPlayerRed ? 7 : 0] += gainedTotal;
	}

	//Did my rock end in own goal
	return inAScore;
	//if (y == static_cast<int>(gs->amPlayerRed) && x == (gs->amPlayerRed ? 7 : 0))
	//{
	//	return true;
	//}
	//return false;
}

void Update(GameState* gs)
{
	if (gs->isPlayerTurn)
	{
		//If there is a selection (safety check) 
		if (gs->selection != -1)
		{
			gs->isPlayerTurn = PlayerTurn(gs);
			//THIS IS JUST TO SWAP DELTE LATER
			if (!gs->isPlayerTurn)
			{
				gs->isPlayerTurn = true;
				gs->amPlayerRed = !gs->amPlayerRed;
			}
		}
	}

	//Update side total
	gs->playBoard[0][7] = 0;
	gs->playBoard[1][0] = 0;
	for (int i = 1; i < 7; ++i)
	{
		gs->playBoard[0][7] += gs->playBoard[0][i];
		gs->playBoard[1][0] += gs->playBoard[1][i];
	}
}

void OutputRemote(GameState* gs)
{

}

void OutputLocal(GameState* gs)
{
	DisplayGame(gs);
}


int main(/*int const argc, char const* const argv[]*/)
{
	bool gameRunning = true;

	const unsigned short SERVER_PORT = 7777;
	const char SERVER_IP[] = "172.16.2.63";

	GameState* gameState = new GameState();
	gameState->peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	gameState->peer->Startup(1, &sd, 1);
	gameState->peer->SetMaximumIncomingConnections(0);
	gameState->peer->Connect(SERVER_IP, SERVER_PORT, 0, 0);
	InitBoard(gameState);
	printf("Starting the CLIENT.\n");

	gpro_consoleResetColor();
	while (gameRunning)
	{
		assert(gameState);
		InputLocal(gameState); //Chose square
		InputRemote(gameState);
		Update(gameState); //Update board
		OutputRemote(gameState); //Send board
		OutputLocal(gameState);
	}

	RakNet::RakPeerInterface::DestroyInstance(gameState->peer);
	printf("\n\n");
	system("pause");
	return 0;
}