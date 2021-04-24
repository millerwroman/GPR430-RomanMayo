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
};

public class gproClientPlugin
{
    //Start up
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool InitPlugin();
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool ConnectToServer(string ip, int port);

    //Shut down
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool DestroyPlugin();

    //Update
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.StdCall)]
    public static extern bool UpdateOutputRemote();
    [DllImport("gpro-net-Client-Plugin")]
    public static extern bool UpdateInputRemote();

    //Player State
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.StdCall)]
    public static extern bool OutputLocalPlayerState(ref PlayerMove playerMove);


    //Debug
    [DllImport("gpro-net-Client-Plugin", CallingConvention = CallingConvention.StdCall)]
    public static extern IntPtr DebugMessage();
}