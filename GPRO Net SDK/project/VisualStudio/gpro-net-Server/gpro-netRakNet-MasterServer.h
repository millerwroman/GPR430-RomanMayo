#pragma once
#include "gpro-net/gpro-net-server/gpro-net-RakNet-Server.hpp"

const static int MAX_SERVERS = 10;

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
	virtual RakNet::SystemAddress GetServerAddress() override { return GameServers[0]; }
	RakNet::SystemAddress GetServerAddress(int serverIndex) { return (serverIndex < MAX_SERVERS ? GameServers[serverIndex] : nullptr); }
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
	RakNet::SystemAddress GameServers[MAX_SERVERS]; //0 its the personal adress;
};