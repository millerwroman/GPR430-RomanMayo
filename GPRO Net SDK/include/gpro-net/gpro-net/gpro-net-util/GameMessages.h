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
		ID_GAME_MESSAGE_END
	};

	class GameMessage //abstract
	{
		GameMessageID ID;
	};
}