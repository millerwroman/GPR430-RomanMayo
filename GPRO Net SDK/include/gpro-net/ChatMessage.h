#pragma once

#pragma pack (push)
#pragma pack (1)
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