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

	bool madeInitalContact = false;

	bool requestUsernames = false;
	char msgOut[512];
	std::queue<RakNet::RakString> msgInQueue;
};