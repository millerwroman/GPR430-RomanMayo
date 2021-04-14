using System;
using System.Runtime.InteropServices;
using UnityEngine;


/* THIS IS HOW TO SEND 2D ARRAYS ACROSS SHIT
 https://limbioliong.wordpress.com/2011/06/21/passing-multi-dimensional-managed-array-to-c-part-1/
*/



public class gproClientManager : MonoBehaviour
{
    private string IP_ADDRESS = "172.16.2.186"; 
    private int SERVER_PORT = 7777;
    int[,] arr = new int[2,8];

    void Start()
    {
        gproClientPlugin.InitPlugin();
        gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT);
    }

    // Update is called once per frame
    void Update()
    {
        GameStateTest gs = new GameStateTest();   
        //gproClientPlugin.UpdateInputRemote(ref gs);
        //gproClientPlugin.ClientSelectionMade(1,2);
        //Get Gamestate
        //Make turn
       
        GCHandle handle = GCHandle.Alloc(arr,GCHandleType.Pinned);
        IntPtr pArr = handle.AddrOfPinnedObject();
        gproClientPlugin.UpdateOutputRemote(ref gs, arr);
        Foo(gs);
        handle.Free();
    }

    void Foo(GameStateTest gs)
    {
        //Debug.Log(gs.x);
        //Debug.Log(gs.y);
        //Debug.Log(gs.amBottomRow);
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                Debug.Log(arr[i,j]);
            }
        }
    }
}

//cliewnt takes turn
//Sends turn to server
//server says if its your turn or not
//if its client turns it will process that info