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

MNCL::GameStateMessage::GameStateMessage(GameState& gameState)
{
	gs = gameState;
}

MNCL::GameStateMessage::GameStateMessage(RakNet::BitStream& bs)
{
	bs.Read(gs.username);
	bs.Read(gs.isPlayerTurn);
	bs.Read(gs.amBottomRow);
	bs.Read(gs.playBoard);
	bs.Read(gs.x);
	bs.Read(gs.y);
}

void MNCL::GameStateMessage::WriteToBitStream(RakNet::BitStream& bs)
{
	bs.Write(gs.username);
	bs.Write(gs.isPlayerTurn);
	bs.Write(gs.amBottomRow);
	bs.Write(gs.playBoard);
	bs.Write(gs.x);
	bs.Write(gs.y);

}
