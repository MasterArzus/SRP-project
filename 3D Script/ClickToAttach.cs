using Cinemachine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;


public class ClickToAttach : MonoBehaviour, IPointerDownHandler
{
    [SerializeField] CinemachineVirtualCamera firstPersonCam;
    [SerializeField] CinemachineVirtualCamera thirdPersonCam;
    // Start is called before the first frame update
    public void OnPointerClick(PointerEventData eventData)
    {


    }

    public void OnPointerDown(PointerEventData eventData)
    {
        thirdPersonCam.Follow = this.transform;
        firstPersonCam.Follow= this.transform.Find("HeadPosition");
        CameraSwitcher.SwitchCamera(thirdPersonCam);
    }

    private void Update()
    {
        float sroll = Input.GetAxis("Mouse ScrollWheel");
        if (sroll == 0.1f&&CameraSwitcher.IsActiveCamera(thirdPersonCam))
        {
            CameraSwitcher.SwitchCamera(firstPersonCam);
        }
        if(sroll == -0.1f&& CameraSwitcher.IsActiveCamera(firstPersonCam))
        {
            CameraSwitcher.SwitchCamera(thirdPersonCam);
        }

    }
}
