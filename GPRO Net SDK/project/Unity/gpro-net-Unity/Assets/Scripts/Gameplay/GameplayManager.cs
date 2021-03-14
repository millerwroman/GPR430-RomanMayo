using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using UnityEngine;
using UnityEngine.UI;

//Slot [0][0] player 1 (self) score
//Slot [1][0] player 2 score

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

struct GameState
{
    public string username;
    public bool isPlayerTurn;
    public int amBottomRow; // 0 is FALSE 1 is TRUE
    public int[,] playBoard;

    public Vector2Int selection;
}

public class GameplayManager : MonoBehaviour
{
    private GameState gameState;


    void Start()
    {   //X is Left/Right
        //Y is Up/Down
        gameState = new GameState { playBoard = new int[8, 2] };
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void SquareSelected(GameObject obj)
    {
        Vector2Int selected = (obj.GetComponent<SlotIndex>()).Slot;
        gproClientPlugin.ClientSelectionMade(selected.x,selected.y);
    }

    bool PLayerTurn()
    {
        //IS THIS SELECTION ON MY SIDE

        int numRocks = gameState.playBoard[gameState.selection.x, gameState.selection.y];
        gameState.playBoard[gameState.selection.x, gameState.selection.y] = 0;

        Vector2Int sel = gameState.selection;
        gameState.selection = Vector2Int.zero;

        bool inAScore = false;
        while (numRocks > 0)
        {
            if (sel.x == 0) //Top Row
            {
                while (sel.y >= gameState.amBottomRow)
                {
                    gameState.playBoard[sel.x, sel.y]++;
                    numRocks--;
                    if (numRocks < 0)
                    {
                        break;
                    }

                    sel.x--;
                    inAScore = false;
                    if (sel.x == gameState.amBottomRow && sel.y == (gameState.amBottomRow == 1 ? 7 : 0))
                    {
                        inAScore = true;
                    }
                }

                sel.x = 1;
                sel.y = 1;
            }
            else if (sel.x == 1)
            {
                while (sel.y <= (gameState.amBottomRow == 1 ? 7 : 6))
                {
                    gameState.playBoard[sel.x, sel.y]++;
                    numRocks--;
                    if (numRocks < 0)
                    {
                        break;
                    }

                    sel.x++;
                    inAScore = false;
                    if (sel.x == gameState.amBottomRow && sel.y == (gameState.amBottomRow == 1 ? 7 : 0))
                    {
                        inAScore = true;
                    }
                }

                sel.x = 6;
                sel.y = 0;
            }
        } // End while rocks

        //CHECK IF ROCK LANDED WITH NO ROCKS ON OTHER SIDE

        //Did I land in my own goal
        return inAScore;
    }


}//End Class---