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

	gpro-net-Client-Plugin.h
	Main interface for plugin; wraps C++ features with C prototypes.

	****THIS FILE MUST NOT CONTAIN C++ KEYWORDS****
*/

//https://docs.microsoft.com/en-us/dotnet/framework/interop/marshaling-different-types-of-arrays


#ifndef _GPRO_NET_PLUGIN_CLIENT_H_
#define _GPRO_NET_PLUGIN_CLIENT_H_


#include "gpro-net/gpro-net/gpro-net-util/gpro-net-lib.h"
#include "gpro-net/gpro-net/gpro-net-util/gpro-net-gamestate.h"


#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus

	//Start Up
	GPRO_NET_SYMBOL bool InitPlugin();
	GPRO_NET_SYMBOL bool ConnectToServer(const char* ip, int port);
	GPRO_NET_SYMBOL int GetLocalPlayerIndex();

	//Shut Down
	GPRO_NET_SYMBOL bool DestroyPlugin();

	//Update
	GPRO_NET_SYMBOL bool UpdateOutputRemote();
	GPRO_NET_SYMBOL bool UpdateInputRemote();

	//Player State
	GPRO_NET_SYMBOL bool OutputLocalPlayerState(PlayerMove* playerMove);
	GPRO_NET_SYMBOL int GetNetworkedPlayers(PlayerMove* playerMove, int lastCount);

	//Proj State
	GPRO_NET_SYMBOL bool OutputLocalProjState(ProjectileMove* projMoveArr);
	GPRO_NET_SYMBOL int GetNetworkedProjs(ProjectileMove* projMove, int lastCount);

	//Disconnection/Delete
	GPRO_NET_SYMBOL bool ProjectileDeleted(int index);
	GPRO_NET_SYMBOL int GetDeletedProjs(int count);
	
	//Debug
	GPRO_NET_SYMBOL const char* DebugMessage();

#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !_GPRO_NET_PLUGIN_CLIENT_H_