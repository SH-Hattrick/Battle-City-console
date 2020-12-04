#pragma once
#include"Tools.h"
#include<ctime>

//该头文件包含以下类：
//所有坦克类（基类：Tank）



class Tank : public Widget
{
protected:
	
	int power;
	int speed;
	int x_dir = 0;
	int y_dir = -1;

	bool existence = false;
	bool isplayer = false;
	IMAGE playertank_up;
	IMAGE playertank_down;
	IMAGE playertank_right;
	IMAGE playertank_left;
public:
	int id;
	int steptimes = 0;
	int shtimes = 0;
	Tank(int p, int h, int x, int y, int s) :Widget(x, y, h), power(p), speed(s) {
		existence = true; 
		isatank = true; 
		loadimage(&playertank_up, _T("../images/player1tank_up.jpg"), XSIZE, YSIZE);
		loadimage(&playertank_down, _T("../images/player1tank_down.jpg"), XSIZE, YSIZE);
		loadimage(&playertank_right, _T("../images/player1tank_right.jpg"), XSIZE, YSIZE);
		loadimage(&playertank_left, _T("../images/player1tank_left.jpg"), XSIZE, YSIZE);
		srand((unsigned int)time(NULL));
		id = rand() % 100;
		maxhealth = health;
	}
	virtual Shell* Attack();
	virtual void Damaged(Shell*);
	void Move(int nxdir,int nydir);
	void changeDir(int nxdir, int nydir);
	void setplayer() { isplayer = true; }
	void playerDrawWidget();
	void instantKill() { power = 5000; }
	void speedup() { speed *= 2; }
	void setID(int i);
	void restoreHealth() { health = maxhealth; }
	string giveDirection();
	bool ifDead();
};



class LightTank : public Tank
{
private:
	IMAGE up;
	IMAGE down;
	IMAGE left;
	IMAGE right;
public:
	LightTank(int x, int y) :Tank(5, 15, x, y, 8){
		loadimage(&up, _T("../images/lighttank_up.jpg"), XSIZE, YSIZE);
		loadimage(&down, _T("../images/lighttank_down.jpg"), XSIZE, YSIZE);
		loadimage(&right, _T("../images/lighttank_right.jpg"), XSIZE, YSIZE);
		loadimage(&left, _T("../images/lighttank_left.jpg"), XSIZE, YSIZE);
	}
	void drawWidget();

};



class ArmoredTank : public Tank
{
	
private:
	IMAGE up;
	IMAGE down;
	IMAGE left;
	IMAGE right;
public:
	ArmoredTank(int x, int y) :Tank(5, 20, x, y, 4) {
		loadimage(&up, _T("../images/armoredtank_up.jpg"), XSIZE, YSIZE);
		loadimage(&down, _T("../images/armoredtank_down.jpg"), XSIZE, YSIZE);
		loadimage(&right, _T("../images/armoredtank_right.jpg"), XSIZE, YSIZE);
		loadimage(&left, _T("../images/armoredtank_left.jpg"), XSIZE, YSIZE);
	}
	void drawWidget();


};



class HeavyTank : public Tank
{
private:
	IMAGE up;
	IMAGE down;
	IMAGE left;
	IMAGE right;
public:
	HeavyTank(int x, int y) :Tank(5, 25, x, y, 4) {
		loadimage(&up, _T("../images/heavytank_up.jpg"), XSIZE, YSIZE);
		loadimage(&down, _T("../images/heavytank_down.jpg"), XSIZE, YSIZE);
		loadimage(&right, _T("../images/heavytank_right.jpg"), XSIZE, YSIZE);
		loadimage(&left, _T("../images/heavytank_left.jpg"), XSIZE, YSIZE);
	}
	void drawWidget();

};



