using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.Runtime.InteropServices;

public class gproClientPlugin
{
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool ClientSelectionMade(int x, int y);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool InitPlugin();
    //[DllImport("gpro-net-Client-Plugin")]
    //public static extern bool ConnectToServer(char* ip, int port);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool DestroyPlugin();
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateOutputRemote();
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateInputRemote();
}