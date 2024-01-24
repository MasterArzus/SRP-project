using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Human : MonoBehaviour
{
    public float speedX, speedY;
    int type;
    float angle;
    public void SetSpeed(float x, float y)
    {
        speedX = x;
        speedY = y;
        if (x == 0)
            x = 0.0001f;
        angle=Mathf.Atan(y / x) * Mathf.Rad2Deg;
        if (x < 0)
            angle += 180;
        angle -= 90;
    }
    public int Type(int t)
    {
        if (t == -1)
            return type;
        else
        {
            type = t;
            return type;
        }
    }
    private void Move()
    {
       base.transform.position += new Vector3(speedX*Time.deltaTime, speedY * Time.deltaTime,0);
       base.transform.localEulerAngles = new Vector3(0f, 0f, angle);
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        Move();
    }
}
