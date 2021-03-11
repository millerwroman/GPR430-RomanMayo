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

	main-server.cpp
	Main source for console server application.
*/

#include "gpro-net/gpro-net-server/gpro-net-RakNet-Server.hpp"
#include "gpro-net/gpro-net-server/gpro-netRakNet-MasterServer.h"

const static int MAX_SERVERS = 10;

int main(int const argc, char const* const argv[])
{
	//Array of servers (0 is master)
	gproNet::cRakNetServer* GameServers[MAX_SERVERS];
	//Init all servers
	GameServers[0] = new cRakNetServerMaster(); //Init Master Server
	RakNet::SystemAddress masterAddress = GameServers[0]->GetServerAddress();
	for(int i=1; i<MAX_SERVERS; ++i)
	{
		GameServers[i] = new gproNet::cRakNetServer();
		GameServers[i]->SetMasterServer(masterAddress);
	}
	bool runServers = true; //Flip this when shutdown is ready
	while (runServers)
	{
		//have each server run a message loop
		for (gproNet::cRakNetServer* server : GameServers)
		{
			server->MessageLoop();
		}
	}

	for (auto* game_server : GameServers)
	{
		delete game_server;
	}
	printf("\n\n");
	system("pause");
}
