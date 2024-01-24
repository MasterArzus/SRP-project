#pragma once
#include"basic_definition.h"

// 定义楼梯的位置
STAIR s1(0, 82, 7.6, 82, 11);


// 定义list？
vector<QUEUE*> q_list = {};
vector<ROOM*> r_list = {};
vector<QUEUE*> med_q_list = {};


// 函数声明
double randval(double, double);
double agent_dis(AGENT, AGENT); // agents 之间距离
void agent_force(AGENT*, AGENT*, double*, double*, double); // agents 之间作用力
double point_to_line_dis(double, double, double, double, double, double, double, double*, double*); // 计算点到线距离
void obline_force(AGENT*, OBLINE*, double*, double*); // 计算障碍物作用力
