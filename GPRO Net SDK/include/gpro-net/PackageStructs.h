#pragma once

#pragma pack(push,1)
struct ChatMessage
{
	//ID
	char msgID;
	//Message
	char msg[512];
	//Time ID
	char timeID; //needs to be ID_TIMESTAMP
	RakNet::Time time; //assuing using RakNet::GetTime();
};
#pragma pack (pop)
//
//#pragma pack(push,1)
//struct InitalConnectMessage
//{
//	//ID
//	char msgID;
//	//Message
//	char username[512];
//	//Ip Adress
//	RakNet::SystemAddress sysAdress;
//};
//#pragma pack (pop)