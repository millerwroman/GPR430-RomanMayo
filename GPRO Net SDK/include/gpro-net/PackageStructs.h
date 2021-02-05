#pragma once

#pragma pack(push,1)
struct GameMessage1
{
	//TODO: Uncomment once time is set up
	//char timeID; //needs to be ID_TIMESTAMP
	//RakNet::Time time; //assuing using RakNet::GetTime();
	//ID
	char msgID;
	//Message
	char msg[512];
	//Time
};
#pragma pack (pop)

#pragma pack(push,1)
struct InitalConnectMessage
{
	//ID
	char msgID;
	//Message
	char username[512];
	//Ip Adress
	char ip[512];
};
#pragma pack (pop)