#pragma once
#include<easyx.h>
#include<conio.h>
#include<graphics.h>
#include<iostream>
//#ifndef TANKH
//#include"Tank.h"
//#endif 




using namespace std;

#define XSIZE 40
#define YSIZE 40
#define BKCOLOR BLACK
#define SHELLSIZE 3

//该头文件包括以下类：
//一切类的基类：Widget
//炮弹类（Shell）:在坦克（被）攻击时生成并被调用
//地形类（Block）:初始化地图时调用
//加成道具类（Buff）:赋予坦克加成属性


//总基类
class Widget
{
protected:
	int _x = 0;
	int _y = 0;
	int x_size = 0;
	int y_size = 0;
	int health = 0;
	int maxhealth = 0;
	IMAGE rim;

	
public:
	bool isatank = false;
	bool isbuff = false;
	bool existence = false;
	bool penetrable = false;
	Widget() {
		existence = true;
		loadimage(&rim, _T("../images/black.jpg"), XSIZE, YSIZE);
		
	}
	Widget(int x, int y, int h) :_x(x), _y(y), health(h) { 
		existence = true;
	loadimage(&rim, _T("../images/black.jpg"), XSIZE, YSIZE);
	}
	void undrawWidget();
	virtual void drawWidget() = 0;
	int giveHealth();
	void reduceHealth(int r);
	pair<int,int> giveCoordinate();
};



//炮弹类
class Shell : public Widget
{
protected:
	int damage = 0;
	int x_direction = 1;
	int y_direction = 0;
	int speed = 8;
	Widget* towner;
public:
	friend class Tank;
	Shell(){}
	Shell(int x, int y, int dam, int sp, int xdir,int ydir) :
		Widget(x, y, 4), damage(dam), x_direction(xdir),y_direction(ydir) { x_size = 5; y_size = 5; }
	void undrawWidget();
	void drawWidget();
	void owner(Widget*);
	string getDir();
	int collide(Widget*);
	void Move();
};



//地形基类
class Block : public Widget
{
protected:

public:
	Block(int x, int y, int h) :Widget(x, y, h) { x_size = XSIZE, y_size = XSIZE; };
	virtual void drawWidget() = 0;
};



//加成道具基类
class Buff : public Widget
{
protected:
	IMAGE* im;
public:
	friend class Tank;
	Buff(int x, int y) :Widget(x, y, 1) { penetrable = true; isbuff = true; }
	virtual void buffTank(Tank*) = 0;
};



//砖墙
class BrickWall : public Block
{
private:
	IMAGE im;
public:
	BrickWall(int x, int y) :Block(x,y,5){

		loadimage(&im, _T("../images/brickwall.jpg"), x_size, y_size);
	}
	void drawWidget();
	
};




//铁墙
class IronWall :public Block
{
private:
	IMAGE im;
public:
	IronWall(int x, int y) :Block(x,y,10000){

		loadimage(&im, _T("../images/ironwall.png"), x_size, y_size);
	}
	void drawWidget();
};


//草
class Grass :public Block
{
private:
	IMAGE im;
public:
	Grass(int x, int y) :Block(x,y ,100000) { 
		penetrable = true;
	loadimage(&im, _T("../images/grass.jpg"), x_size, y_size);
	};
	void drawWidget();
};



class Base :public Block
{
private:
	IMAGE im;
public:
	Base(int x, int y) :Block(x,y,1) {
		loadimage(&im, _T("../images/base.jpg"), x_size, y_size);
	};
	void drawWidget();
};



class healthUp :public Buff
{
public:
	healthUp(int x, int y) :Buff(x, y) {};
	void buffTank(Tank*);
	void drawWidget() override;
};



class SpeedUp :public Buff
{
public:
	SpeedUp(int x, int y) :Buff(x, y) {};
	void buffTank(Tank*);
	void drawWidget() override;
};



class Ironbreaker:public Buff
{
public:
	Ironbreaker(int x, int y) :Buff(x, y) {};
	void buffTank(Tank*);
	void drawWidget() override;
};
