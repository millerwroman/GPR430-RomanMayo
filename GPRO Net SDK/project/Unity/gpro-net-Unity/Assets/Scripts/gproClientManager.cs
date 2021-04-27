using System;
using System.Runtime.InteropServices;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;


/* THIS IS HOW TO SEND 2D ARRAYS ACROSS SHIT
 https://limbioliong.wordpress.com/2011/06/21/passing-multi-dimensional-managed-array-to-c-part-1/
*/

/* THIS IS HOW TO GET a callback from c++
 https://www.programmersought.com/article/3150696803/
 */


public class gproClientManager : MonoBehaviour
{
    private string IP_ADDRESS = "172.16.2.59";
    private int SERVER_PORT = 7777;
    public GameObject playerPrefab;

    //Players
    private PlayerController localPlayer;
    private List<PlayerMove> networkedMoves = new List<PlayerMove>();
    private Dictionary<int, NetworkedPlayer> networkedPlayers = new Dictionary<int, NetworkedPlayer>();

    void Start()
    {
        localPlayer = GameObject.FindWithTag("Player").GetComponent<PlayerController>();
        gproClientPlugin.InitPlugin();
        Debug.Log("Connected to Server:" + gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT));

        int index = gproClientPlugin.GetLocalPlayerIndex();
        

    }



    private void Update()
    {
        //Input Remote
        gproClientPlugin.UpdateInputRemote();

        //Update
        gproClientPlugin.OutputLocalPlayerState(ref localPlayer.GetPlayerMove());
        GetNetworkedPlayer();
        UpdateNetworkedPlayers();

       




        //Output Remote
        gproClientPlugin.UpdateOutputRemote();
        PrintDebugMessage(gproClientPlugin.DebugMessage());
    }

    void GetNetworkedPlayer()
    {
        networkedMoves.Clear();
        while (true)
        {
            PlayerMove move = new PlayerMove();
            int action = gproClientPlugin.GetNetworkedPlayers(ref move, networkedMoves.Count);
            if (action == 0)
            {
                break;
            }
            if (action == 1)
            {
                networkedMoves.Add(move);
            }
        }
    }

    void UpdateNetworkedPlayers()
    {
        foreach (PlayerMove move in networkedMoves)
        {
            if (!networkedPlayers.ContainsKey(move.PlayerIndex))
            {
               // Debug.Log("New Player! Index: " + move.PlayerIndex);

                //NewPlayerConnected(move);
                continue;
            }
            networkedPlayers[move.PlayerIndex].NetworkUpdate(move);
        }
    }

    void NewPlayerConnected(PlayerMove newPlayer)
    {
        //Instantiate
        GameObject obj = GameObject.Instantiate(playerPrefab);
        NetworkedPlayer cont = obj.GetComponent<NetworkedPlayer>();
        cont.NetworkUpdate(newPlayer);
        networkedPlayers.Add(newPlayer.PlayerIndex, cont);
    }

    void PrintDebugMessage(IntPtr ptr)
    {
        string str = Marshal.PtrToStringAnsi(ptr);
        Debug.Log("DLL Debug: " + str);
    }

    void OnDisable()
    {
        gproClientPlugin.DestroyPlugin();
    }
}
