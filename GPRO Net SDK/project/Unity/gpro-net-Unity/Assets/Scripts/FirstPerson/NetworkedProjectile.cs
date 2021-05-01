using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkedProjectile : MonoBehaviour
{
    private ProjectileMove move;
    private bool UpdatedThisFrame = false;

    void Start()
    {
        gproClientManager.FinishedNetworkUpdate.AddListener(HandleDeleteProj);
    }

    public void NetworkUpdate(ProjectileMove newMove)
    {
        move = newMove;
        transform.position = new Vector3(move.LocX, move.LocY, move.LocZ);
        UpdatedThisFrame = true;
    }

    private void HandleDeleteProj()
    {
        if(UpdatedThisFrame)
        {
            UpdatedThisFrame = false;
        }
        else
        {
            Destroy(gameObject);
        }
    }
}
