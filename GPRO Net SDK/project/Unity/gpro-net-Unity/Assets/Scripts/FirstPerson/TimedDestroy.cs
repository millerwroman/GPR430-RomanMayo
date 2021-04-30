using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimedDestroy : MonoBehaviour
{
    public float lifeTime = 7.0f;
    private ProjectileMove move;
    void Start()
    {
        move.ProjIndex =  gproClientManager.Instance.GetNextProjIndex();
        gproClientManager.Instance.AddLocalProj(this);
        StartCoroutine(TimeDelete());
    }

    void Update()
    {
        move.LocX = transform.position.x;
        move.LocY = transform.position.y;
        move.LocZ = transform.position.z;
    }

    public ref ProjectileMove GetMove()
    {
        return ref move;
    }

    IEnumerator TimeDelete()
    {
        yield return new WaitForSecondsRealtime(lifeTime);
        gproClientManager.Instance.RemoveLocalProj(this);
        Destroy(gameObject);
    }
}
