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

bool ClientSelectionMade(int x, int y)
{
	if (!g_Interface) return false;

	return g_Interface->PlayerMoveSelected(x, y);
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
	
	strcpy(g_Interface->stuff, ip);
	return g_Interface->ConnectToServer(ip, port);
}

bool DestroyPlugin()
{
	if (!g_Interface) return false;

	delete g_Interface;
	g_Interface = nullptr;
	return true;
}

GameState UpdateOutputRemote()
{
	if (!g_Interface) return GameState();

	return g_Interface->UpdateOutputRemote();
}

GameState UpdateInputRemote()
{
	if (!g_Interface) return GameState();

	return g_Interface->UpdateInputRemote();
}

