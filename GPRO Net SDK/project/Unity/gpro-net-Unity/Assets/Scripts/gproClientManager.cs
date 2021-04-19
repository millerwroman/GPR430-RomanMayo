using System;
using System.Runtime.InteropServices;
using UnityEngine;
using System.Collections;


/* THIS IS HOW TO SEND 2D ARRAYS ACROSS SHIT
 https://limbioliong.wordpress.com/2011/06/21/passing-multi-dimensional-managed-array-to-c-part-1/
*/

/* THIS IS HOW TO GET a callback from c++
 https://www.programmersought.com/article/3150696803/
 */


public class gproClientManager : MonoBehaviour
{
    private string IP_ADDRESS = "172.16.2.57";
    private int SERVER_PORT = 7777;

    enum StateEnum
    {
        NO_PLUGIN = 0,
        Waiting_For_Start,
        In_Game,
        End_Game
    }
    private GameplayManager gm;
    private int currentState = 0;
    IEnumerator HandlePlayerAssignment()
    {
        yield return new WaitForEndOfFrame();
    }

    void Start()
    {
        gm = gameObject.GetComponent<GameplayManager>();
        gproClientPlugin.InitPlugin();
        gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT);

        //do
        //{
        //    gm.amBottomRow = gproClientPlugin.GetPlayerAssignment();
        //} while (gm.amBottomRow == -1);

        if (gm.amBottomRow == -2)
        {
            Debug.Assert(true, "NO SERVER CONNECTED");
        }


        //If I am player 2 then the game is ready to go
        //if I am payer 1 then we gotta wait
    }

    // Update is called once per frame
    void Update()
    {
        gproClientPlugin.GetState(ref currentState);
        Debug.Log(currentState);
        switch ((StateEnum)currentState)
        {
            case StateEnum.NO_PLUGIN:
                break;
            case StateEnum.Waiting_For_Start:
                break;
            case StateEnum.In_Game:
                // gproClientPlugin.UpdateInputRemote(ref gs);

                GCHandle handle = GCHandle.Alloc(gm.playBoard, GCHandleType.Pinned);
                IntPtr pArr = handle.AddrOfPinnedObject();
                gproClientPlugin.UpdateOutputRemote(ref gm.PlayerMove, gm.playBoard);
                Foo(gm.PlayerMove);
                handle.Free();
                break;
            case StateEnum.End_Game:
                break;
            default:
                Debug.Assert(true);
                break;
        }

        //gproClientPlugin.ClientSelectionMade(1,2);
        //Get Gamestate
        //Make turn
    }

    void Foo(PlayerMove gs)
    {
        //Debug.Log(gs.x);
        //Debug.Log(gs.y);
        //Debug.Log(gs.amBottomRow);
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                Debug.Log(gm.playBoard[i, j]);
            }
        }
    }
}

//client takes turn
//Sends turn to server
//server says if its your turn or not
//if its client turns it will process that info