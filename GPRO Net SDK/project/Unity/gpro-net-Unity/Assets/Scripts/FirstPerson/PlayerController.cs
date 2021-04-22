using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerController : MonoBehaviour
{
    [Header("Movement")]
    [SerializeField]
    private float walkSpeed = 2.0f;
    [SerializeField]
    private float sprintSpeed = 10.0f;
    [SerializeField]
    private float jumpHeight = 1.0f;
    [SerializeField]
    private float gravityValue = -9.81f;
    [Header("Camera")]
    [SerializeField]
    private float rotationDevider = 5.0f;
    private float rotationDeviderRecip;
    [SerializeField]
    [Range(0f, 170f)] private float maxCameraAngle = -170f;
    [SerializeField]
    [Range(-170f, 0f)] private float minCameraAngle = 170f;

    //Controller
    CharacterController controller = null;
    private Vector3 playerVelocity;
    private Vector3 playerMoveInput;
    //Run/Spring
    private bool shouldJump = false;
    private bool shouldSprint = false;

    void Awake()
    {
        controller = GetComponent<CharacterController>();
        rotationDeviderRecip = 1 / rotationDevider;
    }
    void FixedUpdate()
    {
        MovePlayer();
    }
    private void MovePlayer()
    {
        Debug.Log(playerMoveInput);
        if (controller.isGrounded)
        {
            playerVelocity.y = 0.0f;
        }
        playerVelocity.x = playerMoveInput.x * (shouldSprint ? sprintSpeed : walkSpeed);
        playerVelocity.z = playerMoveInput.z * (shouldSprint ? sprintSpeed : walkSpeed);
        playerVelocity = transform.TransformDirection(playerVelocity);
        if (shouldJump && controller.isGrounded)
        {
            playerVelocity.y += jumpHeight;
        }
        shouldJump = false;
        playerVelocity.y += gravityValue * Time.deltaTime;
        controller.Move(playerVelocity * Time.deltaTime);
    }

    public void OnYMove(InputValue value)
    {
        playerMoveInput.x = value.Get<float>();
    }
    public void OnXMove(InputValue value)
    {
        playerMoveInput.z = -value.Get<float>();
    }
    public void OnJump()
    {
        shouldJump = true;
    }

    public void OnLook(InputValue value)
    {
        //Character Rotation
        Vector3 deltaRotation = new Vector3(0, value.Get<Vector2>().x, 0);
        deltaRotation *= rotationDeviderRecip;
        transform.Rotate(deltaRotation);
        Vector3 cameraRotation = Camera.main.transform.rotation.eulerAngles;
        //Remap camera angle
        cameraRotation.x -= value.Get<Vector2>().y * rotationDeviderRecip;
        cameraRotation.x = (cameraRotation.x + 180f) % 360f;
        cameraRotation.x = Mathf.Clamp(cameraRotation.x, (minCameraAngle + 180), (maxCameraAngle + 180));
        cameraRotation.x -= 180f;
        Camera.main.transform.rotation = Quaternion.Euler(cameraRotation);
    }
    public void OnSprint(InputValue value)
    {
        shouldSprint = (int)value.Get<float>() != 0;
    }

}
