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
	bool ConnectToServer(char* ip, int port) const;
	RakNet::RakPeerInterface* GetRakInterface() const { return peer; }
	//Update
	bool UpdateOutputRemote(); // Send messages
	bool UpdateInputRemote(); //Receive
	//GameState & Turns
	const GameState& GetGameState() const { return gameState; }
	bool PlayerMoveSelected(int x, int y);
private:
	RakNet::RakPeerInterface* peer;
	std::queue<MNCL::GameMessage> sendQueue;
	GameState gameState;
};


