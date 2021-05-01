﻿using System;
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
    [Header("Networked Object Prefabs")]
    public GameObject playerPrefab;
    public GameObject projectilePrefab;

    //Networked Players
    private List<PlayerMove> networkedMoves = new List<PlayerMove>();
    private Dictionary<int, NetworkedPlayer> networkedPlayers = new Dictionary<int, NetworkedPlayer>();

    //Networked Projs
    private List<ProjectileMove> networkedProjMoves = new List<ProjectileMove>();
    private Dictionary<int, NetworkedProjectile> networkedProjectiles = new Dictionary<int, NetworkedProjectile>();

    //Local Shit
    private PlayerController localPlayer;
    private Dictionary<int, TimedDestroy> localProjs = new Dictionary<int, TimedDestroy>();
    private int NextProjIndex = 0;


    private static gproClientManager _instance;
    public static gproClientManager Instance
    {
        get
        {
            if (_instance == null)
            {
                _instance = GameObject.FindObjectOfType<gproClientManager>();
            }
            return _instance;
        }
    }

    public int GetNextProjIndex()
    {
        ++NextProjIndex;
        return NextProjIndex;
    }

    void Start()
    {
        localPlayer = GameObject.FindWithTag("Player").GetComponent<PlayerController>();
        gproClientPlugin.InitPlugin();
        Debug.Log("Connected to Server:" + gproClientPlugin.ConnectToServer(IP_ADDRESS, SERVER_PORT));
        localPlayer.SetPlayerIndex(gproClientPlugin.GetLocalPlayerIndex());
    }



    private void Update()
    {
        if (localPlayer.GetPlayerMove().PlayerIndex == -1)
        {
            localPlayer.SetPlayerIndex(gproClientPlugin.GetLocalPlayerIndex());
            gproClientPlugin.UpdateInputRemote();
        }
        else
        {
            //Input Remote
            gproClientPlugin.UpdateInputRemote();

            //Update Players
            gproClientPlugin.OutputLocalPlayerState(ref localPlayer.GetPlayerMove());
            GetNetworkedPlayer();
            UpdateNetworkedPlayers();

            //Update Proj
            // This will need to loop and shit
            OutputLocalProjs();
            GetNetworkedProjs();
            UpdateNetworkedProjs();

            //Delete/Disconnect
            HandleDeletedProjs();




            //Output Remote
            gproClientPlugin.UpdateOutputRemote();

        }
        PrintDebugMessage(gproClientPlugin.DebugMessage());
    }

    void HandleDeletedProjs()
    {
        int count = 0;
        while (true)
        {
            int i = gproClientPlugin.GetDeletedProjs(count);
            ++count;
            if (i == -1)
            {
                break;
            }
            Debug.Log("delete Index: " + i);
            networkedProjectiles[i].DeleteProj();
            networkedProjectiles.Remove(i);
        }
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
            if (!networkedPlayers.ContainsKey(move.PlayerIndex))
            {
                NewPlayerConnected(move);
            }
            else
            {
                networkedPlayers[move.PlayerIndex].NetworkUpdate(move);
            }
        }
    }

    void OutputLocalProjs()
    {
        foreach (KeyValuePair<int, TimedDestroy> i in localProjs)
        {
            gproClientPlugin.OutputLocalProjState(ref i.Value.GetMove());
        }
    }

    void GetNetworkedProjs()
    {
        networkedProjMoves.Clear();
        while (true)
        {
            ProjectileMove move = new ProjectileMove();
            int action = gproClientPlugin.GetNetworkedProjs(ref move, networkedProjMoves.Count);
            if (action == 0)
            {
                break;
            }
            if (action == 1)
            {
                networkedProjMoves.Add(move);
            }
        }
    }

    void UpdateNetworkedProjs()
    {
        foreach (ProjectileMove move in networkedProjMoves)
        {
            if (!networkedProjectiles.ContainsKey(move.ProjIndex))
            {
                Debug.Log("New Proj Index: " + move.ProjIndex);
                NewProjectileFound(move);
            }
            else
            {
                Debug.Log("Update Index: " + move.ProjIndex);
                networkedProjectiles[move.ProjIndex].NetworkUpdate(move);
            }
        }
    }



    void NewPlayerConnected(PlayerMove newPlayer)
    {

        //Instantiate
        GameObject obj = GameObject.Instantiate(playerPrefab);
        NetworkedPlayer cont = obj.GetComponent<NetworkedPlayer>();
        cont.NetworkUpdate(newPlayer);
        networkedPlayers.Add(newPlayer.PlayerIndex, cont);
    }

    void NewProjectileFound(ProjectileMove newProj)
    {
        GameObject obj = GameObject.Instantiate(projectilePrefab);
        NetworkedProjectile cont = obj.GetComponent<NetworkedProjectile>();
        cont.NetworkUpdate(newProj);
        networkedProjectiles.Add(newProj.ProjIndex, cont);
    }

    void PrintDebugMessage(IntPtr ptr)
    {
        string str = Marshal.PtrToStringAnsi(ptr);
        Debug.Log("DLL Debug: " + str);
    }

    void OnDisable()
    {
        gproClientPlugin.DestroyPlugin();
    }

    public void AddLocalProj(TimedDestroy proj)
    {
        localProjs.Add(proj.GetMove().ProjIndex, proj);
    }
    public void RemoveLocalProj(TimedDestroy proj)
    {
        localProjs.Remove(proj.GetMove().ProjIndex);
        gproClientPlugin.ProjectileDeleted(proj.GetMove().ProjIndex);
    }
}
