#pragma once
#include "gpro-net/gpro-net/gpro-net-RakNet.hpp"
#include "gpro-net-gamestate.h"
#include <string>
#include <queue>

namespace FPV
{
	enum GameMessageID
	{
		ID_GAME_MESSAGE_START = gproNet::eMessageCommon::ID_GPRO_MESSAGE_COMMON_END,
		ID_DEFAULT_GAME_MESSAGE,
		ID_PLAYER_STATE,
		ID_PROJ_STATE,
		ID_REQUEST_PLAYER_ID,
		ID_PROJ_DELETED,
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

	struct TimeStampMessage : public GameMessage
	{
		TimeStampMessage() = default;
		TimeStampMessage(RakNet::Time t);
		TimeStampMessage(RakNet::BitStream& bs);
		virtual void WriteToBitStream(RakNet::BitStream& bs) override;

		RakNet::Time time;
		RakNet::MessageID MsgID;
	};

	struct PlayerStateMessage : public TimeStampMessage
	{
		PlayerStateMessage() = default;
		PlayerStateMessage(PlayerMove& playerMove, RakNet::Time t);
		PlayerStateMessage(RakNet::BitStream& bs);
		virtual void WriteToBitStream(RakNet::BitStream& bs) override;

		PlayerMove move;
	};

	struct ProjStateMessage : public GameMessage
	{
		ProjStateMessage() = default;
		ProjStateMessage(ProjectileMove& playerMove);
		ProjStateMessage(RakNet::BitStream& bs);
		virtual void WriteToBitStream(RakNet::BitStream& bs) override;

		ProjectileMove move;
	};

	struct ProjDeletedMessage : public GameMessage
	{
		ProjDeletedMessage() = default;
		ProjDeletedMessage(int index);
		ProjDeletedMessage(RakNet::BitStream& bs);
		virtual void WriteToBitStream(RakNet::BitStream& bs) override;

		int i;
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