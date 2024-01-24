using Cinemachine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour
{

    public float sensitivity = 5f;
    public float speed = 0.5f;

    private float X=0f;
    private float Y=0f;

    float Xmax=60f;
    float Xmin=-60f;
    // Update is called once per frame
    [SerializeField] CinemachineVirtualCamera firstPersonCam;
    [SerializeField] CinemachineVirtualCamera thirdPersonCam;
    [SerializeField] CinemachineVirtualCamera freeCam;
    [SerializeField] CinemachineVirtualCamera doorCam;
    [SerializeField] CinemachineVirtualCamera Camfloor1;
    [SerializeField] CinemachineVirtualCamera Camfloor2;
    [SerializeField] CinemachineVirtualCamera Camfloor3;

    private void Start()
    {
        CameraSwitcher.Register(firstPersonCam);
        CameraSwitcher.Register(thirdPersonCam);
        CameraSwitcher.Register(freeCam);
        CameraSwitcher.Register(doorCam);
        CameraSwitcher.Register(Camfloor1);
        CameraSwitcher.Register(Camfloor2);
        CameraSwitcher.Register(Camfloor3);
    }


    void Update()
    {
        CameraListen();
        Moving();
    }

    /// <summary>
    /// change camera
    /// alpha0 means doorcamera
    /// alpha1~3 means floor 1~3 camera
    /// </summary>
    private void CameraListen()
    {
        if (Input.GetKey(KeyCode.T))
        {
            CameraSwitcher.SwitchCamera(freeCam);
        }
        if (Input.GetKey(KeyCode.Alpha0))
        {
            CameraSwitcher.SwitchCamera(doorCam);
        }
        if (Input.GetKey(KeyCode.Alpha1))
        {
            CameraSwitcher.SwitchCamera(Camfloor1);
        }
        if (Input.GetKey(KeyCode.Alpha2))
        {
            CameraSwitcher.SwitchCamera(Camfloor2);

        }
        if (Input.GetKey(KeyCode.Alpha3))
        {
            CameraSwitcher.SwitchCamera(Camfloor3);

        }
    }

    void Moving()
    {
        if (Input.GetKey(KeyCode.W))
        {
            transform.Translate(Vector3.forward * speed, Space.Self);
        }
        if (Input.GetKey(KeyCode.S))
        {
            transform.Translate(Vector3.back * speed, Space.Self);
        }

        if (Input.GetKey(KeyCode.A))
        {
            transform.Translate(Vector3.left * speed, Space.Self);
        }

        if (Input.GetKey(KeyCode.D))
        {
            transform.Translate(Vector3.right * speed, Space.Self);
        }
        if (Input.GetKey(KeyCode.Space))
        {
            transform.Translate(Vector3.up * speed, Space.Self);
        }
        if (Input.GetKey(KeyCode.LeftShift))
        {
            transform.Translate(Vector3.down * speed, Space.Self);
        }

        //transform.Rotate(Vector3.up, Input.GetAxis("Mouse X") * sensitity);
        //transform.Rotate(Vector3.left, Input.GetAxis("Mouse Y") * sensitity);
        X += Input.GetAxis("Mouse X") * sensitivity;
        Y += Input.GetAxis("Mouse Y") * sensitivity;
        Y = Mathf.Clamp(Y, Xmin, Xmax);

        this.transform.localEulerAngles = new Vector3(-Y, X, 0);
    }
}
