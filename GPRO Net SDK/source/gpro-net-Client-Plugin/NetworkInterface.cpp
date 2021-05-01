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
		case FPV::ID_REQUEST_PLAYER_ID:
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
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			break;
		case FPV::ID_PLAYER_STATE:
		{
			FPV::PlayerStateMessage msg(bitstream);

			AddState(msg.move);
		}
		break;
		case FPV::ID_PROJ_STATE:
		{
			FPV::ProjStateMessage msg(bitstream);
			AddState(msg.move);
		}
		break;
		case FPV::ID_PROJ_DELETED:
		{
			FPV::ProjDeletedMessage msg(bitstream);
			deletedProjs.push_back(msg.i);
		}
		break;
		default:
			//debugMessage = "Default Case: " + std::to_string(msgID);
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

void NetworkInterface::AddState(ProjectileMove& move)
{
	networkedProjMoves.push_back(DynamicMoveCopy(move));
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

ProjectileMove* NetworkInterface::DynamicMoveCopy(ProjectileMove& move)
{
	ProjectileMove* m = new ProjectileMove();
	m->ProjIndex = move.ProjIndex;
	m->LocX = move.LocX;
	m->LocY = move.LocY;
	m->LocZ = move.LocZ;

	return m;
}

bool NetworkInterface::PackagePlayerState(PlayerMove* move)
{
	FPV::PlayerStateMessage msg = FPV::PlayerStateMessage(*move);
	AddMessageToQueue(msg);
	return true;
}

bool NetworkInterface::PackageProjStates(ProjectileMove* moves)
{

	FPV::ProjStateMessage msg = FPV::ProjStateMessage(*moves);
	AddMessageToQueue(msg);
	return true;
}

int NetworkInterface::GetNetworkedMoves(PlayerMove* moves, int lastCount)
{
	if (lastCount < networkedMoves.size())
	{
		PlayerMove* m = networkedMoves[lastCount];
		moves->playerIndex = m->playerIndex;
		moves->LocX = m->LocX;
		moves->LocY = m->LocY;
		moves->LocZ = m->LocZ;
		moves->RotX = m->RotX;
		moves->RotY = m->RotY;
		moves->RotZ = m->RotZ;
		moves->RotW = m->RotW;
		return 1; //Valid
	}
	networkedMoves.clear();
	return 0;

}

int NetworkInterface::GetNetworkedProjMoves(ProjectileMove* moves, int lastCount)
{
	if (lastCount < networkedProjMoves.size())
	{
		ProjectileMove* m = networkedProjMoves[lastCount];
		moves->ProjIndex = m->ProjIndex;
		moves->LocX = m->LocX;
		moves->LocY = m->LocY;
		moves->LocZ = m->LocZ;
		return 1; //Valid
	}
	networkedMoves.clear();
	return 0;

}

bool NetworkInterface::ProjDeleted(int index)
{
	FPV::ProjDeletedMessage msg(index);
	AddMessageToQueue(msg);
	return true;
}

int NetworkInterface::GetDeletedProjs(int count)
{
	if (count < deletedProjs.size())
	{
		return deletedProjs[count];
	}
	deletedProjs.clear();
	return -1;
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
