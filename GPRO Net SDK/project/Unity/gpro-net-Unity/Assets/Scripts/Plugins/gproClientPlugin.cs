using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.Runtime.InteropServices;

public class gproClientPlugin
{
    [DllImport("gpro-net-Client-Plugin")]
    public static extern int foo(int bar);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern void ClientSelectionMade(int x, int y);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern void SendMessages();
}
