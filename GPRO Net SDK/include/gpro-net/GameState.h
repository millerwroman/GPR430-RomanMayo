#pragma once
#include "RakNet/RakPeerInterface.h"

class GameState
{
public:
	RakNet::RakPeerInterface* peer;
	RakNet::Packet* packet;

	bool madeInitalContact = false;

	char msg[512];
};