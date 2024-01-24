#include<math.h>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include<vector>
#include<iostream>
#include<list>
#include<queue>

using namespace std;

// ��������
#define MAX_AGENT_NUM 1000 // ���agent��������Ϊʹ�õ���vector����ʵ����û�õ�
#define MAX_OBLINE_NUM 1000 // ����ϰ�������

// ��ģ����
#define MAX_V 1.42 // agent ����ٶ�
#define sense_range 5 // ��֪��Χ��m
#define density 320 // ����/�뾶��m/density = r 320
#define tao 0.5 // Ŀǰ����ת�Ƶ�Ŀ�귽��ļ��ٶȲ���

#define total_level 3


// sfm����
#define k1 120000 // body force parameter
#define k2 240000 // tangential force parameter ����������s
#define A 5000 // N// A * exp[dis/B] only social force  2000
#define B 0.08 // m

//������
int col_num[total_level] = {};
int row_num[total_level] = {};//init in init_map

// ����ͼ�Ŵ��
double  map_factor = 10;

// ��������
vector<vector<vector<int>>> map_matrix; // ����ͼ
vector<vector<vector<int>>> density_map; // ��Ⱥ�ܶ�ͼ,��A*����ѡȡ���ٵ�·��


enum class state;


// �ṹ������
struct cordinate;
struct AGENT;
struct OBLINE;
struct node;
struct dir;
struct STAIR;
struct QUEUE;
struct ROOM;
