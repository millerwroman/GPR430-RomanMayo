using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SlotIndex : MonoBehaviour
{
    public Vector2Int Slot;

    public void UpdateSlot(int[,] gameBoard)
    {
        Text t = gameObject.GetComponentInChildren<Text>();
        t.text = gameBoard[Slot.y, Slot.x].ToString();
    }
}
