#ifndef __LYNX_GAME_SERVER_ConfigData1_H__
#define __LYNX_GAME_SERVER_ConfigData1_H__
#include "stdafx.h"
#include "ConfigData.h"
using namespace std;

class ConfigData1 
{
public:
	ConfigData1();
	virtual ~ConfigData1();
	
	static ConfigData1 *getIns();
	void init();
	HuItem isHu(int *pai, int baovalue);

	void test();
private:
	_int64 getLeftValue(_int64 a1, map<char, int> v2,vector<int> &leftvs);
	vector<_int64> getLeftValue(_int64 a, int baocount, vector<vector<int>> &leftvs);
	HuItem isQidui(int *pai,int baovalue);
	HuItem isPPH(int *pai,int baovalue);
	HuItem isQYS(int *pai,int baovalue);
private:
	static ConfigData1 *m_ins;
	map<int, vector<map<char,int>>> m_hus;
};


#endif // __LYNX_GAME_SERVER_ConfigData1_H__