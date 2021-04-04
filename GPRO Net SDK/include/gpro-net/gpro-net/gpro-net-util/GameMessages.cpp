#include "GameMessages.h"
#include "RakNet/RakPeerInterface.h"

MNCL::PlayerSelectionMessage::PlayerSelectionMessage(RakNet::BitStream& bs)
:GameMessage((RakNet::MessageID)ID_PLAYER_SELECTION)
{
	bs.Read(x);
	bs.Read(y);
}

void MNCL::PlayerSelectionMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
	bs.Write(x);
	bs.Write(y);
}

void MNCL::GameMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(ID);
}

MNCL::GameMessage::GameMessage()
{
	ID = GameMessageID::ID_GAME_MESSAGE_START;
}
