using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkedProjectile : MonoBehaviour
{
    private ProjectileMove move;
    private bool updatedThisFrame = false;
    private int frameDelay = 0;
    void Start()
    {
        gproClientManager.FinishedNetworkUpdate.AddListener(HandleDeleteProj);
    }

    public void NetworkUpdate(ProjectileMove newMove)
    {
        move = newMove;
        transform.position = new Vector3(move.LocX, move.LocY, move.LocZ);
        updatedThisFrame = true;
    }

    private void HandleDeleteProj()
    {
        ++frameDelay;
        if (updatedThisFrame)
        {
            updatedThisFrame = false;
            frameDelay = 0;
        }
        else if (frameDelay > 10 && !updatedThisFrame)
        {
            gproClientManager.Instance.RemoveNetworkedProj(move.ProjIndex);
            Destroy(gameObject);

        }
    }
}
