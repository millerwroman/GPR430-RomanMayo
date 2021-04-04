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
		virtual void WriteToBitStream(RakNet::BitStream& bs);
		GameMessage();
		GameMessage(RakNet::MessageID _id) : ID(_id) {}
		RakNet::MessageID ID;
	};

	//struct TimeStampMessage : public GameMessage
	//{
	//	//needs constructor
	//	RakNet::MessageID Time_ID = ID_TIMESTAMP;
	//	RakNet::Time time;
	//	RakNet::MessageID ID;
	//};

	struct PlayerSelectionMessage : public GameMessage
	{
		PlayerSelectionMessage() = default;
		PlayerSelectionMessage(int _x, int _y) : GameMessage((RakNet::MessageID)ID_PLAYER_SELECTION), x(_x), y(_y) {};
		PlayerSelectionMessage(RakNet::BitStream& bs);
		virtual void WriteToBitStream(RakNet::BitStream& bs) override;

		int x, y;
	};

	//Risk With sending stucts:
	/*
	* Risk Sending Pointer
	* If you send X-Plat it might understand data differntly
	* BS takes care of byte ordedring and packing
	* Have a Read/Write Pair for the struct and send via bs
	* Structs are good for storing but not sending
	*/
#pragma pack(pop)
}