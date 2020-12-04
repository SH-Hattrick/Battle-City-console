#include"Control.h"
void allTankCtrl(Tank* mytank, vector<vector<Widget*>>& vw, promise<Shell*>& ps, char ifai)
{
	static Widget* tmp = NULL;
	if (tmp != NULL)
		tmp->drawWidget();
	
	char c;
	//����Ϊ0��ΪAI����
	if (0 != ifai)//AIͨ�ú���
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

	//��ȡ����
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
		//���������꣺��ֱ���ƶ������ж�
		if (0 == xflag)
			mytank->Move(-1, 0);
		else
		{
			//�Ƿ��ڵ�ͼ�߽�
			if (cur_x - 1 < 0)
				break;
			//��ǰ���Ƿ�ɴ���
			if (NULL == vw[cur_x - 1][cur_y] || vw[cur_x - 1][cur_y]->penetrable)
			{
				//��ǰ���Ƿ�ɴ���
				if (1 == yflag || (NULL == vw[cur_x - 1][cur_y + 1] || vw[cur_x - 1][cur_y + 1]->penetrable))
				{
					mytank->Move(-1, 0);
					//�л�����������
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
			//�ƶ�֮������Ϊ���㣬�ƶ�̹�������
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
			//�Ƿ��ڵ�ͼ�߽�
			if (cur_y + 1 > 15)
				break;
			//��ǰ���Ƿ���Դ���
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
		//���������꣺��ֱ���ƶ������ж�
		if (0 == xflag)
		{
			mytank->Move(1, 0);
			//�ƶ�֮������Ϊ���㣬�ƶ�̹�������
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
			//�Ƿ��ڵ�ͼ�߽�
			if (cur_x + 1 > 19)
				break;
			//��ǰ���Ƿ�ɴ���
			if (NULL == vw[cur_x + 1][cur_y] || vw[cur_x + 1][cur_y]->penetrable)
			{
				//��ǰ���Ƿ�ɴ���
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
			//�Ƿ��ڵ�ͼ�߽�
			if (cur_y - 1 < 0)
				break;
			//��ǰ���Ƿ���Դ���
			if (NULL == vw[cur_x][cur_y - 1] || vw[cur_x][cur_y - 1]->penetrable)
			{
				if (1 == xflag || (NULL == vw[cur_x + 1][cur_y - 1] || vw[cur_x + 1][cur_y - 1]->penetrable))
				{
					mytank->Move(0, -1);
					//�ƶ�֮������Ϊ���㣬�ƶ�̹�������
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
	{//�����
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
//���Գɹ�



void shellCtrl(vector<Shell*> &allsh, vector<vector<Widget*>>& vw)
{
	for (int i = 0; i < allsh.size(); )
	{

		//�ӵ���С��3,SHELLSIZE
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
	//�Ƿ�Խ��


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
	//���������ұ���
	if (x_coo != (cur_x + SHELLSIZE) / XSIZE)
	{
		over = true;
	}

	//�������
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//������������ײ���
		//�����ײ�ɹ�
		if (1 == res)
		{
			//��ײ�ɹ���
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
				vw[x_coo][y_coo]->undrawWidget();
				if(vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//���ûײ��ײ�ұ�
		if (over == true && allsh[i]->collide(vw[x_coo + 1][y_coo]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo + 1][y_coo]->existence == false)
			{
				//ԭ������ݻ�
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
	//�Ƿ�Խ��
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
	//���������ұ���
	if (x_coo != (cur_x + SHELLSIZE) / XSIZE)
	{
		over = true;
	}



	//�������
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//������������ײ���
		//�����ײ�ɹ�
		if (1 == res)
		{
			//��ײ�ɹ���
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//���ûײ��ײ�ұ�
		if (over == true && allsh[i]->collide(vw[x_coo + 1][y_coo]) == 1)
		{
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo + 1][y_coo]->existence == false)
			{
				//ԭ������ݻ�
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
	//�Ƿ�Խ��
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
	//������ǰ�����
	if (y_coo != (cur_y + SHELLSIZE) / YSIZE)
	{
		over = true;
	}

	//�������
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//������������ײ���
		//�����ײ�ɹ�
		if (1 == res)
		{
			//��ײ�ɹ���
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//���ûײ��ײ�ұ�
		if (over == true && allsh[i]->collide(vw[x_coo][y_coo+1]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
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
	//�Ƿ�Խ��
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
	//������ǰ�����
	if (y_coo != (cur_y + SHELLSIZE) / YSIZE)
	{
		over = true;
	}

	//�������
	if (vw[x_coo][y_coo] != NULL)
	{
		int res = allsh[i]->collide(vw[x_coo][y_coo]);
		//������������ײ���
		//�����ײ�ɹ�
		if (1 == res)
		{
			//��ײ�ɹ���
			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
				vw[x_coo][y_coo]->undrawWidget();
				if (vw[x_coo][y_coo]->isatank == false)
					delete vw[x_coo][y_coo];
				vw[x_coo][y_coo] = NULL;
			}
			else vw[x_coo][y_coo]->drawWidget();
			return 1;
		}
		//���ûײ��ײ�ұ�
		if (over == true && allsh[i]->collide(vw[x_coo][y_coo + 1]) == 1)
		{

			allsh.erase(allsh.begin() + i);
			if (vw[x_coo][y_coo]->existence == false)
			{
				//ԭ������ݻ�
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