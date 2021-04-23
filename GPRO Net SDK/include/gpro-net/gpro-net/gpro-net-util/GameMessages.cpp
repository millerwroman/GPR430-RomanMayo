#include "GameMessages.h"
#include "RakNet/RakPeerInterface.h"


void MNCL::GameMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
}

MNCL::GameMessage::GameMessage()
{
	ID = GameMessageID::ID_GAME_MESSAGE_START;
}

MNCL::PlayerStateMessage::PlayerStateMessage(PlayerMove& playerMove)
{
	move = playerMove;
}

MNCL::PlayerStateMessage::PlayerStateMessage(RakNet::BitStream& bs)
{
	bs.Read(move.LocX);
	bs.Read(move.LocY);
	bs.Read(move.LocZ);
	bs.Read(move.RotX);
	bs.Read(move.RotY);
	bs.Read(move.RotZ);
	bs.Read(move.RotW);
	bs.Read(move.Shoot);
}

void MNCL::PlayerStateMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
	bs.Write(move.LocX);
	bs.Write(move.LocY);
	bs.Write(move.LocZ);
	bs.Write(move.RotX);
	bs.Write(move.RotY);
	bs.Write(move.RotZ);
	bs.Write(move.RotW);
	bs.Write(move.Shoot);
}
