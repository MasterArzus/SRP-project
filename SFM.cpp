#include"SFM.h"

// ����ʵ��
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
	// ��֪�����жϷ��ں�������֮ǰ��
	if (dis == 0)
	{
		dis = 1e-10;//��ֹ����dis=0
	}
	double dx = (a1->x - a2->x) / dis; // a2��a1ʩ�ӵ����ĵ�λ����������x
	double dy = (a1->y - a2->y) / dis; // a2��a1ʩ�ӵ����ĵ�λ����������y

	//�Գ��ж�
	double cos1 = (a2->vx * dx + a2->vy * dy) / sqrt(dx * dx + dy * dy) / sqrt(a2->vx * a2->vx + a2->vy * a2->vy);  // a2�ٶȺ�a2��a1����������н�cos
	double sin = (a2->vx * dy - a2->vy * dx) / sqrt(dx * dx + dy * dy) / sqrt(a2->vx * a2->vx + a2->vy * a2->vy); // a2�ٶȺ�a2��a1����������н�sin
	double cos2 = (-a1->vx * dx - a1->vy * dy) / sqrt(dx * dx + dy * dy) / sqrt(a1->vx * a1->vx + a1->vy * a1->vy); // a1�ٶȵķ������a2��a1����������н�

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

	double rij = a1->m / density + a2->m / density; // ����agent�뾶��
	double delta_d = rij - dis; // �뾶�ͼ�ȥ���ĵ����
	double rif = A * exp(delta_d / B); // repulsive interaction force ����
	double bf = delta_d < 0 ? 0 : k1 * delta_d; // body force agents�Ӵ�ʱ���ڵ���
	// ������body force�ĺ���
	double res_f_x = (rif + bf) * dx;
	double res_f_y = (rif + bf) * dy;

	// ������ agents�Ӵ�ʱ����
	double tfx = 0;
	double tfy = 0;
	if (delta_d > 0) // ����뾶�нӴ�ʱ
	{
		double tix = -dy;
		double tiy = dx;
		double delta_v = (a2->vx - a1->vx) * tix + (a2->vy - a1->vy) * tiy; // �����ٶȲ�
		tfx = k2 * delta_d * delta_v * tix;
		tfy = k2 * delta_d * delta_v * tiy;
	}
	*fx += res_f_x + tfx;
	*fy += res_f_y + tfy;

	return;

}


double point_to_line_dis(double ax, double ay, double sx, double sy, double ex, double ey, double d, double* px, double* py)//pΪ����
{
	// �ú�����Ҫ���㴹�㣬�����ϰ����agent����������ļ���
	double dis = 0;
	double dot_pro = ((ax - sx) * (ex - sx) + (ay - sy) * (ey - sy)) / (d * d); // dot product of s_a and s_e divide by d2(s->start point of line;e->end point of line;a->agent)

	if (dot_pro <= 0) // ������start point���
	{
		dis = sqrt((ax - sx) * (ax - sx) + (ay - sy) * (ay - sy));
		*px = sx;
		*py = sy;

	}
	else if (dot_pro < 1) // �������߶���
	{
		*px = sx + (ex - sx) * dot_pro;
		*py = sy + (ey - sy) * dot_pro;
		dis = sqrt((ax - *px) * (ax - *px) + (ay - *py) * (ay - *py));

	}
	else if (dot_pro >= 1) // ������end point���
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
		dis = 1e-10;//��ֹ����dis=0
	}
	if (dis > sense_range) // ��֪�����ж��ں�����
	{
		return;
	}
	double riw = a->m / density - dis; // �뾶-����
	double rif = A * exp(riw / B);
	double bf = riw < 0 ? 0 : k1 * riw;
	//if (bf != 0)cout << endl;
	// ��������������ķ�������
	double nx = (a->x - px) / dis;
	double ny = (a->y - py) / dis;
	// ������body force����
	double res_f_x = (rif + bf) * nx;
	double res_f_y = (rif + bf) * ny;
	// ������
	double tfx = 0;
	double tfy = 0;
	if (riw > 0)
	{
		double tix = -ny;
		double tiy = nx;
		double delta_v = a->vx * tix + a->vy * tiy;//�����ٶȲ�
		tfx = k2 * riw * delta_v * tix;
		tfy = k2 * riw * delta_v * tiy;
	}

	*fx += res_f_x - tfx;
	*fy += res_f_y - tfy;


	return;

}