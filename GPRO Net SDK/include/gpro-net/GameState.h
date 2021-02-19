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


	bool inLobby = true;


	char msgOut[512];
	std::queue<RakNet::RakString> msgInQueue;
};