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
		case ID_NEW_INCOMING_CONNECTION:
			bitstream.Read(localPlayerIndex);
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
		case FPV::ID_PLAYER_STATE:
		{
			FPV::PlayerStateMessage msg(bitstream);
			AddState(msg.move);
		}
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

void NetworkInterface::AddState(PlayerMove& move)
{
	networkedMoves.push_back(DynamicMoveCopy(move));
}

PlayerMove* NetworkInterface::DynamicMoveCopy(PlayerMove& move)
{
	PlayerMove* m = new PlayerMove();
	m->playerIndex = move.playerIndex;
	m->LocX = move.LocX;
	m->LocY = move.LocY;
	m->LocZ = move.LocZ;
	m->RotX = move.RotX;
	m->RotY = move.RotY;
	m->RotZ = move.RotZ;
	m->RotW = move.RotW;

	return m;
}

bool NetworkInterface::PackagePlayerState(PlayerMove* move)
{
	FPV::PlayerStateMessage msg = FPV::PlayerStateMessage(*move);
	AddMessageToQueue(msg);
	return true;
}

int NetworkInterface::GetNetworkedMoves(PlayerMove* moves, int lastCount)
{
	if (networkedMoves.size() < lastCount) return 0; //No more to get
	moves = networkedMoves[lastCount];
	return 1; //Valid
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
