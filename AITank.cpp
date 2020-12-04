#include"AITank.h"

void AITankControl(Tank*aitank, vector<vector<Widget*>>& vw, promise<Shell*>& ps)
{
	/*
	1.尽量向基地方向运动
	2.开炮随机，向基地方向开炮的概率提升
	3.简单判断：前后左右四个方向是否可以前进
	*/
	int &steps = aitank->shtimes;
	int &fire = aitank->steptimes;
	fire++;
	static char cmd = 's';
	if (steps < 20 && cmd != 'j')
	{
		steps++;
		fire++;
	}
	else
	{
		steps++;
		if (fire > 20)
		{
			fire = 0;
			cmd = 'j';
			allTankCtrl(aitank, vw, ps, cmd);
			return;
		}
		int x_coo = aitank->giveCoordinate().first / XSIZE;
		int y_coo = aitank->giveCoordinate().second / YSIZE;
		int a, s = 6, d, w = 2;
		if (x_coo < 10)
		{
			a = 3; d = 5;
		}
		else 
		{
			a = 3; d = 5;
		}
		if (y_coo > 10)
			s = 4;
		srand(aitank->id * (unsigned int)time(NULL));
		srand((rand() % 100) * x_coo / (y_coo / 10 + 1));
		int ra = rand() % 14;
		int total = 0;
		if (ra < a + total)
		{
			cmd = 'a';
			allTankCtrl(aitank, vw, ps, cmd);
			return;
		}
		else total += a;
		if (ra < s + total)
		{
			cmd = 's';
			allTankCtrl(aitank, vw, ps, cmd);
			return;
		}
		else total += s;
		if (ra < d + total)
		{
			cmd = 'd';
			allTankCtrl(aitank, vw, ps, cmd);
			return;
		}
		else total += d;
		if (ra < w + total)
		{
			cmd = 'w';
			allTankCtrl(aitank, vw, ps, cmd);
			return;
		}
		else total += w;

	}

	allTankCtrl(aitank, vw, ps, cmd);
	return;
}