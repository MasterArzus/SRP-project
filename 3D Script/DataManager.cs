using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEngine;

public class DataManager : MonoBehaviour
{
    TextAsset txt;
    string[] lineStr;
    ulong curr_line = 0;
    float m_timer = 0;
    float frameTime = 0.02f;
    float m_scale_pos;
    float m_scale_speed;
    int maxMenCnt;//最大人数

    Vector3 position=new Vector3(0f,0f,0f);
    GameObject[] humanList;

    [SerializeField] GameObject prefab;

    /// <summary>
    /// 获取相应的参数
    /// </summary>
    void GetParam()
    {
        string temp = lineStr[curr_line];
        string[] m_params = temp.Split(',');

        // 调整帧数
        //frameTime = 5/float.Parse(m_params[0]);
        // adjust the scale
        m_scale_pos = float.Parse(m_params[1]) / 2;
        m_scale_speed = float.Parse(m_params[1]) / 4;
        //m_scale_speed = float.Parse(m_params[1]);
        maxMenCnt = int.Parse(m_params[2]);
        curr_line++;
    }

    /// <summary>
    /// 可能会有拓展，故保留
    /// </summary>
    void CreatHuman()
    {
        humanList = new GameObject[maxMenCnt];
        //ulong frameNum = ulong.Parse(lineStr[curr_line++]);
        //string m_str = lineStr[curr_line];
        //while (curr_line <= frameNum + 1)
        //{
        //    GameObject temp=Instantiate(prefab);
        //    string[] ss = m_str.Split(',');

        //    //坐标系应该是这样对应的，可能要调整
        //    position.x=float.Parse(ss[1]) * m_scale_pos;
        //    position.z=float.Parse(ss[2]) * m_scale_pos;

        //    temp.transform.position=position;
        //    humanList.Add(temp);
        //    curr_line++;
        //    m_str = lineStr[curr_line];
        //}
    }

    void TimeStep()
    {
        
        if (curr_line >= (ulong)lineStr.Length)
        {
            return;
        }
        int lineCnt = int.Parse(lineStr[curr_line++]);
        //Debug.Log(lineCnt);
        if (curr_line < (ulong)lineStr.Length)
        {
            //for(int i=0; i<humanList.Count; i++)
            //{
            //    string m_str=lineStr[curr_line];
            //    string[] ss = m_str.Split(',');
                
            //    //change the position directly
            //    position.x=float.Parse(ss[0]) * m_scale_pos;
            //    position.z=float.Parse(ss[1]) * m_scale_pos;
            //    //humanList[i].GetComponent<Robot>().transform.position = position;
            //    //humanList[i].GetComponent<Robot>().fixSpeed(float.Parse(ss[0]) * m_scale_pos, float.Parse(ss[1]) * m_scale_pos, frameTime);

            //    //设置速度
            //    humanList[i].GetComponent<Robot>().SetVelocity(float.Parse(ss[2]) * m_scale_speed, float.Parse(ss[3]) * m_scale_speed);

            //    // move
            //    humanList[i].GetComponent<Robot>().Move();

            //    //next
            //    curr_line++;
            //}
            for(int i = 0; i < maxMenCnt; ++i)
            {
                if (humanList[i] != null)
                {
                    humanList[i].GetComponent<Robot>().SetVelocity(0f,0f);
                }
            }
            for(int i = 0; i < lineCnt; ++i)
            {
                string m_str = lineStr[curr_line++];
                string[] ss = m_str.Split(',');
                int id = int.Parse(ss[0]);

                //get position
                position.x = float.Parse(ss[1]) * m_scale_pos;
                position.z = float.Parse(ss[2]) * m_scale_pos;

                if (int.Parse(ss[3]) == 0)
                {
                    position.y = 0;
                }
                else if (int.Parse(ss[3]) == 1)
                {
                    position.y = 42;
                }
                else if (int.Parse(ss[3]) == 2)
                {
                    position.y = 82;
                }

                if (humanList[id] == null)
                {
                    GameObject temp = Instantiate(prefab);
                    temp.transform.position = position;
                    humanList[id] = temp;
                    
                }

                humanList[id].GetComponent<Robot>().SetVelocity(float.Parse(ss[4]) * m_scale_speed, float.Parse(ss[5]) * m_scale_speed);
                humanList[id].GetComponent<Robot>().mPosition = position;

                humanList[id].GetComponent<Robot>().floor = int.Parse(ss[3]);

                //for test
                //humanList[id].GetComponent<Robot>().transform.position = position;
            }

        }
    }

    // Start is called before the first frame update
    void Start()
    {
        StreamReader sr = new StreamReader(Application.streamingAssetsPath + "/test3.txt");
        //txt = Resources.Load("test2") as TextAsset;
        //lineStr = txt.text.Split('\n');
        lineStr = sr.ReadToEnd().Split('\n');
        GetParam();
        CreatHuman();
    }

    // Update is called once per frame
    void Update()
    {
        m_timer+=Time.deltaTime;
        if (m_timer >= frameTime)
        {
            m_timer=0;
            TimeStep();
        }
    }
}
