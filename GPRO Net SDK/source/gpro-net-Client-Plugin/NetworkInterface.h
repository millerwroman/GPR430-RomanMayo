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
	GameState UpdateOutputRemote(); // Send messages
	GameState UpdateInputRemote(); //Receive
	//GameState & Turns
	const GameState& GetGameState() const { return gameState; }
	bool PlayerMoveSelected(int x, int y, int playerIndex);



	
private:
	void AddMessageToQueue(MNCL::GameMessage msg);
	RakNet::RakPeerInterface* peer;
	RakNet::SystemAddress serverAddress;
	std::queue<RakNet::BitStream*> sendQueue;
	GameState gameState;
	
};


