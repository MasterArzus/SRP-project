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
    public void ChangeScene_2D()/*定义一个切换场景*/
    {
        SceneManager.LoadScene("2DScene");
    }
    public void ChangeScene_3D()/*定义一个切换场景*/
    {
        SceneManager.LoadScene("3DScene");
    }
    public void BackScene()/*定义一个切换场景*/
    {
        SceneManager.LoadScene("StartScene");
    }
}