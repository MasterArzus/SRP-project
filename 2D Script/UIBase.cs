using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIBase : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        gameObject.SetActive(false);
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public virtual void close()
    {
        gameObject.SetActive(false);
    }

    public virtual void open()
    {
        gameObject.SetActive(true);
    }

    public virtual void toggle()
    {
        gameObject.SetActive(!gameObject.activeSelf);
    }
    public virtual void Quit()
    {
        Application.Quit();
    }

    private void OnMouseDown()
    {
        // Debug.LogWarning("mouse down");
    }
}
