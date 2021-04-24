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
    private string IP_ADDRESS = "172.16.2.59";
    private int SERVER_PORT = 7777;
    private PlayerController localPlayer;

    void Start()
    {
        localPlayer = GameObject.FindWithTag("Player").GetComponent<PlayerController>();
        Debug.Log("Bob");
        Debug.Log("Init Plugin:" + gproClientPlugin.InitPlugin());
        Debug.Log("Connecting to Server:" + gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT));
    }

    private void Update()
    {
        //Input Remote
        gproClientPlugin.UpdateInputRemote();

        //Update Info
        PlayerMove move = new PlayerMove();
        localPlayer.GetPlayerMove(ref move);
        gproClientPlugin.OutputLocalPlayerState(ref move);

        //Output Remote
        gproClientPlugin.UpdateOutputRemote();
    }
}
