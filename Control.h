#pragma once
#include"Tools.h"
#include"Tank.h"
#include<vector>
#include<future>
#include<thread>
#include<atomic>
#include<mutex>

using namespace std;

#define TANKSLEEPINGTIME 5
#define SHELLSLEEPINGTIME 1

void allTankCtrl(Tank*, vector<vector<Widget*>>& vw, promise<Shell*>& ps, char ifai = 0);

void shellCtrl(vector<Shell*> &allshell, vector<vector<Widget*>>& vw);

int shellUpHit(vector<Shell*>& allshell, vector<vector<Widget*>>& vw, int index);

int shellDownHit(vector<Shell*>& allshell, vector<vector<Widget*>>& vw, int index);

int shellRightHit(vector<Shell*>& allshell, vector<vector<Widget*>>& vw, int index);

int shellLeftHit(vector<Shell*>& allshell, vector<vector<Widget*>>& vw, int index);
