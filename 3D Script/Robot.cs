using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Robot : MonoBehaviour
{
    float sx, sy;
    public float speed;
    Animator anim;
    Rigidbody rb;
    Vector3 move;
    Vector3 moveAmount;

    private Vector3 dirY;
    private Vector3 rayOrigin;
    private Ray ray;

    public Vector3 mPosition
    {
        get;
        set;
    }
    //if modeling well,follows can be delete
    public bool downStair
    {
        get;set;
    }
    public int floor
    {
        get; set;
    }


    public void fixSpeed(float i, float j, float t)
    {
        t = t * 200;
        float x = GetComponent<Rigidbody>().position.x - i;
        float y = GetComponent<Rigidbody>().position.y - j;
        //speed = Mathf.Sqrt(x * x + y * y)  / t;
    }

    public void SetVelocity(float x, float y)
    {
        sx = x;
        sy = y;
        speed = Mathf.Sqrt(x*x+y*y);
    }
    public void Move()
    {
        transform.LookAt(transform.position + new Vector3(sx, 0, sy));
        move = new Vector3(sx, 0, sy);

        //transform.position += new Vector3(sx, 0, sy) * Time.deltaTime;
        anim.SetFloat("speed", move.magnitude);
    }

    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<Animator>();
        rb= GetComponent<Rigidbody>();
    }


    private void FixedUpdate()
    {
        rayOrigin = new Vector3(this.transform.position.x, this.transform.position.y, this.transform.position.z)
            +this.transform.up*2f + this.transform.forward * speed + this.transform.up * 2;

        ray.origin = rayOrigin;
        ray.direction = new Vector3(0, -1, 0);
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit))
        {
            dirY.x = mPosition.x;
            dirY.z = mPosition.z;
            dirY.y = hit.point.y;
            if (transform.position.y>1f&&transform.position.y - hit.point.y>0.1f)
            {
                dirY.y += 0.8f;
            }
            rb.MovePosition(dirY);
            rb.velocity = move;
            return;
        }

        rb.MovePosition(mPosition);
        rb.velocity = move;
    }

    //Update is called once per frame
    //necessary anim function
    void Update()
    {
        Move();
    }
}
