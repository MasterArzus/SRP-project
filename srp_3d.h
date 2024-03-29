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


// 结构体
struct cordinate {
	double x;
	double y;
	int level;
	cordinate() = default;
	cordinate(double a, double b,int l) 
	{
		x = a;
		y = b;
		level = l;
	}
}; // 坐标结构体

struct AGENT
{
	int id;

	double m; // mass
	double x; // pos
	double y;
	int level = 0;
	double vx; // v
	double vy;
	double gx; // level goal pos
	double gy;
	double fgx; // final goal pos
	double fgy;
	int goal_level = 0;
	double next_gx; // next goal pos
	double next_gy;
	double v0; // max velocity
	double tao_1 = 0.1;

	double dis; // 起点到终点距离
	int jam_time = 0; // 连续多次速度过小时增加，达到设定次数时判定为卡住，重新寻路
	bool is_jamed = false;
	int np = 0; // no path flag
	int color = 0;
	double arrive_time = 0;

	bool arrived = 0;
	double arrive_range = 0;

	// queue
	QUEUE* Q;
	bool go_queue = 0;
	int order = 0;
	bool in_queue = 0;
	double process_time = 0;
	double cant_process_time = 0;


	// room
	ROOM* r;
	bool go_room = 0;
	bool in_room = 0;
	double room_time = 0;

	enum class state state;
	enum class state last_state;

	// density_check
	int side_density = 0;
	
	


	list<cordinate> path; // A* 路径存储
}; // agent 结构体

struct OBLINE
{
	double sx; // start point
	double sy;
	double ex; // end point
	double ey;
	double len; // length

}; // 障碍物结构体

struct node
{
	int x;
	int y;
	int g; // 移动代价
	int h; // 估算成本
	bool flag; // 这里用flag来表示node被推入close_list
	bool check;



	node* parent; // 父节点,从终点依次指向起点

	node() = default;

	node(int a, int b, double h = 0) {
		flag = 0;
		check = 0;
		x = a;
		y = b;
		h = h;
	}

	/*static bool compare(node& a, node& b)
	{
		return (a.g + a.h) > (b.g + b.h);
	}*/

};

struct dir
{
	int x;
	int y;
	dir(int a, int b)
	{
		x = a;
		y = b;
	}
};

struct STAIR
{
	int id;
	double up_x;
	double up_y;
	double down_x;
	double down_y;
	STAIR(int i, double ux, double uy, double dx, double dy)
	{
		id = i;
		up_x = ux;
		up_y = uy;
		down_x = dx;
		down_y = dy;
	}
};

struct QUEUE
{
	int id;
	int a_num = 0;
	double x;
	double y;
	int level;
	//bool symptom = 1;
	list<AGENT*> out_list;
	// list<AGENT*> in_list;
	vector<cordinate> path;

	int point_num = 0;

	int gap = 10; // 10 = 1m
	int q_len = 10;

	int q_time = 10;

	int up_down = 0; // -1 line goes up, 1 line goes down
	int left_right = 0; // -1 line goes left ,1 line goes right 

	QUEUE()
	{
		a_num = 0;
		path.push_back(cordinate(x, y, level));
	}
	QUEUE(int id1, double x1, double y1, int level1, int u,int l)
	{
		id = id1;
		x = x1;
		y = y1;
		level = level1;
		a_num = 0;
		path.push_back(cordinate(x, y, level));
		up_down = u;
		left_right = l;
	}

	void queue_back();

};

struct ROOM
{
	int id;
	double x;
	double y;
	int level = 0;

	int max_agent;
	int agent_num = 0;
	int time = 10;

	QUEUE* q;

	ROOM()
	{
		agent_num = 0;
	}

	ROOM(int i, double x1, double y1, int level1, int max, int time1, QUEUE* q1)
	{
		id = i;
		x = x1;
		y = y1;
		level = level1;
		max_agent = max;
		time = time1;
		q = q1;
		agent_num = 0;

	}

};


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



// 函数实现
double randval(double a, double b)
{
	return a + (b - a) * rand() / (double)RAND_MAX;
}


double agent_dis(AGENT* a1, AGENT* a2)
{
	return sqrt((a1->x - a2->x) * (a1->x - a2->x) + (a1->y - a2->y) * (a1->y - a2->y));
}


void agent_force(AGENT* a1, AGENT* a2, double* fx, double* fy, double dis) // a1<-a2
{
	// 感知距离判断放在函数调用之前了
	if (dis == 0)
	{
		dis = 1e-10;//防止出现dis=0
	}
	double dx = (a1->x - a2->x) / dis; // a2对a1施加的力的单位方向向量的x
	double dy = (a1->y - a2->y) / dis; // a2对a1施加的力的单位方向向量的y

	//对冲判断
	double cos1 = (a2->vx * dx + a2->vy * dy) / sqrt(dx * dx + dy * dy) / sqrt(a2->vx * a2->vx + a2->vy * a2->vy);  // a2速度和a2对a1作用力方向夹角cos
	double sin = (a2->vx * dy - a2->vy * dx) / sqrt(dx * dx + dy * dy) / sqrt(a2->vx * a2->vx + a2->vy * a2->vy); // a2速度和a2对a1作用力方向夹角sin
	double cos2 = (-a1->vx * dx - a1->vy * dy) / sqrt(dx * dx + dy * dy) / sqrt(a1->vx * a1->vx + a1->vy * a1->vy); // a1速度的反方向和a2对a1作用力方向夹角

	if (cos1 >= 0.866 && cos2 >= 0.866)//60`
	{
		if (sin >= 0)
		{
			dx = dx + dy;
			dy = dy - dx;

		}
		else
		{
			dx = dx - dy;
			dy = dy + dx;

		}
	}

	double rij = a1->m / density + a2->m / density; // 两个agent半径和
	double delta_d = rij - dis; // 半径和减去中心点距离
	double rif = A * exp(delta_d / B); // repulsive interaction force 斥力
	double bf = delta_d < 0 ? 0 : k1 * delta_d; // body force agents接触时存在的力
	// 斥力和body force的合力
	double res_f_x = (rif + bf) * dx;
	double res_f_y = (rif + bf) * dy;

	// 切向力 agents接触时存在
	double tfx = 0;
	double tfy = 0;
	if (delta_d > 0) // 身体半径有接触时
	{
		double tix = -dy;
		double tiy = dx;
		double delta_v = (a2->vx - a1->vx) * tix + (a2->vy - a1->vy) * tiy; // 切向速度差
		tfx = k2 * delta_d * delta_v * tix;
		tfy = k2 * delta_d * delta_v * tiy;
	}
	*fx += res_f_x + tfx;
	*fy += res_f_y + tfy;

	return;

}


double point_to_line_dis(double ax, double ay, double sx, double sy, double ex, double ey, double d, double* px, double* py)//p为垂足
{
	// 该函数需要计算垂足，用于障碍物对agent作用力方向的计算
	double dis = 0;
	double dot_pro = ((ax - sx) * (ex - sx) + (ay - sy) * (ey - sy)) / (d * d); // dot product of s_a and s_e divide by d2(s->start point of line;e->end point of line;a->agent)

	if (dot_pro <= 0) // 垂足在start point外侧
	{
		dis = sqrt((ax - sx) * (ax - sx) + (ay - sy) * (ay - sy));
		*px = sx;
		*py = sy;

	}
	else if (dot_pro < 1) // 垂足在线段内
	{
		*px = sx + (ex - sx) * dot_pro;
		*py = sy + (ey - sy) * dot_pro;
		dis = sqrt((ax - *px) * (ax - *px) + (ay - *py) * (ay - *py));

	}
	else if (dot_pro >= 1) // 垂足在end point外侧
	{
		dis = sqrt((ax - ex) * (ax - ex) + (ay - ey) * (ay - ey));
		*px = ex;
		*py = ey;

	}
	return dis;

}


void obline_force(AGENT* a, OBLINE* l, double* fx, double* fy)
{
	double px, py;
	double dis = point_to_line_dis(a->x, a->y, l->sx, l->sy, l->ex, l->ey, l->len, &px, &py);
	if (dis <= 0)
	{
		dis = 1e-10;//防止出现dis=0
	}
	if (dis > sense_range) // 感知距离判断在函数内
	{
		return;
	}
	double riw = a->m / density - dis; // 半径-距离
	double rif = A * exp(riw / B);
	double bf = riw < 0 ? 0 : k1 * riw;
	//if (bf != 0)cout << endl;
	// 点与线作用力点的方向向量
	double nx = (a->x - px) / dis;
	double ny = (a->y - py) / dis;
	// 斥力和body force合力
	double res_f_x = (rif + bf) * nx;
	double res_f_y = (rif + bf) * ny;
	// 切向力
	double tfx = 0;
	double tfy = 0;
	if (riw > 0)
	{
		double tix = -ny;
		double tiy = nx;
		double delta_v = a->vx * tix + a->vy * tiy;//切向速度差
		tfx = k2 * riw * delta_v * tix;
		tfy = k2 * riw * delta_v * tiy;
	}

	*fx += res_f_x - tfx;
	*fy += res_f_y - tfy;


	return;

}


//---------------------------------------------以上为SFM部分------------------------------------------------

//---------------------------------------------以下为A★部分------------------------------------------------

//函数声明
void A_star(AGENT*);
bool in_map(int, int, int);

// 数据
vector<vector<vector<node>>> map_matrix_A;

// 可调参数
const int a_step = 5;
const int detect_step = 3;
vector<dir> direction = { {a_step,0}, {-a_step,0}, {0,a_step}, {0,-a_step} }; // 正向
vector<dir> ob_direction = { {a_step,a_step},{a_step,-a_step},{-a_step,-a_step},{-a_step,a_step} }; // 斜向
vector<dir> wall_detect = { {detect_step,0}, {-detect_step,0}, {0,detect_step}, {0,-detect_step},{detect_step,detect_step},{detect_step,-detect_step},{-detect_step,-detect_step},{-detect_step,detect_step} };
const int path_len = 1;
const int max_time = 800; // ms


//函数实现
void A_star(AGENT* a)
{
	list<node*> open_list;

	//初始化close_list 和 check
	for (auto& y : map_matrix_A[a->level])
	{
		for (auto& x : y)
		{
			x.flag = 0;
			x.check = 0;
		}
	}

	// 初始化起点 目前社会力模型正确 应该是不会出现被挤出地图的情况 所以这部分可以注掉
	/*if (!in_map(int(a->x * map_factor), int(a->y * map_factor)))
	{
		a->path.clear();
		a->gx = a->x;
		a->gy = a->y;
		a->next_gx = a->x;
		a->next_gy = a->y;
		cout << "***********************************************no path" << endl;
		a->np = 1;
		return;
	}*/
	open_list.push_back(&map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)]);
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].x = int(a->x * map_factor);
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].y = int(a->y * map_factor);
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].flag = 1;
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].h = (abs(int(a->gx * map_factor) - int(a->x * map_factor)) + abs(int(a->gy * map_factor) - int(a->y * map_factor))) * 10;
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].g = 0;
	map_matrix_A[a->level][int(a->y * map_factor)][int(a->x * map_factor)].parent = nullptr;

	clock_t start, end; // 加个时间统计,用于判断无路径的情况
	start = clock();

	node* temp = open_list.front(); // 目前搜索的点


	while (!(abs(temp->x - a->gx * map_factor) <= a_step && abs(temp->y - a->gy * map_factor) <= a_step))
	{
		double f = INT_MAX;
		//找F最小的节点
		for (auto* a : open_list)
		{
			if ((a->g + a->h) < f)
			{
				f = (a->g + a->h);
				temp = a;
			}
		}

		bool flag = 0;
		// 判断周围是否有墙
		/*for (auto d : direction)
		{
			if (in_map(temp->x + d.x, temp->y + d.y,a->level) && map_matrix[a->level][temp->y + d.y][temp->x + d.x] == 0)
			{
				flag = 1;
				break;
			}
		}
		for (auto d : ob_direction)
		{
			if (in_map(temp->x + d.x, temp->y + d.y,a->level) && map_matrix[a->level][temp->y + d.y][temp->x + d.x] == 0)
			{
				flag = 1;
				break;
			}
		}*/

		// 正方向上搜索,此时移动代价为10
		for (auto d : direction)
		{

			bool wall = 0;
			if (in_map(temp->x + d.x, temp->y + d.y,a->level))
			{
				for (double i = 1; i <= a_step; ++i)
				{
					// cout << d.y * (i / a_step) <<" " << d.x * (i / a_step) << endl;
					if (map_matrix[a->level][temp->y + d.y * (i / a_step)][temp->x + d.x * (i / a_step)] == 0)
					{
						wall = 1;
						flag = 1;
						break;
					}
					
				}
				if (wall)
				{
					continue;
				}
				for (auto dd : wall_detect)
				{
					if (in_map(temp->x + d.x + dd.x, temp->y + d.y + dd.y, a->level))
					{
						if (map_matrix[a->level][temp->y + d.y + dd.y][temp->x + d.x + dd.x] == 0)
						{
							wall = 1;
						}
					}
				}
				if (wall)
				{
					continue;
				}
				int point_type = map_matrix[a->level][temp->y + d.y][temp->x + d.x];
				//if (density_map[temp->y + d.y][temp->x + d.x])point_type == 0;
				//cout << density_map[temp->y + d.y][temp->x + d.x] << endl;
				if (point_type != 0 && map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].check == 0)//节点在地图内，且不在障碍物部分,且未check过
				{
					if (map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].flag == 0)//表示该点第一次进入open_list
					{
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].x = temp->x + d.x;
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].y = temp->y + d.y;
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].h = (abs(int(a->gx * map_factor) - (temp->x + d.x)) + abs(int(a->gy * map_factor) - (temp->y + d.y))) * 10;
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g = temp->g + 10 * a_step * (point_type * 0.75 + density_map[a->level][temp->y + d.y][temp->x + d.x]);
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].parent = temp;
						map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].flag = 1;
					}
					else // 再次筛查一个点d,只有在通过(这次搜索的点temp到达d的代价)比(d的父节点到d)更小时,才会更新
					{
						if (temp->g + 10 * a_step < map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g)
						{
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g = temp->g + 10 * a_step * (point_type * 0.75 + density_map[a->level][temp->y + d.y][temp->x + d.x]);
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].parent = temp;
						}
					}
					open_list.push_back(&map_matrix_A[a->level][temp->y + d.y][temp->x + d.x]);
				}
			}

		}

		// 斜方向上搜索,此时移动代价为14,实际上应该是10*2^(1/2),小于这个值会导致结果更偏向斜向移动
		if (!flag)
		{
			for (auto d : ob_direction)
			{
				if (in_map(temp->x + d.x, temp->y + d.y,a->level))
				{
					bool wall = 0;
					for (auto dd : wall_detect)
					{
						if (in_map(temp->x + d.x + dd.x, temp->y + d.y + dd.y, a->level))
						{
							if (map_matrix[a->level][temp->y + d.y + dd.y][temp->x + d.x + dd.x] == 0)
							{
								wall = 1;
							}
						}
					}
					if (wall)
					{
						continue;
					}
					int point_type = map_matrix[a->level][temp->y + d.y][temp->x + d.x];
					//if (density_map[temp->y + d.y][temp->x + d.x])point_type == 0;
					if (point_type != 0 && map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].check == 0)//节点在地图内，且不在障碍物部分,且未check过 && map_matrix_A[temp->y + d.y][temp->x + d.x].check == 0
					{
						if (map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].flag == 0) // 表示该点第一次进入open_list
						{
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].x = temp->x + d.x;
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].y = temp->y + d.y;
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].h = (abs(int(a->gx * map_factor) - (temp->x + d.x)) + abs(int(a->gy * map_factor) - (temp->y + d.y))) * 10;
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g = temp->g + 15 * a_step * (point_type * 0.75 + density_map[a->level][temp->y + d.y][temp->x + d.x]);
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].parent = temp;
							map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].flag = 1;
						}
						else // 再次筛查一个点d,只有在通过(这次搜索的点temp到达d的代价)比(d的父节点到d)更小时,才会更新
						{
							if (temp->g + 15 * a_step < map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g)
							{
								map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].g = temp->g + 15 * a_step * (point_type * 0.75 + density_map[a->level][temp->y + d.y][temp->x + d.x]);
								map_matrix_A[a->level][temp->y + d.y][temp->x + d.x].parent = temp;
							}
						}
						open_list.push_back(&map_matrix_A[a->level][temp->y + d.y][temp->x + d.x]);
					}
				}
			}
		}

		open_list.remove(temp);
		temp->check = 1; // 已经检查过这个点

		// 无路径判断,绝大部分在n s之内可以搜到,超过视为不存在路径
		end = clock();
		if (end - start > max_time)
		{
			a->path.clear();
			a->gx = a->x;
			a->gy = a->y;
			a->next_gx = a->x;
			a->next_gy = a->y;
			a->np = 1;
			return;
		}

	}
	// push 路径到 path,这里可以选择路径密度
	int counter = 0;
	do
	{
		counter++;
		if (counter == path_len) // 每n个取一个点
		{
			a->path.push_front(cordinate(temp->x, temp->y,a->level));
			counter = 0;
		}
		else if (map_matrix[a->level][temp->y][temp->x] == 2) // 门口重要节点也要push
		{
			a->path.push_front(cordinate(temp->x, temp->y,a->level));
		}
		if (temp->parent == nullptr)break;
		temp = temp->parent;

	} while (temp != nullptr);
	if (!a->path.empty())
	{
		a->next_gx = a->path.front().x / map_factor;
		a->next_gy = a->path.front().y / map_factor;
	}
	else
	{
		a->next_gx = a->gx;
		a->next_gy = a->gy;
	}

}


bool in_map(int x, int y,int level)
{
	return (x >= 0 && x < col_num[level] - 1) && (y >= 0 && y < row_num[level] - 1);
}


//---------------------------------------------以上为A★部分------------------------------------------------

//---------------------------------------------以下为3D部分-------------------------------------------------


// 函数声明
void update_g(AGENT*);


// 函数实现

// 到达楼层目标后判断
void update_g(AGENT* a)
{	// 目前的情况
	
	if (a->level == a->goal_level)
	{   // 已经到同层了就搜索
		a->gx = a->fgx;
		a->gy = a->fgy;
		a->path.clear();
		A_star(a);
	}
	else if(a->level < a->goal_level)
	{
		// 更新gx到这一层的上楼点,目前只写楼梯,只写一个楼梯
		//cout << "here" << endl;
		a->gx = s1.up_x;
		a->gy = s1.up_y;
		a->path.clear();
		A_star(a);
		
	}
	else if (a->level > a->goal_level)
	{
		// 更新gx到这一层的下楼点
		a->gx = s1.down_x;
		a->gy = s1.down_y;
		a->path.clear();
		A_star(a);
		
	}

}

// 走楼梯
void use_stair(AGENT* a)
{


}

// 走电梯
void use_lift(AGENT* a)
{

}

//---------------------------------------------以上为3D部分-------------------------------------------------

//---------------------------------------------以下为排队部分-------------------------------------------------


// 终点
// 是否能够留接口？
// 在地图标识不同颜色设定接口
struct cordinate goal[6] = { {80,57.5,1} , { 54,18,0 }, { 13.5,55.5,0 }, { 50,41,1 }, { 80,18.4,2 }, {50,34.7,2} };




void QUEUE::queue_back()
{
	if (a_num < q_len)
	{
		path.push_back(cordinate(x + left_right * (a_num / map_factor) * gap, y + up_down* (a_num / map_factor) * gap, level));
	}
	else
	{
		path.push_back(cordinate(x + left_right * (q_len / map_factor) * gap + left_right * (a_num - q_len) / map_factor * 0.01, y + up_down * (q_len / map_factor) * gap + up_down * (a_num - q_len) / map_factor * 0.01, level));
	}

}

// 队列创建 在init_map中
// 53.4 71.5





// 函数声明
void go_queue(AGENT*,QUEUE*);
void in_queue(AGENT*);
void out_queue(AGENT*);
void cant_process(AGENT*);
void go_room(AGENT*, ROOM*);

// 参数


// 函数实现
// agent选择一个队列加入
void go_queue(AGENT* a,QUEUE* q)
{
	a->go_queue = true;
	a->order = 0;
	a->Q = q;
	a->fgx = a->Q->x;
	a->fgy = a->Q->y;
	a->goal_level = a->Q->level;
	q->out_list.push_back(a);
}

// 当go_agent状态的agent来到队列附近判定为arrive时，开始进入排队进程
void in_queue(AGENT* a)
{
	a->arrived = false;
	a->go_queue = false;
	a->in_queue = true;
	a->Q->a_num += 1;
	a->order = a->Q->a_num;
	a->path.clear();

	if (a->order > a->Q->point_num)
	{
		//cout << "in" << endl;
		//cout << a->Q->path.size()<<endl;

		a->Q->point_num = a->order;
		a->Q->queue_back();
		//cout << a->Q->path.size() << endl;
	}
	//cout << "front" << endl;
	//cout << a->order << endl;
	//cout << a->Q->path.size() << endl;
	a->next_gx = a->Q->path[a->order].x;
	a->next_gy = a->Q->path[a->order].y;
	//cout << "rear" << endl;

}

// 挂号完毕，离开队列，去自己的目标
void out_queue(AGENT* a)
{
	
	a->process_time = 0;
	a->Q->a_num -= 1;
	for (auto& a_q : a->Q->out_list)
	{
		if (a_q->order && a_q->in_queue)
		{
			a_q->order -= 1;
			a_q->next_gx = a->Q->path[a_q->order].x;
			a_q->next_gy = a->Q->path[a_q->order].y;
		}
	}
	a->in_queue = false;
	a->Q->out_list.remove(a);
}

void cant_process(AGENT* a)
{
	
	a->cant_process_time = 0;
	a->process_time = 0;
	a->Q->a_num -= 1;
	for (auto& a_q : a->Q->out_list)
	{
		if (a_q->order && a_q->in_queue)
		{
			a_q->order -= 1;
			a_q->next_gx = a->Q->path[a_q->order].x;
			a_q->next_gy = a->Q->path[a_q->order].y;
		}
	}
	a->in_queue = false;
	in_queue(a);

}



//---------------------------------------------以上为排队部分-------------------------------------------------

//---------------------------------------------以下为ROOM部分-------------------------------------------------



// 函数声明

void in_room(AGENT*);
void out_room(AGENT*);


// 函数实现
void go_room(AGENT* a, ROOM* r)
{
	a->r = r;
	go_queue(a, r->q);
	a->go_room = true;
}


void in_room(AGENT* a)
{	
	if (a->r->agent_num < a->r->max_agent)
	{
		a->r->agent_num += 1;
		a->Q->a_num -= 1;
		for (auto& a_q : a->Q->out_list)
		{
			if (a_q->order && a_q->in_queue)
			{
				a_q->order -= 1;
				a_q->next_gx = a->Q->path[a_q->order].x;
				a_q->next_gy = a->Q->path[a_q->order].y;
			}
		}
		a->in_queue = false;
		a->go_room = false;
		a->in_room = true;
		a->Q->out_list.remove(a);
		a->fgx = a->r->x;
		a->fgy = a->r->y;
		a->goal_level = a->r->level;
		update_g(a);
	}
	
}


void out_room(AGENT* a)
{
	a->room_time = 0;
	a->in_room = false;
	a->r->agent_num -= 1;
}

//a->fgx = 8;
//a->fgy = 62.3;
//a->goal_level = 0;

//---------------------------------------------以上为ROOM部分-------------------------------------------------

//---------------------------------------------以下为RPD部分-------------------------------------------------

enum class state
{
	reg,// 挂号
	doc, // 看医生
	med, // 取药
	ct, // 查ct
	wc, // 上厕所
	leave, // 离开

};

// 函数声明
void do_sth(AGENT*);

void go_reg(AGENT*);
void go_doc(AGENT*);
void go_med(AGENT*);
void go_ct(AGENT*);
void go_wc(AGENT*);
void go_leave(AGENT*);


// 函数实现
void do_sth(AGENT* a)
{
	
	if (a->state == state::reg)
	{
		go_reg(a);
	}
	else if (a->state == state::doc)
	{
		go_doc(a);
	}
	else if (a->state == state::med)
	{
		go_med(a);
	}
	else if (a->state == state::ct)
	{
		go_ct(a);
	}
	else if (a->state == state::wc)
	{
		go_wc(a);
	}
	else if (a->state == state::leave)
	{
		go_leave(a);
	}
	

}

void go_reg(AGENT* a)
{
	int min = INT_MAX;
	QUEUE* least_q = NULL;
	for (auto& q : q_list)
	{
		if (q->out_list.size() < min)
		{
			least_q = q;
			min = q->out_list.size();
		}
	}
	go_queue(a, least_q);
	a->arrived = false;
	
	double rand_s = randval(0, 2.5);
	if (rand_s < 1)
	{
		a->state = state::doc;
	}
	else if (rand_s <2)
	{
		a->state = state::med;
	}
	else if (rand_s < 2.5)
	{
		a->last_state = state::reg;
		a->state = state::wc;
	}
	
}

void go_doc(AGENT* a)
{
	int rand_r = 0;
	do
	{
		rand_r = int(randval(0, 24));
	} while (rand_r == 8 || rand_r == 17 || rand_r == 0 || rand_r == 5 || rand_r == 6);
	go_room(a, r_list[rand_r]);
	
	if (a->last_state != state::ct)
	{
		double rand_s = randval(0, 3.5);
		if (rand_s < 1)
		{
			a->state = state::ct;
		}
		else if (rand_s < 2)
		{
			a->state = state::med;
		}
		else if (rand_s < 3)
		{
			a->state = state::leave;
		}
		else if (rand_s < 3.35)
		{
			a->last_state = state::doc;
			a->state = state::wc;
		}
	}
	else
	{
		double rand_s = randval(0, 2.5);
		if (rand_s < 1)
		{
			a->state = state::med;
		}
		else if (rand_s < 2)
		{
			a->state = state::leave;
		}
		else if (rand_s < 2.5)
		{
			a->last_state = state::doc;
			a->state = state::wc;
		}
	}
	

}

void go_med(AGENT* a)
{
	int min = INT_MAX;
	QUEUE* least_q = NULL;
	for (auto& q : med_q_list)
	{
		if (q->out_list.size() < min)
		{
			least_q = q;
			min = q->out_list.size();
		}
	}
	go_queue(a, least_q);
	a->state = state::leave;
}

void go_ct(AGENT* a)
{
	int rand_r = int(randval(5, 7));
	go_room(a, r_list[rand_r]);
	a->last_state = state::ct;
	a->state = state::doc;
}

void go_wc(AGENT* a)
{
	int rand_r = int(randval(0,3));
	if (rand_r == 0)
	{
		go_room(a, r_list[0]);
	}
	else if (rand_r == 1)
	{
		go_room(a, r_list[8]);
	}
	else if (rand_r == 2)
	{
		go_room(a, r_list[17]);
	}
	a->state = a->last_state;


}

void go_leave(AGENT* a)
{
	a->fgx = 8;
	a->fgy = 62.3;
	a->goal_level = 0;
	a->arrived = false;
}


//---------------------------------------------以下为异常行为(密度)检测部分-------------------------------------------------

//检测人群密度(密度过大)


void calculateDensity(std::vector<AGENT*>& agent_list, double threshold) {
	for (AGENT* agent : agent_list) {
		int count = 0;
		for (AGENT* otherAgent : agent_list) {
			if (agent == otherAgent) {
				continue;
			}
			double distance = std::sqrt(std::pow(agent->x - otherAgent->x, 2) + std::pow(agent->y - otherAgent->y, 2));
			if (distance < threshold) {
				count++;
			}
		}
		agent->side_density = count;
	}
}


void detectDensity(int threshold_x, int threshold_y) {
	// check density

	for (int i = 0; i < agent_list.size(); ++i)
	{
		if (agent_list[i]->arrived)continue;
		if (agent_list[i]->vx <= 0.1 && agent_list[i]->vy <= 0.1)// 6
		{

			for (int j = -5; j <= 5; ++j)
			{
				for (int k = -5; k <= 5; ++k)
				{

					if (in_map(int(agent_list[i]->x * map_factor + j), int(agent_list[i]->y * map_factor + k), agent_list[i]->level))
					{
						// density_map[agent_list[i]->level][int(agent_list[i]->y * map_factor + k)][int(agent_list[i]->x * map_factor + j)] += 5*(abs(5-j) + abs(5-k));//***
						// 统一加40个密度点
						density_map[agent_list[i]->level][int(agent_list[i]->y * map_factor + k)][int(agent_list[i]->x * map_factor + j)] += 40;
					}
				}
			}
		}


	}
}



//检测人群行为(如静止不动)
void calculateJam(std::vector<AGENT*>& agent_list, double threshold) {
	for (AGENT* agent : agent_list) {
		int count = 0;
		if (agent->jam_time > 40)
		{
			agent->is_jamed = true;
		}
		else
		{
			agent->is_jamed = false;
		}
	}
}

