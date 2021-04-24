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

bool NetworkInterface::UpdateOutputRemote()
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
	return true;
}

bool NetworkInterface::UpdateInputRemote()
{
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
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			break;
		default:
			break;
		}

		// done with packet
		peer->DeallocatePacket(packet);
	}

	// done
	return true;
}

bool NetworkInterface::PackagePlayerState(PlayerMove* move)
{
	FPV::PlayerStateMessage msg = FPV::PlayerStateMessage(*move);
	AddMessageToQueue(msg);
	return true;
}

const char* NetworkInterface::PrintDebugUnity()
{
	return debugMessage.c_str();
}

void NetworkInterface::AddMessageToQueue(FPV::GameMessage& msg)
{
	RakNet::BitStream* bs = new RakNet::BitStream();
	msg.WriteToBitStream(*bs);
	sendQueue.push(bs);
}
