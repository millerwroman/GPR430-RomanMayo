using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimedDestroy : MonoBehaviour
{
    public float lifeTime = 7.0f;
    void Start()
    {
        StartCoroutine(TimeDelete());
    }

    IEnumerator TimeDelete()
    {
        yield return new WaitForSecondsRealtime(lifeTime);
        Destroy(gameObject);
    }
}
