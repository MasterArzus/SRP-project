using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HumanManager_UI: MonoBehaviour
{
    int n = 0;
    float timer = 0;
    float totalTime = 0.02f;
    int number = 0;
    TextAsset txt;
    string[] str;
    Vector3 positon = new Vector3(0f,0f,0f);
    GameObject parent;
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
            instance.transform.parent = null;
            instance.transform.parent = parent.transform;
            instance.transform.SetParent(parent.transform, true);
            string[] ss = strs.Split(',');
            positon.x = float.Parse(ss[1]);
            positon.y = float.Parse(ss[2]);
            positon.z = 1f;
            instance.transform.localPosition = positon;
            humanList.Add(instance);
            n++;
            strs = new string(str[n]);
       }
        Debug.Log(n);
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
                    humanList[i].transform.localPosition = positon;
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
        parent = GameObject.Find("HumanMove");
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
