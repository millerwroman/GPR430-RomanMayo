using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkedProjectile : MonoBehaviour
{
    private ProjectileMove move;

    public void NetworkUpdate(ProjectileMove newMove)
    {
        move = newMove;
        transform.position = new Vector3(move.LocX, move.LocY, move.LocZ);
    }

    public void DeleteProj()
    {
        Destroy(gameObject);
    }
}
