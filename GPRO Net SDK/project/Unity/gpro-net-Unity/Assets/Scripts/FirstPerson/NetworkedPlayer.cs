﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NetworkedPlayer : MonoBehaviour
{
    private PlayerMove move;
    private Transform gun;


    private void Start()
    {

        gun = transform.GetChild(0);
    }

    public void NetworkUpdate(PlayerMove newMove)
    {
        move = newMove;
        transform.position = new Vector3(move.LocX, move.LocY, move.LocZ);
        gun.rotation = new Quaternion(move.RotX, move.RotY, move.RotZ, move.RotW);

    }
}