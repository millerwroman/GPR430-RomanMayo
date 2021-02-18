/*
   Copyright 2021 Daniel S. Buckstein

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	GPRO Net SDK: Networking framework.
	By Daniel S. Buckstein

	main-client.c/.cpp
	Main source for console client application.
*/

//Provided
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> //Take this out when we can 

//RakNet
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h" 
#include "RakNet/GetTime.h" 
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"

//Shared Lib
#include "gpro-net/gpro-net.h"
#include "gpro-net/GameMessages.h"
#include "gpro-net/gpro-net-common/gpro-net-console.h"


//Things that are common to all message types
// 1) Timestamp ID (constant)
// 2) Timestamp
// 3) Identifier actual
// 4) Ability to read and write

//Things that are differnt
// 1) actual message data
//		-- Any type of raw bytes
//		-- no raw or emcapsualted pointers

class cMessage
{
	RakNet::MessageID msgID;
protected:
	cMessage(RakNet::MessageID id_new) : msgID(id_new) {}
public:
	RakNet::MessageID GetID() const { return msgID; }

	virtual RakNet::BitStream& Read(RakNet::BitStream& bs)
	{
		return bs;
	}
	virtual RakNet::BitStream& Write(RakNet::BitStream& bs) const
	{
		return bs;
	}

};
RakNet::BitStream& operator >> (RakNet::BitStream& bs, cMessage& msg)
{
	return msg.Read(bs);
}
RakNet::BitStream& operator << (RakNet::BitStream& bs, cMessage const& msg)
{
	return msg.Write(bs);
}

class cMessageHeader
{
	RakNet::Time time;
//Sequence
	int count;
	RakNet::MessageID* id_list;
};

class cTimeMessage : public cMessage
{
	RakNet::Time time;
public:
	cTimeMessage() :cMessage(ID_TIMESTAMP), time(RakNet::GetTime()) {}

	virtual bool Read(RakNet::BitStream* bs) override
	{
		//Read some bs id
		//bs->Read(time);
		return bs;
	}
	virtual bool Write(RakNet::BitStream* bs) const override
	{
		//bs->Write(GetID());
		//bs->Write(time);
		return bs;
	}
};

class cChatmessage : public cMessage
{
	char* cstr;
	size_t len;
public:
	cChatmessage(char* cstr_new) : cMessage(ID_CHAT_MESSAGE), cstr(cstr_new), len(strlen(cstr_new)) {}


	virtual bool Read(RakNet::BitStream* bs) override

	{
		bs->Read(len);
		bs->Read(cstr, (unsigned)len);
		return bs;
	}
	virtual bool Write(RakNet::BitStream* bs) const override
	{
		bs->Write(len);
		bs->Write(cstr, (unsigned)len);
		return bs;
	}
};

int main(int const argc, char const* const argv[])
{
	gpro_consoleDrawTestPatch();

	printf("\n\n");
	system("pause");
}