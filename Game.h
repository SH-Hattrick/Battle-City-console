#pragma once
#include"Control.h"
#include"AITank.h"

#include<vector>
#include<ctime>
#include<fstream>
#include<string>
#include<condition_variable>


//至少实现3种类型的坦克：轻型坦克、装甲车和重型坦克
//至少实现3种坦克属性：生命值、攻击力和移动速度
//至少实现2种障碍物：砖墙、铁墙
//实现当前局的状态栏，显示玩家生命值、敌军剩余坦克数量
//游戏结束时显示胜负，得分等统计信息



//总画面大小：960*640
//地图大小：800*640
//侧边栏：160*640




using namespace std;
using namespace this_thread;

void mainGame();

void welcomePage();

void gameProcess();

void initBeginningGraph();

void initEndingGraph();

void readMapInFile();

void buffPlayerTank(Tank*, vector<Buff*>&);

int generateEnemy(vector<Tank*>&, vector<vector<Widget*>>& vw);

void initMap(vector<vector<Widget*>>&, vector<Widget*>&,vector<Buff*>&);

void gameData(int defeat,int h);

bool winningCondition(int t);

bool losingCondition(Tank* player1tank, vector<vector<Widget*>> vw);

LightTank* initTankAndBase(vector<vector<Widget*>>& map);


