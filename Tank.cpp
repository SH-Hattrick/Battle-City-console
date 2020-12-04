#include "Tank.h"



Shell* Tank::Attack()
{
	//up
	if (x_dir == 0 && y_dir == -1)
	{
		Shell* sh = new Shell(_x + XSIZE / 2, _y, power, 40, x_dir, y_dir);
		sh->owner(this);
		return sh;
	}
	//down
	else if (x_dir == 0 && y_dir == 1)
	{
		Shell* sh = new Shell(_x + XSIZE / 2, _y + YSIZE, power, 40, x_dir, y_dir);
		sh->owner(this);
		return sh;
	}
	//left
	else if (x_dir == -1 && y_dir == 0)
	{
		Shell* sh = new Shell(_x, _y + YSIZE / 2, power, 40, x_dir, y_dir);
		sh->owner(this);
		return sh;
	}//right
	else if (x_dir == 1 && y_dir == 0)
	{
		Shell* sh = new Shell(_x + XSIZE, _y + YSIZE / 2, power, 40, x_dir, y_dir);
		sh->owner(this);
		return sh;
	}
	
}



//被击中
void Tank::Damaged(Shell* sh)
{
	health -= sh->damage;
	if (ifDead())
		existence = false;
}



void Tank::setID(int i)
{
	id = i;
}



void Tank::changeDir(int nxdir,int nydir)
{

	undrawWidget();
	x_dir = nxdir; y_dir = nydir;
	if (isplayer)
		playerDrawWidget();
	else drawWidget();
}


//移动
void Tank::Move(int nxdir, int nydir)
{
	undrawWidget();
	x_dir = nxdir;
	y_dir = nydir;
	_x += nxdir * speed/2;
	_y += nydir * speed/2;
	if (isplayer)
		playerDrawWidget();
	else drawWidget();
	undrawWidget();
	_x += nxdir * speed / 2;
	_y += nydir * speed / 2;
	if (isplayer)
		playerDrawWidget();
	else drawWidget();
}




string Tank::giveDirection()
{
	if (x_dir == 0 && y_dir == 1)
		return "down";
	else if (x_dir == 0 && y_dir == -1)
		return "up";
	else if (x_dir == 1 && y_dir == 0)
		return "right";
	else if (x_dir == -1 && y_dir == 0)
		return "left";
	else exit(-1);
}



//是否被摧毁
bool Tank::ifDead()
{
	if (health <= 0 || existence == false)
	{
		undrawWidget();
		return true;
	}
	else return false;
}




void Tank::playerDrawWidget()
{
	string dir = giveDirection();
	if ("up" == dir)
		putimage(_x, _y, &playertank_up);
	else if ("down" == dir)
		putimage(_x, _y, &playertank_down);
	else if ("right" == dir)
		putimage(_x, _y, &playertank_right);
	else if ("left" == dir)
		putimage(_x, _y, &playertank_left);


}



void LightTank::drawWidget()
{

	string dir = giveDirection();
	if ("up" == dir)
		putimage(_x, _y, &up);
	else if ("down" == dir)
		putimage(_x, _y, &down);
	else if ("right" == dir)
		putimage(_x, _y, &right);
	else if ("left" == dir)
		putimage(_x, _y, &left);

	
}

void HeavyTank::drawWidget()
{
	string dir = giveDirection();
	if ("up" == dir)
		putimage(_x, _y, &up);
	else if ("down" == dir)
		putimage(_x, _y, &down);
	else if ("right" == dir)
		putimage(_x, _y, &right);
	else if ("left" == dir)
		putimage(_x, _y, &left);


}

void ArmoredTank::drawWidget()
{
	string dir = giveDirection();
	if ("up" == dir)
		putimage(_x, _y, &up);
	else if ("down" == dir)
		putimage(_x, _y, &down);
	else if ("right" == dir)
		putimage(_x, _y, &right);
	else if ("left" == dir)
		putimage(_x, _y, &left);
}



