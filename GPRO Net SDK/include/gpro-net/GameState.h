#pragma once
#include "RakNet/RakPeerInterface.h"
#include "PackageStructs.h"
#include <queue>

class GameState
{
public:
	RakNet::RakPeerInterface* peer;
	RakNet::Packet* packet;
	RakNet::SystemAddress serverAdress;

	bool madeInitalContact = false;

	ChatMessage msgOut; 
	std::queue<ChatMessage*> msgInQueue;
};