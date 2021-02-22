#pragma once
#include "RakNet/RakPeerInterface.h"
#include "PackageStructs.h"
#include "RakNet/RakString.h"
#include <queue>


class GameState
{
public:
	RakNet::RakPeerInterface* peer;
	RakNet::Packet* packet;
	RakNet::SystemAddress serverAdress;

	bool serverEstablished = true;

	char username[512];
	int numberGameRooms = 3;
	int roomSelection;

	bool isPlayerTurn = true;
	bool amPlayerRed = true;;
	int playBoard[2][8];
	//Slot [0][0] player 1 (self) score
	//Slot [1][0] player 2 score
	int selection = -1;


	bool inLobby = true;


	char msgOut[512];
	std::queue<RakNet::RakString> msgInQueue;
};



/*
	Mancala:
		[2]        ___________________________
				  /				7			  \
			 _______________________________________
		 0	| 0  | 1  | 2  | 3  | 4  | 5  | 6  |    |	[8]
			|    |____|____|____|____|____|____|    |
		 1	|    | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
			|____|____|____|____|____|____|____|____|

				  \___________________________/
								7
*/