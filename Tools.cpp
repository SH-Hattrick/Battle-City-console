#include "Tools.h"
#include"Tank.h"
class Tank;
pair<int, int> Widget::giveCoordinate()
{
	pair<int, int> pp(_x, _y);
	return pp;
}

int Widget::giveHealth()
{
	return health;
}



void Widget::reduceHealth(int r)
{
	health -= r;
	if (health <= 0)
	{
		existence = false;
	}
}



void Shell::owner(Widget* t)
{
	towner = t;
}



int Shell::collide(Widget* www)
{
	//未击中有效目标return 0
	if (NULL == www || www->penetrable||www == towner)
		return 0;
	//否则return 1
	else
	{
		www->reduceHealth(damage);
		undrawWidget();
		delete this;
		return 1;
	}
}



string Shell::getDir()
{
	if (x_direction == 0 && y_direction == 1)
		return "down";
	else if (x_direction == 0 && y_direction == -1)
		return "up";
	else if (x_direction == 1 && y_direction == 0)
		return "right";
	else if (x_direction == -1 && y_direction == 0)
		return "left";
	else exit(-1);
}



void Shell::Move()
{
	undrawWidget();
	_x += speed * x_direction;
	_y += speed * y_direction;
	drawWidget();
	undrawWidget();
	_x += speed / 2 * x_direction;
	_y += speed / 2 * y_direction;
	drawWidget();
}




void Shell::drawWidget()
{
	setlinecolor(WHITE);
	setfillcolor(WHITE);
	fillcircle(_x, _y, SHELLSIZE-1);
}



void Shell::undrawWidget()
{
	setlinecolor(BKCOLOR);
	setfillcolor(BKCOLOR);
	fillcircle(_x, _y, SHELLSIZE-1);
}



void BrickWall::drawWidget()
{

	putimage(_x,_y,&im);
}



void Widget::undrawWidget()
{
	putimage(_x, _y, &rim);
}



void IronWall::drawWidget()
{

	putimage(_x, _y, &im);
}



void Grass::drawWidget()
{

	putimage(_x, _y, &im);
}



void Base::drawWidget()
{

	putimage(_x, _y, &im);
}



void healthUp::buffTank(Tank* tank)
{
	tank->restoreHealth();
	undrawWidget();
}



void healthUp::drawWidget()
{
	setfillcolor(YELLOW);
	fillcircle(_x + XSIZE / 2, _y + YSIZE / 2, XSIZE / 4);
}


void SpeedUp::drawWidget()
{
	setfillcolor(RGB(123,0,234));
	fillcircle(_x + XSIZE / 2, _y + YSIZE / 2, XSIZE / 4);
}



void Ironbreaker::drawWidget()
{
	setfillcolor(RGB(1, 140, 54));
	fillcircle(_x + XSIZE / 2, _y + YSIZE / 2, XSIZE / 4);
}



void Ironbreaker::buffTank(Tank* tank)
{
	tank->instantKill();
	undrawWidget();
}



void SpeedUp::buffTank(Tank* tank)
{
	tank->speedup();
	undrawWidget();
}