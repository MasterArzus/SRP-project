#include<math.h>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include<vector>
#include<iostream>
#include<list>
#include<queue>

using namespace std;

// 数量限制
#define MAX_AGENT_NUM 1000 // 最大agent数量，因为使用的是vector所以实际上没用到
#define MAX_OBLINE_NUM 1000 // 最大障碍物数量

// 建模参数
#define MAX_V 1.42 // agent 最大速度
#define sense_range 5 // 感知范围：m
#define density 320 // 质量/半径，m/density = r 320
#define tao 0.5 // 目前方向转移到目标方向的加速度参数

#define total_level 3


// sfm参数
#define k1 120000 // body force parameter
#define k2 240000 // tangential force parameter 切向力参数s
#define A 5000 // N// A * exp[dis/B] only social force  2000
#define B 0.08 // m

//行列数
int col_num[total_level] = {};
int row_num[total_level] = {};//init in init_map

// 点阵图放大比
double  map_factor = 10;

// 程序数据
vector<vector<vector<int>>> map_matrix; // 点阵图
vector<vector<vector<int>>> density_map; // 人群密度图,给A*考虑选取人少的路径


enum class state;


// 结构体声明
struct cordinate;
struct AGENT;
struct OBLINE;
struct node;
struct dir;
struct STAIR;
struct QUEUE;
struct ROOM;
