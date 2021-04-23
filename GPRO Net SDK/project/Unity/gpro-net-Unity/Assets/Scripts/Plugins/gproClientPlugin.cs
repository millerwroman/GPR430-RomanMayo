using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct PlayerMove
{
    public float LocX;
    public float LocY;
    public float LocZ;

    public float RotX;
    public float RotY;
    public float RotZ;
    public float RotW;

    public int Shoot;
};

public class gproClientPlugin
{
    //Start up
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool InitPlugin();
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool ConnectToServer(string ip, int port);
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetPlayerAssignment();

    //State
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetState(ref int state);

    //Shut down
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool DestroyPlugin();

    //Update
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.StdCall)]
    public static extern bool UpdateOutputRemote(ref PlayerMove gameState, [In][MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I4)] int[,] pArray);
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateInputRemote(ref PlayerMove playerMove);

    //Client Selection
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool ClientSelectionMade(int x, int y, int PlayerIndex); //Edit
}