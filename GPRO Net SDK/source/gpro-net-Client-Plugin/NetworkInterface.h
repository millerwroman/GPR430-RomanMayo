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
	//Start Up
	bool ConnectToServer(const char* ip, int port) const;
	int GetLocalPlayerIndex() { return localPlayerIndex; }
	RakNet::RakPeerInterface* GetRakInterface() const { return peer; }
	//Update
	bool UpdateOutputRemote(); // Send messages
	bool UpdateInputRemote(); //Receive

	bool PackagePlayerState(PlayerMove* move);
	bool PackageProjStates(ProjectileMove* moves);

	int GetNetworkedMoves(PlayerMove* moves, int lastCount);
	int GetNetworkedProjMoves(ProjectileMove* moves, int lastCount);

	//Debug
	const char* PrintDebugUnity();



private:
	void AddMessageToQueue(FPV::GameMessage& msg);
	RakNet::RakPeerInterface* peer;
	RakNet::SystemAddress serverAddress;
	std::queue<RakNet::BitStream*> sendQueue;
	std::string debugMessage = "DEFUALT";
	int localPlayerIndex = -1;

	std::vector<PlayerMove*> networkedMoves;
	std::vector<ProjectileMove*> networkedProjMoves;
	void AddState(PlayerMove& move);
	void AddState(ProjectileMove& move);
	static PlayerMove* DynamicMoveCopy(PlayerMove& move);
	static ProjectileMove* DynamicMoveCopy(ProjectileMove& move);

	RakNet::BitStream& ReadTimestamp(RakNet::BitStream& bitstream, RakNet::Time& dtSendToReceive_out, RakNet::MessageID& msgID_out);
};

