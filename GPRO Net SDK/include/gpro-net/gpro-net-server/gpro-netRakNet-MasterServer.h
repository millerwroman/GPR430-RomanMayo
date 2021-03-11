#pragma once
#include "gpro-net/gpro-net-server/gpro-net-RakNet-Server.hpp"


class cRakNetServerMaster : public gproNet::cRakNetServer
{
	// public methods
public:
	// cRakNetServer
	//	Default constructor.
	cRakNetServerMaster();

	// ~cRakNetServer
	//	Destructor.
	virtual ~cRakNetServerMaster() override;
public:
	RakNet::SystemAddress GetServerAddress() { return mAddress; }

	// protected methods
protected:
	// ProcessMessage
	//	Unpack and process packet message.
	//		param bitstream: packet data in bitstream
	//		param dtSendToReceive: locally-adjusted time difference from sender to receiver
	//		param msgID: message identifier
	//		return: was message processed
	virtual bool ProcessMessage(RakNet::BitStream& bitstream, RakNet::SystemAddress const sender, RakNet::Time const dtSendToReceive, RakNet::MessageID const msgID);

private:
	RakNet::SystemAddress mMasterServer;
	RakNet::SystemAddress mAddress; //This would get set on initialization from RakNet

};