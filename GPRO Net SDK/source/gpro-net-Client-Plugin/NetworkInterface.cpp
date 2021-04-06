#include "NetworkInterface.h"


NetworkInterface::NetworkInterface()
{
	peer = RakNet::RakPeerInterface::GetInstance();
}

NetworkInterface::~NetworkInterface()
{
	peer->Shutdown(0);
	peer = nullptr;
}

bool NetworkInterface::ConnectToServer(const char* ip, int port) const
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

GameState NetworkInterface::UpdateOutputRemote()
{
	/*RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)MNCL::GameMessageID::ID_DEFAULT_GAME_MESSAGE);
	bs.Write(stuff);
	peer->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverAddress, false)*/;

	while (!sendQueue.empty())
	{
		RakNet::BitStream* bs = sendQueue.front();
		peer->Send(&(*bs), HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverAddress, false);
		sendQueue.pop();
	}
	return gameState;
}

GameState NetworkInterface::UpdateInputRemote()
{
	int count = 0;
	RakNet::Packet* packet = 0;
	RakNet::MessageID msgID = 0;
	RakNet::Time dtSendToReceive = 0;

	while (packet = peer->Receive())
	{
		RakNet::BitStream bitstream(packet->data, packet->length, false);
		bitstream.Read(msgID);

		switch (msgID)
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			break;
		case ID_REMOTE_CONNECTION_LOST:
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			break;
		case ID_CONNECTION_LOST:
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			serverAddress = packet->systemAddress;
			//packet->length This is how you would get the size of packet (remeber you already have first byte)
			//if no timestand packetLenghts-1
			UpdateOutputRemote();
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			break;
		case MNCL::GameMessageID::ID_PLAYER_SELECTION:
		{
			MNCL::PlayerSelectionMessage msg(bitstream);
			turn.x = msg.x;
			turn.y = msg.y;
		}
		break;
		default:
			break;
		}

		// done with packet
		peer->DeallocatePacket(packet);
	}

	// done
	//return count;
	return gameState;
}

bool NetworkInterface::PlayerMoveSelected(int x, int y)
{
	//gameState.x = x;
	//gameState.y = y;
	RakNet::BitStream* bs = new RakNet::BitStream();
	MNCL::PlayerSelectionMessage msg(x, y);
	msg.WriteToBitStream(*bs);
	sendQueue.push(bs);
	return true;
}

void NetworkInterface::AddMessageToQueue(MNCL::GameMessage msg)
{
	RakNet::BitStream* bs = new RakNet::BitStream();
	msg.WriteToBitStream(*bs);
	sendQueue.push(bs);
}
