using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct GameState
{
    public string username;
    public int isPlayerTurn;
    public int amBottomRow; // 0 is FALSE 1 is TRUE
    public int[,] playBoard;

    public int x;
    public int y;
};

[StructLayout(LayoutKind.Sequential)]
public struct GameStateTest
{
    public int isPlayerTurn;
    public int amBottomRow; // 0 is FALSE 1 is TRUE
    public int x;
    public int y;
};

public class gproClientPlugin
{
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool InitPlugin();
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool ConnectToServer(string ip, int port);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool DestroyPlugin();


    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.StdCall)]
    public static extern bool UpdateOutputRemote(ref GameStateTest gameState, [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I4)]int[,] pArray);


    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateInputRemote(ref GameState gameState);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool ClientSelectionMade(int x, int y, int PlayerIndex); //Edit
}