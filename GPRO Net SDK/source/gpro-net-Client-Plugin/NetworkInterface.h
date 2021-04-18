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
	PlayerMove UpdateOutputRemote(); // Send messages
	PlayerMove UpdateInputRemote(); //Receive
	//GameState & Turns
	const PlayerMove& GetGameState() const { return gameState; }
	bool PlayerMoveSelected(int x, int y, int playerIndex);
	int GetPlayerAssignment() const { return amBottomRow; }
	int GetState() const { return unityState; }



	
private:
	void AddMessageToQueue(MNCL::GameMessage msg);
	RakNet::RakPeerInterface* peer;
	RakNet::SystemAddress serverAddress;
	std::queue<RakNet::BitStream*> sendQueue;
	PlayerMove gameState;
	int amBottomRow = -1;
	int unityState;
};


