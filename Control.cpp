#include"Control.h"
void allTankCtrl(Tank* mytank, vector<vector<Widget*>>& vw, promise<Shell*>& ps, char ifai)
{
	static Widget* tmp = NULL;
	if (tmp != NULL)
		tmp->drawWidget();
	
	char c;
	//若不为0，为AI操作
	if (0 != ifai)//AI通用函数
		c = ifai;
	else if (_kbhit())
		c = _getch();
	else {
		ps.set_value(NULL);
		this_thread::sleep_for(chrono::milliseconds(TANKSLEEPINGTIME));
		return;
	}
	if (mytank == NULL)
	{
		ps.set_value(NULL);
		this_thread::sleep_for(chrono::milliseconds(TANKSLEEPINGTIME));
		return;
	}

	//获取坐标
	pair<int, int> coo = mytank->giveCoordinate();
	int cur_x = coo.first / XSIZE;
	int cur_y = coo.second / YSIZE;
	int xflag = 0;
	int yflag = 0;
	if (coo.first % XSIZE == 0)
		xflag = 1;
	if (coo.second % YSIZE == 0)
		yflag = 1;

	switch (c)
	{
	case 'a':
	case 'A':
		mytank->changeDir(-1, 0);
		//不是整坐标：可直接移动无需判断
		if (0 == xflag)
			mytank->Move(-1, 0);
		else
		{
			//是否在地图边界
			if (cur_x - 1 < 0)
				break;
			//正前方是否可穿过
			if (NULL == vw[cur_x - 1][cur_y] || vw[cur_x - 1][cur_y]->penetrable)
			{
				//侧前方是否可穿过
				if (1 == yflag || (NULL == vw[cur_x - 1][cur_y + 1] || vw[cur_x - 1][cur_y + 1]->penetrable))
				{
					mytank->Move(-1, 0);
					//切换坐标条件：
					Widget* ww = vw[cur_x - 1][cur_y];
					vw[cur_x][cur_y] = tmp;
					vw[cur_x - 1][cur_y] = mytank;
					tmp = ww;
				}
			}
		}
		break;
	case 's':
	case 'S':
		mytank->changeDir(0, 1);
		if (0 == yflag)
		{
			mytank->Move(0, 1);
			//移动之后坐标为整点，移动坦克坐标点
			if (mytank->giveCoordinate().second % YSIZE == 0)
			{
				Widget* ww = vw[cur_x][cur_y + 1];
				vw[cur_x][cur_y] = tmp;
				vw[cur_x][cur_y + 1] = mytank;
				tmp = ww;
			}
		}
		else
		{
			//是否在地图边界
			if (cur_y + 1 > 15)
				break;
			//正前方是否可以穿过
			if (NULL == vw[cur_x][cur_y + 1] || vw[cur_x][cur_y + 1]->penetrable)
			{
				if (1 == xflag || (NULL == vw[cur_x + 1][cur_y + 1] || vw[cur_x + 1][cur_y + 1]->penetrable))
				{
					mytank->Move(0, 1);
				}
			}

		}
		break;
	case 'd':
	case 'D':
		mytank->changeDir(1, 0);
		//不是整坐标：可直接移动无需判断
		if (0 == xflag)
		{
			mytank->Move(1, 0);
			//移动之后坐标为整点，移动坦克坐标点
			if (mytank->giveCoordinate().first % XSIZE == 0)
			{
				Widget *ww = vw[cur_x + 1][cur_y];
				vw[cur_x][cur_y] = tmp;
				vw[cur_x + 1][cur_y] = mytank;
				tmp = ww;
			}
		}
		else
		{
			//是否在地图边界
			if (cur_x + 1 > 19)
				break;
			//正前方是否可穿过
			if (NULL == vw[cur_x + 1][cur_y] || vw[cur_x + 1][cur_y]->penetrable)
			{
				//侧前方是否可穿过
				if (1 == yflag || (NULL == vw[cur_x + 1][cur_y + 1] || vw[cur_x + 1][cur_y + 1]->penetrable))
				{
					mytank->Move(1, 0);

				}
			}
		}
		break;
	case 'w':
	case 'W':
		mytank->changeDir(0, -1);
		if (0 == yflag)
		{
			mytank->Move(0, -1);
		}
		else
		{
			//是否在地图边界
			if (cur_y - 1 < 0)
				break;
			//正前方是否可以穿过
			if (NULL == vw[cur_x][cur_y - 1] || vw[cur_x][cur_y - 1]->penetrable)
			{
				if (1 == xflag || (NULL == vw[cur_x + 1][cur_y - 1] || vw[cur_x + 1][cur_y - 1]->penetrable))
				{
					mytank->Move(0, -1);
					//移动之后坐标为整点，移动坦克坐标点
					if (mytank->giveCoordinate().second % YSIZE != cur_y)
					{
						Widget* ww = vw[cur_x][cur_y - 1];
						vw[cur_x][cur_y] = tmp;
						vw[cur_x][cur_y - 1] = mytank;
						tmp = ww;
					}
				}
			}

		}
		break;
	case 'j':
	case 'J':
	{//射击！
		Shell* sh = mytank->Attack();
		ps.set_value(sh);
		this_thread::sleep_for(chrono::milliseconds(TANKSLEEPINGTIME));
		return;
	}
	default:
		break;
	}
	ps.set_value(NULL);
	this_thread::sleep_for(chrono::milliseconds(TANKSLEEPINGTIME));
	return;
}
//调试成功



void shellCtrl(vector<Shell*> &allsh, vector<vector<Widget*>>& vw)
{
	for (int i = 0; i < allsh.size(); )
	{

		//子弹大小：3,SHELLSIZE
		//unsolved problem!
		if (allsh[i] == nullptr)
		{
			allsh.erase(allsh.begin() + i);
			continue;
		}
		string thisdir = allsh[i]->getDir();
		int flag = 0;

		if ("up" == thisdir)
		{
			flag = shellUpHit(allsh, vw,i);
		}
		else if ("down" == thisdir)
		{
			flag = shellDownHit(allsh, vw, i);
		}
		else if ("right" == thisdir)
		{
			flag = shellRightHit(allsh, vw, i);
		}
		else if ("left" == thisdir)
		{
			flag = shellLeftHit(allsh, vw, i);
		}
		if (flag == 0)
		{
			allsh[i]->Move();
			++i;
		}
	}
	this_thread::sleep_for(chrono::milliseconds(SHELLSLEEPINGTIME));
}



int shellUpHit(vector<Shell*>& allsh, vector<vector<Widget*>>& vw, int i)
{
	bool over = false;
	pair<int, int> pp = allsh[i]->giveCoordinate();
	int cur_x = pp.first;
	int cur_y = pp.second;
	int x_coo = (cur_x - 1) / XSIZE;
	int y_coo = (cur_y - 1) / YSIZE;
	//是否越界


	if (x_coo < 0 || x_coo>19 || y_coo < 0 || y_coo>15)
	{
		allsh[i]->undrawWidget();
		Shell* tm = allsh[i];
		allsh.erase(allsh.begin() + i);
		delete tm;
		return 1;
	}
	if (x_coo + 1 <= 19 && vw[x_coo+1][y_coo] != NULL)
		vw[x_coo+1][y_coo]->drawWidget();
	//不经过左右边线
	if (x_coo != (cur_x + SHELLSIZE) / XSIZE)
	{
		over = true;
	}

	//贴脸情况
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//接下来处理碰撞结果
		//如果碰撞成功
		if (1 == res)
		{
			//碰撞成功！
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo]->undrawWidget();
				if(vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//左边没撞再撞右边
		if (over == true && allsh[i]->collide(vw[x_coo + 1][y_coo]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo + 1][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo + 1][y_coo]->undrawWidget();
				if (vw[x_coo + 1][y_coo]->isatank == false)
					delete vw[x_coo + 1][y_coo];
				vw[x_coo + 1][y_coo] = NULL;
			}
			else vw[x_coo + 1][y_coo]->drawWidget();
			return 1;
		}
	}



	return 0;
}





int shellDownHit(vector<Shell*>& allsh, vector<vector<Widget*>>& vw, int i)
{
	bool over = false;
	pair<int, int> pp = allsh[i]->giveCoordinate();
	int cur_x = pp.first;
	int cur_y = pp.second;
	int x_coo = (cur_x - 1) / XSIZE;
	int y_coo = (cur_y - 1) / YSIZE;
	//是否越界
	if (x_coo < 0 || x_coo>19 || y_coo < 0 || y_coo>15)
	{
		Shell* tmp = allsh[i];
		allsh[i]->undrawWidget();
		allsh.erase(allsh.begin() + i);
		delete tmp;
		return 1;
	}
	if (x_coo+1<=19&&vw[x_coo + 1][y_coo] != NULL)
		vw[x_coo + 1][y_coo]->drawWidget();
	//不经过左右边线
	if (x_coo != (cur_x + SHELLSIZE) / XSIZE)
	{
		over = true;
	}



	//贴脸情况
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//接下来处理碰撞结果
		//如果碰撞成功
		if (1 == res)
		{
			//碰撞成功！
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//左边没撞再撞右边
		if (over == true && allsh[i]->collide(vw[x_coo + 1][y_coo]) == 1)
		{
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo + 1][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo + 1][y_coo]->undrawWidget();
				if (vw[x_coo+1][y_coo]->isatank == false)
					delete vw[x_coo+1][y_coo];
				vw[x_coo + 1][y_coo] = NULL;
			}
			else vw[x_coo+1][y_coo]->drawWidget();
			return 1;
		}
	}

	return 0;
}



int shellRightHit(vector<Shell*>& allsh, vector<vector<Widget*>>& vw, int i)
{
	bool over = false;
	pair<int, int> pp = allsh[i]->giveCoordinate();
	int cur_x = pp.first;
	int cur_y = pp.second;
	int x_coo = (cur_x - 1) / XSIZE;
	int y_coo = (cur_y - 1) / YSIZE;
	//是否越界
	if (x_coo < 0 || x_coo>19 || y_coo < 0 || y_coo>15)
	{
		Shell* tmp = allsh[i];
		allsh[i]->undrawWidget();
		allsh.erase(allsh.begin() + i);
		delete tmp;
		return 1;
	}
	if (y_coo + 1 <= 15 && vw[x_coo][y_coo+1] != NULL)
		vw[x_coo][y_coo+1]->drawWidget();
	//不经过前后边线
	if (y_coo != (cur_y + SHELLSIZE) / YSIZE)
	{
		over = true;
	}

	//贴脸情况
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//接下来处理碰撞结果
		//如果碰撞成功
		if (1 == res)
		{
			//碰撞成功！
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//左边没撞再撞右边
		if (over == true && allsh[i]->collide(vw[x_coo][y_coo+1]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo+1]->undrawWidget();
				if (vw[x_coo][y_coo+1]->isatank == false)
					delete vw[x_coo][y_coo+1];
				vw[x_coo][y_coo+1] = NULL;
			}
			else vw[x_coo][y_coo+1]->drawWidget();
			return 1;
		}
	}
	return 0;
}



int shellLeftHit(vector<Shell*>& allsh, vector<vector<Widget*>>& vw, int i)
{
	bool over = false;
	pair<int, int> pp = allsh[i]->giveCoordinate();
	int cur_x = pp.first;
	int cur_y = pp.second;
	int x_coo = (cur_x - 1) / XSIZE;
	int y_coo = (cur_y - 1) / YSIZE;
	//是否越界
	if (x_coo < 0 || x_coo>19 || y_coo < 0 || y_coo>15)
	{
		Shell* tmp = allsh[i];
		allsh[i]->undrawWidget();
		allsh.erase(allsh.begin() + i);
		delete tmp;
		return 1;
	}
	if (y_coo + 1 <= 15 && vw[x_coo][y_coo + 1] != NULL)
		vw[x_coo][y_coo + 1]->drawWidget();
	//不经过前后边线
	if (y_coo != (cur_y + SHELLSIZE) / YSIZE)
	{
		over = true;
	}

	//贴脸情况
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//接下来处理碰撞结果
		//如果碰撞成功
		if (1 == res)
		{
			//碰撞成功！
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//左边没撞再撞右边
		if (over == true && allsh[i]->collide(vw[x_coo][y_coo + 1]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//原物件被摧毁
				vw[x_coo][y_coo + 1]->undrawWidget();
				if (vw[x_coo][y_coo+1]->isatank == false)
					delete vw[x_coo+1][y_coo];
				vw[x_coo][y_coo + 1] = NULL;
			}
			else vw[x_coo][y_coo+1]->drawWidget();
			return 1;
		}
	}

	
	return 0;
}