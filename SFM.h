#pragma once
#include"basic_definition.h"

// ����¥�ݵ�λ��
STAIR s1(0, 82, 7.6, 82, 11);


// ����list��
vector<QUEUE*> q_list = {};
vector<ROOM*> r_list = {};
vector<QUEUE*> med_q_list = {};


// ��������
double randval(double, double);
double agent_dis(AGENT, AGENT); // agents ֮�����
void agent_force(AGENT*, AGENT*, double*, double*, double); // agents ֮��������
double point_to_line_dis(double, double, double, double, double, double, double, double*, double*); // ����㵽�߾���
void obline_force(AGENT*, OBLINE*, double*, double*); // �����ϰ���������
