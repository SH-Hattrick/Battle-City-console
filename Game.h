#pragma once
#include"Control.h"
#include"AITank.h"

#include<vector>
#include<ctime>
#include<fstream>
#include<string>
#include<condition_variable>


//����ʵ��3�����͵�̹�ˣ�����̹�ˡ�װ�׳�������̹��
//����ʵ��3��̹�����ԣ�����ֵ�����������ƶ��ٶ�
//����ʵ��2���ϰ��שǽ����ǽ
//ʵ�ֵ�ǰ�ֵ�״̬������ʾ�������ֵ���о�ʣ��̹������
//��Ϸ����ʱ��ʾʤ�����÷ֵ�ͳ����Ϣ



//�ܻ����С��960*640
//��ͼ��С��800*640
//�������160*640




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


