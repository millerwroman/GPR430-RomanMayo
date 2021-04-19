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

	gpro-net-Client-Plugin.cpp
	Main implementation for plugin; instantiate and manage networking.
*/

#include "gpro-net-Client-Plugin.h"

#include <iostream>

#include "NetworkInterface.h"

static NetworkInterface* g_Interface;

bool ClientSelectionMade(int x, int y, int playerIndex)
{
	if (!g_Interface) return false;

	return g_Interface->PlayerMoveSelected(x, y, playerIndex);
}

bool InitPlugin()
{
	if (g_Interface) return false;

	g_Interface = new NetworkInterface();
	return true;
}

bool ConnectToServer(const char* ip, int port)
{
	if (!g_Interface) return false;

	return g_Interface->ConnectToServer(ip, port);
}

bool DestroyPlugin()
{
	if (!g_Interface) return false;

	delete g_Interface;
	g_Interface = nullptr;
	return true;
}

bool UpdateOutputRemote(PlayerTurn* playerTurn, int* pArr)
{
	if (!g_Interface) return false;
	//g_Interface->UpdateOutputRemote();
	return true;
}

bool UpdateInputRemote(PlayerMove* gameState)
{
	if (!g_Interface) return false;
	//g_Interface->UpdateInputRemote(gameState);

	return true;
}



//strcpy(gameState->username, "asdasdasdasd");
int GetPlayerAssignment()
{
	if (!g_Interface) return -2;
	return g_Interface->GetPlayerAssignment();
}

void GetState(int* state)
{
	if (!g_Interface)
	{
		*state = -5;
		return;
	}

	*state = g_Interface->GetState();

}
