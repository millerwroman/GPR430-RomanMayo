using UnityEngine;

public class gproClientManager : MonoBehaviour
{
    private string IP_ADDRESS = "172.16.2.59"; 
    private int SERVER_PORT = 7777;
    void Start()
    {
        gproClientPlugin.InitPlugin();
        gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT);
    }

    // Update is called once per frame
    void Update()
    {
        gproClientPlugin.UpdateInputRemote();
        gproClientPlugin.ClientSelectionMade(1,2);
        //Get Gamestate
        //Make turn
        gproClientPlugin.UpdateOutputRemote();
    }
}
