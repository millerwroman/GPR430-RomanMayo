using UnityEngine;

public class gproClientManager : MonoBehaviour
{
    private string IP_ADDRESS = "172.16.2.186"; 
    private int SERVER_PORT = 7777;

    void Start()
    {
        gproClientPlugin.InitPlugin();
        gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT);
    }

    // Update is called once per frame
    void Update()
    {
        GameState gs = new GameState();   
        gproClientPlugin.UpdateInputRemote(ref gs);
        //gproClientPlugin.ClientSelectionMade(1,2);
        //Get Gamestate
        //Make turn
        gproClientPlugin.UpdateOutputRemote(ref gs);

        Foo(gs);
    }

    void Foo(GameState gs)
    {
        Debug.Log(gs.username);
    }
}

//cliewnt takes turn
//Sends turn to server
//server says if its your turn or not
//if its client turns it will process that info