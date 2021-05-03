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

	gpro-net-gamestate.h
	Header for common networking mini-game states.
*/
#ifndef _GPRO_NET_GAMESTATE_H_
#define _GPRO_NET_GAMESTATE_H_
#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus


	/*
		Mancala:
			[2]        ___________________________
					  /				7			  \
				 _______________________________________
			 0	| 0  | 1  | 2  | 3  | 4  | 5  | 6  |    |	[8]
				|    |____|____|____|____|____|____|    |
			 1	|    | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
				|____|____|____|____|____|____|____|____|

					  \___________________________/
									7
	*/

	//struct GameState
	//{
	//	int playerTurn;
	//	int x, y;
	//	int board[8][2];
	//};

	struct PlayerMove
	{
		int playerIndex;
		
		float LocX;
		float LocY;
		float LocZ;

		float RotX;
		float RotY;
		float RotZ;
		float RotW;

		float VelX;
		float VelY;
		float VelZ;
	};

	struct ProjectileMove
	{
		int ProjIndex;

		float LocX;
		float LocY;
		float LocZ;
	};




#ifdef __cplusplus
};
#endif	// __cplusplus


#endif	// !_GPRO_NET_GAMESTATE_H_