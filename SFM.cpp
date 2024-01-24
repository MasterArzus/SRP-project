#include"SFM.h"

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