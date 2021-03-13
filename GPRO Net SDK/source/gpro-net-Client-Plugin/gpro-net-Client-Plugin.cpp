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
#include "gpro-net/gpro-net/gpro-net-util/gpro-net-gamestate.h"

GameState gameState;

int foo(int bar)
{
	return (bar + 1);
}

void ClientSelectionMade(int x, int y)
{
	gameState.x = x;
	gameState.y = y;

	//Package and send message to server
}

void SendMessages()
{
	
}


