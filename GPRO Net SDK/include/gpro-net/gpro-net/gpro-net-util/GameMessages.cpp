#include "GameMessages.h"
#include "RakNet/RakPeerInterface.h"


void FPV::GameMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
}

FPV::GameMessage::GameMessage()
{
	ID = GameMessageID::ID_GAME_MESSAGE_START;
}

FPV::PlayerStateMessage::PlayerStateMessage(PlayerMove& playerMove)
{
	move = playerMove;
	ID = ID_PLAYER_STATE;
}

FPV::PlayerStateMessage::PlayerStateMessage(RakNet::BitStream& bs)
{
	ID = FPV::ID_PLAYER_STATE;
	bs.Read(move.playerIndex);
	bs.Read(move.LocX);
	bs.Read(move.LocY);
	bs.Read(move.LocZ);
	bs.Read(move.RotX);
	bs.Read(move.RotY);
	bs.Read(move.RotZ);
	bs.Read(move.RotW);
}

void FPV::PlayerStateMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
	bs.Write(move.playerIndex);
	bs.Write(move.LocX);
	bs.Write(move.LocY);
	bs.Write(move.LocZ);
	bs.Write(move.RotX);
	bs.Write(move.RotY);
	bs.Write(move.RotZ);
	bs.Write(move.RotW);
}

FPV::ProjStateMessage::ProjStateMessage(ProjectileMove& playerMove)
{
	ID = ID_PROJ_STATE;
	move = playerMove;
}

FPV::ProjStateMessage::ProjStateMessage(RakNet::BitStream& bs)
{
	ID = ID_PROJ_STATE;
	bs.Read(move.ProjIndex);
	bs.Read(move.LocX);
	bs.Read(move.LocY);
	bs.Read(move.LocZ);
}

void FPV::ProjStateMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
	bs.Write(move.ProjIndex);
	bs.Write(move.LocX);
	bs.Write(move.LocY);
	bs.Write(move.LocZ);
}

FPV::ProjDeletedMessage::ProjDeletedMessage(int index)
{
	ID = ID_PROJ_DELETED;
	i = index;
}

FPV::ProjDeletedMessage::ProjDeletedMessage(RakNet::BitStream& bs)
{
	ID = ID_PROJ_DELETED;
	bs.Read(i);
}

void FPV::ProjDeletedMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
	bs.Write(i);
}
