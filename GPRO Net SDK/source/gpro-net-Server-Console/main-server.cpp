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


int main(int const argc, char const* const argv[])
{
	gproNet::cRakNetServer* server = new gproNet::cRakNetServer();


	while (true)
	{
		server->MessageLoop();
	}

	printf("\n\n");
	system("pause");
}
