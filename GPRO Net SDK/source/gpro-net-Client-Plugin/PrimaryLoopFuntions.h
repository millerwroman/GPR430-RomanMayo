#pragma once
#pragma once
#include "gpro-net/gpro-net/gpro-net-util/GameMessages.h"
#include <queue>

class NetworkInterface
{
public:
	NetworkInterface();
	~NetworkInterface();
	bool ConnectToServer(char* ip, int port);
public:
	RakNet::RakPeerInterface* GetRakInterface() { return peer; }
	bool SendMessages();
	bool ReciveMessages();
private:
	RakNet::RakPeerInterface* peer;
	std::queue<MNCL::GameMessage> sendQueue;
};


