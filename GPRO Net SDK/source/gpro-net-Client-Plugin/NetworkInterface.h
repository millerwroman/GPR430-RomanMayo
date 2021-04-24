#pragma once
#include "gpro-net/gpro-net/gpro-net-util/GameMessages.h"
#include "gpro-net/gpro-net/gpro-net-util/gpro-net-gamestate.h"

#include <queue>

class NetworkInterface
{
public:
	//Construction
	NetworkInterface();
	~NetworkInterface();
	//Connections
	bool ConnectToServer(const char* ip, int port) const;
	RakNet::RakPeerInterface* GetRakInterface() const { return peer; }
	//Update
	bool UpdateOutputRemote(); // Send messages
	bool UpdateInputRemote(); //Receive

	bool PackagePlayerState(PlayerMove* move);

	//Debug
	const char* PrintDebugUnity();



	
private:
	void AddMessageToQueue(FPV::GameMessage& msg);
	RakNet::RakPeerInterface* peer;
	RakNet::SystemAddress serverAddress;
	std::queue<RakNet::BitStream*> sendQueue;
	std::string debugMessage;
};


