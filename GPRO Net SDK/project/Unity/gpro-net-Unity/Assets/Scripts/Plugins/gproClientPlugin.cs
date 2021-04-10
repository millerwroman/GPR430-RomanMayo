using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System.Runtime.InteropServices;

public struct GameState
{
    public char[] username;
    public bool isPlayerTurn;
    public int amBottomRow; // 0 is FALSE 1 is TRUE
    public int[,] playBoard;

    public int x, y;
};

public class gproClientPlugin
{
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool InitPlugin();
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool ConnectToServer(string ip, int port);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool DestroyPlugin();
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool UpdateOutputRemote(ref GameState gameState);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateInputRemote(ref GameState gameState);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool ClientSelectionMade(int x, int y, int PlayerIndex); //Edit
}