#include"Game.h"

//1.砖墙 2.铁墙

int mymap[20][16];
int level = 0;
bool ifwin = false;

void mainGame()
{
	initgraph(960, 640);
	welcomePage();
	while (true)
	{
		initBeginningGraph();
		thread game(gameProcess);
		game.join();
		int flag = 0;
		initEndingGraph();
		while (true)
		{
			char cmd = _getch();
			if (cmd == 27)
			{
				flag = 1;
				break;
			}
			else if (cmd == 'c')
				break;
		}
		if (flag == 1)
			break;
	}
	closegraph();
}


void gameProcess()
{
	ifwin = false;
	int win_or_lose = 0;
	//20*16
	//地图资源
	readMapInFile();
	vector<vector<Widget*>> mapWidgets;
	//炮弹对象
	vector<Shell*> allShells;
	//敌军对象
	vector<Tank*> enemies;
	//可穿越物对象
	vector<Widget*> can_be_penetrated;
	//Buff对象
	vector<Buff*> allBuffs;
	//玩家
	initMap(mapWidgets, can_be_penetrated,allBuffs);
	LightTank* player1tank = initTankAndBase(mapWidgets);
	player1tank->setplayer();
	//生成敌军
	int enemynum = generateEnemy(enemies, mapWidgets);
	//总共击败的敌军数量
	int defeated = 0;
	
	//如何使用多线程？
	//默认基地位置：10/15
	int refresh_penetr = 0;
	while (true)
	{
		//if (_kbhit())
		//{
		//	char ui = _getch();
		//	if(ui == 'p')
		//		system("pause");
		//}

		for(int i=0;i<enemies.size();++i)
			if (enemies[i]->ifDead() == true)
			{
				enemies[i]->undrawWidget();
				enemies.erase(enemies.begin() + i);
			}
		if (enemies.size() < 2)
		{
			//默认最多同时存在三个敌军坦克
			int tmp = generateEnemy(enemies, mapWidgets);
			defeated += tmp;
			enemynum = 2;
		}
		initBeginningGraph();

		//玩家与炮弹
		promise<Shell*> proShell;
		future<Shell*> fuShell = proShell.get_future();
		thread player(allTankCtrl, player1tank, ref(mapWidgets), ref(proShell), 0);
		thread shells(shellCtrl, ref(allShells), ref(mapWidgets));
		player.join();
		shells.join();
		
		thread buffth(buffPlayerTank, player1tank, ref(allBuffs));
		buffth.join();

		//AI坦克线程
		promise<Shell*> pro1;
		future<Shell*> fut1 = pro1.get_future();
		thread th1(AITankControl, enemies[0], ref(mapWidgets), ref(pro1));
		th1.join();

		promise<Shell*> pro2;
		future<Shell*> fut2 = pro2.get_future();
		thread th2(AITankControl, enemies[1], ref(mapWidgets), ref(pro2));
		th2.join();

		//处理炮弹
		Shell* nshell1 = fut1.get();
		if (nshell1 != NULL)
			allShells.push_back(nshell1);
		Shell* nshell2 = fut2.get();
		if (nshell1 != NULL)
			allShells.push_back(nshell2);

		//最多同时两辆敌军坦克

		Shell* newshell = fuShell.get();
		if (newshell != NULL)
		{
			allShells.push_back(newshell);
		}

		gameData(defeated, player1tank->giveHealth() / 5);
		if (winningCondition(defeated))
		{
			win_or_lose = 1;
			break;
		}
		if (losingCondition(player1tank, mapWidgets))
			break;
		if (refresh_penetr > 5)
		{
			for (int i = 0; i < can_be_penetrated.size(); ++i)
				can_be_penetrated[i]->drawWidget();
			for (int i = 0; i < allBuffs.size(); ++i)
				allBuffs[i]->drawWidget();
			refresh_penetr = 0;
		}
		else ++refresh_penetr;

	}
	if (win_or_lose == 1)
	{
		IMAGE im;
		loadimage(&im, _T("../images/victory.jpg"), getwidth(), getheight());
		putimage(0, 0, &im);
		Sleep(2000);
	}
	
	else
	{
		IMAGE im;
		loadimage(&im, _T("../images/lost.jpg"), getwidth(), getheight());
		putimage(0, 0, &im);
		Sleep(2000);
	}
	//this_thread::sleep_for(chrono::microseconds(10))
}



void welcomePage()
{
	IMAGE wel;
	loadimage(&wel, _T("../images/welcome.jpg"), getwidth(), getheight());
	putimage(0, 0, &wel);
	Sleep(2000);
	cleardevice();
}



int generateEnemy(vector<Tank*>& en,vector<vector<Widget*>>& vw)
{
	//20*16
	int num = 0;
	srand((unsigned int)time(NULL));
	while (en.size() < 2)
	{
		int y = rand() % 2;
		if (y==0&&vw[0][0] == NULL)
		{
			HeavyTank* tank1 = new HeavyTank(0 * XSIZE, 0 * YSIZE);
			tank1->setID(20);
			vw[0][0] = tank1;
			en.push_back(tank1);
			num++;
			tank1->drawWidget();
		}
			
		else if (y==1&&vw[19][0] == NULL)
		{
			ArmoredTank* tank2 = new ArmoredTank(19 * XSIZE, 0 * YSIZE);
			tank2->setID(60);
			vw[19][0] = tank2;
			en.push_back(tank2);
			num++;
			tank2->drawWidget();
		}
	}
	return num;
}



void initBeginningGraph()
{
	setbkcolor(BKCOLOR);
	setcolor(WHITE);
	setlinecolor(WHITE);
	line(800, 0, 800, 640);
}



void initEndingGraph()
{
	cleardevice();
	WCHAR info1[100];
	WCHAR info2[100];
	if (ifwin == false)
	{
		swprintf_s(info1, _T("Would you like to play again?"));
	}
	else swprintf_s(info1, _T("Advance to next level?"));
	swprintf_s(info2, _T("Press ESC to exit and \'c\' to contintue"));
	outtextxy(400, 280, info1);
	outtextxy(400, 320, info2);
	Sleep(500);
}


////////////////////////////////////////////////
bool winningCondition(int t)
{
	if (t >= 5)
	{
		Sleep(1000);
		level++;
		ifwin = true;
		return true;
	}
	else return false;
}



bool losingCondition(Tank* player1tank, vector<vector<Widget*>> vw)
{
	if (player1tank->ifDead()|| vw[10][15] == NULL)
	{
		Base* bb = new Base(10 * XSIZE, 15 * YSIZE);
		clock_t a, b;
		a = clock();
		while (true)
		{
			b = clock();
			double d = (double)(b - a) / CLOCKS_PER_SEC;
			if (d > 2.0)
			{
				return true;
			}
			if ((d > 0.3&&d<0.8)||(d>1.2&&d<1.7))
			{
				bb->drawWidget();
			}
			else if ((d > 0.8&&d<1.2)||d>1.7)
			{
				bb->undrawWidget();
			}

		}
		return true;
	}

	return false;
}



void initMap(vector<vector<Widget*>>& map,vector<Widget*> &cp, vector<Buff*>&bb)
{
	cleardevice();
	for (int i = 0; i < 20; ++i)
	{
		vector<Widget*> newroll;
		for (int j = 0; j < 16; ++j)
		{
			if (mymap[i][j] == 0)
			{

				//srand((unsigned int)time(NULL));
				//int y = rand() % 10;
				//if (j > 3 && j < 13 && bb.size() < 4 && y >7)
				//{
				//	healthUp* th = new healthUp(i * XSIZE, j * YSIZE);
				//	th->drawWidget();
				//	newroll.push_back(th);
				//	cp.push_back(th);
				//}
				newroll.push_back(NULL);
			}
			else if (mymap[i][j] == 1)
			{
				BrickWall* bb = new BrickWall(i * XSIZE, j * YSIZE);
				bb->drawWidget();
				newroll.push_back(bb);
			}
			else if (mymap[i][j] == 2)
			{
				IronWall* ii = new IronWall(i * XSIZE, j * YSIZE);
				ii->drawWidget();
				newroll.push_back(ii);
			}
			else if (mymap[i][j] == 3)
			{
				Grass* gr = new Grass(i * XSIZE, j * YSIZE);
				gr->drawWidget();
				newroll.push_back(gr);
				cp.push_back(gr);
			}
			//buff
			else if (mymap[i][j] == 4)
			{
				healthUp* th = new healthUp(i * XSIZE, j * YSIZE);
				th->drawWidget();
				newroll.push_back(NULL);
				bb.push_back(th);
			}
			else if (mymap[i][j] == 5)
			{
				SpeedUp* th = new SpeedUp(i * XSIZE, j * YSIZE);
				th->drawWidget();
				newroll.push_back(NULL);
				bb.push_back(th);
			}
			else if (mymap[i][j] == 6)
			{
				Ironbreaker* th = new Ironbreaker(i * XSIZE, j * YSIZE);
				th->drawWidget();
				newroll.push_back(NULL);
				bb.push_back(th);
			}
			else newroll.push_back(NULL);

			
		}
		map.push_back(newroll);
	}
}

LightTank* initTankAndBase(vector<vector<Widget*>>& map)
{
	LightTank *mytank=new LightTank(13*XSIZE,15*YSIZE);
	map[13][15] = mytank;
	mytank->playerDrawWidget();


	Base* b = new Base(10*XSIZE,15*YSIZE);
	map[10][15] = b;


	//生成基地旁一圈墙
	if (1)
	{
		BrickWall* br1 = new BrickWall(9 * XSIZE, 15 * YSIZE);
		BrickWall* br2 = new BrickWall(9 * XSIZE, 14 * YSIZE);
		BrickWall* br3 = new BrickWall(10 * XSIZE, 14 * YSIZE);
		BrickWall* br4 = new BrickWall(11 * XSIZE, 14 * YSIZE);
		BrickWall* br5 = new BrickWall(11 * XSIZE, 15 * YSIZE);
		map[9][15] = br1;
		map[9][14] = br2;
		map[10][14] = br3;
		map[11][14] = br4;
		map[11][15] = br5;
		br1->drawWidget();
		br2->drawWidget();
		br3->drawWidget();
		br4->drawWidget();
		br5->drawWidget();
	}
	else
	{
		IronWall* br1 = new IronWall(9 * XSIZE, 15 * YSIZE);
		IronWall* br2 = new IronWall(9 * XSIZE, 14 * YSIZE);
		IronWall* br3 = new IronWall(10 * XSIZE, 14 * YSIZE);
		IronWall* br4 = new IronWall(11 * XSIZE, 14 * YSIZE);
		IronWall* br5 = new IronWall(11 * XSIZE, 15 * YSIZE);
		map[9][15] = br1;
		map[9][14] = br2;
		map[10][14] = br3;
		map[11][14] = br4;
		map[11][15] = br5;
		br1->drawWidget();
		br2->drawWidget();
		br3->drawWidget();
		br4->drawWidget();
		br5->drawWidget();
	}



	b->drawWidget();

	return mytank;
}



void gameData(int defeated,int health)
{
	//800-960可用
	static int time = 0;
	if (defeated == 5 ||health == 0 )
		time = 0;
	//计时器
	TCHAR cha[100];
	swprintf_s(cha, _T("time used: %d"), time/45);
	time++;
	outtextxy(820, 100, cha);
	//已击杀
	TCHAR killed[100];
	TCHAR left[100];
	swprintf_s(killed, _T("has defeated: %d"), defeated);
	swprintf_s(left, _T("kills left: %d"), 5 - defeated);
	outtextxy(820, 400, killed);
	outtextxy(820, 300, left);
	//剩余血量
	TCHAR heALTH[100];
	swprintf_s(heALTH, _T("plater health left: %d"), health);
	outtextxy(820, 500, heALTH);
}




void buffPlayerTank(Tank*pt, vector<Buff*>& bb)
{
	int xx = pt->giveCoordinate().first / XSIZE;
	int yy = pt->giveCoordinate().second / YSIZE;
	for (int i = 0; i < bb.size(); )
		if (bb[i]->giveCoordinate().first / XSIZE == xx && bb[i]->giveCoordinate().second / YSIZE == yy)
		{
			bb[i]->buffTank(pt);
			bb[i]->undrawWidget();
			delete bb[i];
			bb.erase(bb.begin() + i);
		}
		else ++i;
}




void readMapInFile()
{
	ifstream file("../map/map.txt", ios::in);
	if (file.fail())
	{
		cerr << "File opening error!\n";
		exit(-1);
	}
	vector<vector<int>> tmp;
	for (int i = 0; i < level; ++i)
	{
		for (int j = 0; j < 19; ++j)
		{
			string s;
			getline(file, s);
		}
	}
	for (int i = 0; i < 20; ++i)
	{

		for (int j = 0; j < 16; ++j)
		{
			int num;
			file >> num;
			mymap[i][j] = num;
		}
	}
	file.close();
}