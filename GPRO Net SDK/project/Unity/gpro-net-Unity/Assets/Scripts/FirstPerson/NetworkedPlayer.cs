using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkedPlayer : MonoBehaviour
{
    private PlayerMove move;
    public Transform gun;
    private bool updatedThisFrame = false;
    private int frameDelay = 0;

    private void Start()
    {
        gproClientManager.FinishedNetworkUpdate.AddListener(HandleDelete);
    }

    public void NetworkUpdate(PlayerMove newMove)
    {
        move = newMove;
        transform.position = new Vector3(move.LocX, move.LocY, move.LocZ);
        gun.rotation = new Quaternion(move.RotX, move.RotY, move.RotZ, move.RotW);
        updatedThisFrame = true;
    }

    private void Update()
    {

    }

    private void HandleDelete()
    {
        ++frameDelay;      
        if (updatedThisFrame)
        {
            updatedThisFrame = false;
            frameDelay = 0;
        }
        else if (frameDelay > 10 && !updatedThisFrame)
        {
            gproClientManager.Instance.RemoveNetworkedPlayer(move.PlayerIndex);
            Destroy(gameObject);
        }
    }
}
