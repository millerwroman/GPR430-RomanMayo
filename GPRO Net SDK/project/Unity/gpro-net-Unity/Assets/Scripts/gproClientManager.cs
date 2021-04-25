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
    private string IP_ADDRESS = "172.16.2.51";
    private int SERVER_PORT = 7777;
    public GameObject playerPrefab;

    //Players
    private PlayerController localPlayer;
    private List<PlayerMove> networkedMoves = new List<PlayerMove>();
    private Dictionary<int, PlayerController> networkedPlayers = new Dictionary<int, PlayerController>();

    void Start()
    {
        localPlayer = GameObject.FindWithTag("Player").GetComponent<PlayerController>();
        gproClientPlugin.InitPlugin();
        Debug.Log("Connected to Server:" + gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT));
        StartCoroutine(GetLocalPlayerIndex());
    }

    IEnumerator GetLocalPlayerIndex()
    {
        switch (gproClientPlugin.GetLocalPlayerIndex())
        {
            case -1:
                //Possible stack overflow
                yield return new WaitWhile(() => gproClientPlugin.GetLocalPlayerIndex() == -1);
                break;
            case -2:
                Debug.Log("NO PLUGIN");
                break;
            default:
                localPlayer.SetPlayerIndex(gproClientPlugin.GetLocalPlayerIndex());
                break;
        }
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
       // PrintDebugMessage(gproClientPlugin.DebugMessage());

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
            networkedPlayers[move.PlayerIndex].NetworkUpdate(move);
        }
    }

    void NewPlayerConnected(PlayerMove newPlayer)
    {
        //Instantiate
        GameObject obj = GameObject.Instantiate(playerPrefab);
        PlayerController cont =  obj.GetComponent<PlayerController>();
        cont.NetworkUpdate(newPlayer);
        networkedPlayers.Add(newPlayer.PlayerIndex, cont);
    }

    void PrintDebugMessage(IntPtr ptr)
    {
        string str = Marshal.PtrToStringAnsi(ptr);
        Debug.Log(str);
    }

    void OnDisable()
    {
        gproClientPlugin.DestroyPlugin();
    }
}
