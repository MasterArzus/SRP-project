#include"basic_definition.h"

// 结构体
struct cordinate {
	double x;
	double y;
	int level;
	cordinate() = default;
	cordinate(double a, double b, int l)
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
	QUEUE(int id1, double x1, double y1, int level1, int u, int l)
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