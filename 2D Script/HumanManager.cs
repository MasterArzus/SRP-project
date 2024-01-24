using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HumanManager : MonoBehaviour
{
    int n = 0;
    float timer = 0;
    float totalTime = 0.02f;
    int number = 0;
    TextAsset txt;
    string[] str;
    Vector3 positon = new Vector3(0f,0f,0f);
    [SerializeField] GameObject[] prefabs;
    List<GameObject> humanList = new List<GameObject>();
    List<List<GameObject>> humanPool = new List<List<GameObject>>();

    void SetBackGround()
    {
        string strs = new string(str[n]);
        string[] ss = strs.Split(',');
        float scale = float.Parse(ss[1]);
        float width = float.Parse(ss[2]);
        float length = float.Parse(ss[3]);
        GameObject.Find("BackGround").GetComponent<Transform>().position = new Vector3(width/2/scale,length/2/scale,0);
        GameObject.Find("BackGround").GetComponent<Transform>(). localScale= new Vector3(scale, scale, 0);
        n++;
        number = int.Parse(str[1]);
        n++;
    }
    void MoveStart()
    {
       string strs =new string(str[n]);
       while (n-1<=number)
       {
            int i = Random.Range(0, prefabs.Length);
            GameObject instance = Instantiate(prefabs[i]);
            instance.GetComponent<Human>().Type(i);
            string[] ss = strs.Split(',');
            positon.x = float.Parse(ss[1]);
            positon.y = float.Parse(ss[2]);
            positon.z = 1f;
            instance.transform.position = positon;
            humanList.Add(instance);
            n++;
            strs = new string(str[n]);
       }
        Debug.Log(n);
    }
    GameObject GetPoolItem(int type)
    {
        if (humanPool[type].Count==0)
        {
            GameObject tempObject = Instantiate(prefabs[type]);
            return tempObject;
        }
        else
        {
            GameObject tempObject = humanPool[type][humanPool[type].Count - 1];
            humanPool[type].RemoveAt(humanPool[type].Count - 1);
            tempObject.SetActive(true);
            return tempObject;
        }
    }
    void TimeStep()
    {
        if (n < str.Length)
        {
            int num = int.Parse(str[n]);
            n++;
            if (num <= humanList.Count)
            {
                for (int i = 0; i < num; i++)
                {
                    string strs = str[n];
                    string[] ss = strs.Split(',');
                    positon.x = float.Parse(ss[1]);
                    positon.y = float.Parse(ss[2]);
                    positon.z = 1f;
                    humanList[i].transform.position = positon;
                    humanList[i].GetComponent<Human>().SetSpeed(float.Parse(ss[3]), float.Parse(ss[4]));
                    n++;
                }
                for(int i=humanList.Count-1; i>=num;i++)
                {
                    Debug.Log(n);
                    humanList[i].SetActive(false);
                    humanPool[humanList[i].GetComponent<Human>().Type(-1)].Add(humanList[i]);
                    humanList.RemoveAt(i);
                }
            }
            else
            {
                Debug.Log(n);
                for (int i = 0; i < humanList.Count; i++)
                {
                    string strs = str[n];
                    string[] ss = strs.Split(',');
                    positon.x = float.Parse(ss[1]);
                    positon.y = float.Parse(ss[2]);
                    positon.z = 1f;
                    humanList[i].transform.position = positon;
                    humanList[i].GetComponent<Human>().SetSpeed(float.Parse(ss[3]), float.Parse(ss[4]));
                    n++;
                }
                for (int i = humanList.Count; i < num; i++)
                {
                    int type = Random.Range(0, prefabs.Length);
                    humanList.Add(GetPoolItem(type));
                    string strs = str[n];
                    string[] ss = strs.Split(',');
                    positon.x = float.Parse(ss[1]);
                    positon.y = float.Parse(ss[2]);
                    positon.z = 1f;
                    humanList[i].transform.position = positon;
                    humanList[i].GetComponent<Human>().SetSpeed(float.Parse(ss[3]), float.Parse(ss[4]));
                    n++;
                }
            }
        }
        else
            Debug.Log("END");
    }
    // Start is called before the first frame update
    void Start()
    {
        txt = Resources.Load("test") as TextAsset;
        str = txt.text.Split('\n');
        SetBackGround();
        MoveStart();
    }

    // Update is called once per frame
    void Update()
    {
        timer += Time.deltaTime;
        if (timer >= totalTime)
        {
            timer = 0;
            TimeStep();
        }
    }
}
