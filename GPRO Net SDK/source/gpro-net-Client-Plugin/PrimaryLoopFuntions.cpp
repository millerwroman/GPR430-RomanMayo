#include "PrimaryLoopFuntions.h"

NetworkInterface::NetworkInterface()
{
	peer = RakNet::RakPeerInterface::GetInstance();
}

NetworkInterface::~NetworkInterface()
{
	peer->Shutdown(0);
	peer = nullptr;
}

bool NetworkInterface::ConnectToServer(char* ip, int port) const
{
	if (!peer)
	{
		return false;
	}

	RakNet::SocketDescriptor socket;
	peer->Startup(1, &socket, 1);
	peer->SetMaximumIncomingConnections(0);
	peer->Connect(ip, port, 0, 0);
	return true;
}

bool NetworkInterface::UpdateOutputRemote()
{
	return true;
}

bool NetworkInterface::UpdateInputRemote()
{
	return true;
}

bool NetworkInterface::PlayerMoveSelected(int x, int y)
{
	gameState.x = x;
	gameState.y = y;
	return true;
}
