#pragma once
#include "gpro-net/gpro-net/gpro-net-RakNet.hpp"
#include <string>
#include <queue>

namespace MNCL //(Mancala)
{
	enum GameMessageID
	{
		ID_GAME_MESSAGE_START = gproNet::eMessageCommon::ID_GPRO_MESSAGE_COMMON_END,
		ID_DEFAULT_GAME_MESSAGE,
		ID_PLAYER_SELECTION,
		ID_GAME_MESSAGE_END
	};

#pragma pack(push, 1)
	struct GameMessage //abstract
	{
		GameMessage();
		GameMessage(RakNet::MessageID _id) : ID(_id) {}
		RakNet::MessageID ID;
	};

	struct TimeStampMessage : public GameMessage
	{
		//needs constructor
		RakNet::MessageID Time_ID = ID_TIMESTAMP;
		RakNet::Time time;
		RakNet::MessageID ID;
	};

	struct PlayerSelectionMessage : public GameMessage
	{
		PlayerSelectionMessage();
		PlayerSelectionMessage(int _x, int _y) : GameMessage((RakNet::MessageID)ID_PLAYER_SELECTION), x(_x), y(_y) {};

		int x, y;
	};
#pragma pack(pop)
}