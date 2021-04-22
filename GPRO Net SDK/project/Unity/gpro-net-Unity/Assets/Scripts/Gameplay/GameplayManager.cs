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

//struct GameState
//{
//    public string username;
//    public bool isPlayerTurn;
//    public int amBottomRow; // 0 is FALSE 1 is TRUE
//    public int[,] playBoard;

//    public int x, y;
//}



public class GameplayManager : MonoBehaviour
{
    public PlayerMove PlayerMove;
    public int amBottomRow = -1;
    public int[,] playBoard = new int[8, 2];


    void Start()
    {   //X is Left/Right
        //Y is Up/Down
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void SquareSelected(GameObject obj)
    {
        Vector2Int selected = (obj.GetComponent<SlotIndex>()).Slot;
        //gproClientPlugin.ClientSelectionMade(selected.x,selected.y);
        PlayerMove.x = selected.x;
        PlayerMove.y = selected.y;
        PlayerTurn();
    }

    bool PlayerTurn()
    {
        //IS THIS SELECTION ON MY SIDE

        int numRocks = playBoard[PlayerMove.x, PlayerMove.y];
        playBoard[PlayerMove.x, PlayerMove.y] = 0;

        Vector2Int sel = new Vector2Int(PlayerMove.x, PlayerMove.y);
        PlayerMove.x = 0;
        PlayerMove.y = 0;

        bool inAScore = false;
        while (numRocks > 0)
        {
            if (sel.x == 0) //Top Row
            {
                while (sel.y >= amBottomRow)
                {
                    playBoard[sel.x, sel.y]++;
                    numRocks--;
                    if (numRocks < 0)
                    {
                        break;
                    }

                    sel.x--;
                    inAScore = false;
                    if (sel.x == amBottomRow && sel.y == (amBottomRow == 1 ? 7 : 0))
                    {
                        inAScore = true;
                    }
                }

                sel.x = 1;
                sel.y = 1;
            }
            else if (sel.x == 1)
            {
                while (sel.y <= (amBottomRow == 1 ? 7 : 6))
                {
                    playBoard[sel.x, sel.y]++;
                    numRocks--;
                    if (numRocks < 0)
                    {
                        break;
                    }

                    sel.x++;
                    inAScore = false;
                    if (sel.x == amBottomRow && sel.y == (amBottomRow == 1 ? 7 : 0))
                    {
                        inAScore = true;
                    }
                }

                sel.x = 6;
                sel.y = 0;
            }
        } // End while rocks

        //CHECK IF ROCK LANDED WITH NO ROCKS ON OTHER SIDE

        UpdateGameBoard();
        //Did I land in my own goal
        return inAScore;
    }

    void UpdateGameBoard()
    {
        SlotIndex[] arr = GameObject.FindObjectsOfType<SlotIndex>();
        Debug.Log(arr.Length);
        foreach (SlotIndex index in arr)
        {
            index.UpdateSlot(playBoard);
        }
    }
}//End Class---