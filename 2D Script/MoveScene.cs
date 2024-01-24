using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class MoveToScene : MonoBehaviour
{
    void Start()
    {
    }
    void Update()
    {
        if (Input.GetKey(KeyCode.P))
            BackScene();
    }
    public void ChangeScene_2D()/*����һ���л�����*/
    {
        SceneManager.LoadScene("2DScene");
    }
    public void ChangeScene_3D()/*����һ���л�����*/
    {
        SceneManager.LoadScene("3DScene");
    }
    public void BackScene()/*����һ���л�����*/
    {
        SceneManager.LoadScene("StartScene");
    }
}