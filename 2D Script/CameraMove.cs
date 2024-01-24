using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMove : MonoBehaviour
{
    float speed = 25;
    float scrollSpeed = 10;
    Camera c;
    // Start is called before the first frame update
    void Start()
    {
        c = gameObject.GetComponent<Camera>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.W))
        {
            transform.Translate(speed * Vector3.up * Time.deltaTime);
        }
        if (Input.GetKey(KeyCode.S))
        {
            transform.Translate(speed * Vector3.down * Time.deltaTime);
        }
        if (Input.GetKey(KeyCode.A))
        {
            transform.Translate(speed * Vector3.left * Time.deltaTime);
        }
        if (Input.GetKey(KeyCode.D))
        {
            transform.Translate(speed * Vector3.right * Time.deltaTime);
        }
        if( Input.GetAxis("Mouse ScrollWheel") != 0)
        {
            if(Input.GetAxis("Mouse ScrollWheel") < 0 && c.orthographicSize <= 25)
            {
                c.orthographicSize -= scrollSpeed * Input.GetAxis("Mouse ScrollWheel");
            }
            if (Input.GetAxis("Mouse ScrollWheel") > 0 && c.orthographicSize >= 5)
            {
                c.orthographicSize -= scrollSpeed * Input.GetAxis("Mouse ScrollWheel");
            }
        }
    }
}
