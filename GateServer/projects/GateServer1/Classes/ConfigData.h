#ifndef __LYNX_GAME_SERVER_ConfigData_H__
#define __LYNX_GAME_SERVER_ConfigData_H__
#include "stdafx.h"

using namespace std;

#define CARDNUMBER 120

enum HuTypeEnum
{
	None=0,
	PI,//屁胡
	QINGYISE,//清一色
	QIDUI,//七对
	PENGPENG,//碰碰胡
	QYSQD,//清一色七对
	QYSPENG,//清一色碰碰胡
	
};

enum HeiOrYing
{
	HEI,
	RUAN,
};

struct HuItem
{
	HuTypeEnum	_hutype;
	HeiOrYing	_hy;
	bool		_ka;//卡牌或者单调
	bool		_menqing;//门清
	bool		_gangkai;
	bool		_qianggang;
	int			_pao;
};

static int g_all_mjkind[] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
	0x31, 0x32, 0x33, 0x34,
	0x35, 0x36, 0x37
};

#define g_kind (sizeof(g_all_mjkind)/sizeof(int))

static int g_all_mj[] = {
	g_all_mjkind[0], g_all_mjkind[0], g_all_mjkind[0], g_all_mjkind[0],
	g_all_mjkind[1], g_all_mjkind[1], g_all_mjkind[1], g_all_mjkind[1],
	g_all_mjkind[2], g_all_mjkind[2], g_all_mjkind[2], g_all_mjkind[2],
	g_all_mjkind[3], g_all_mjkind[3], g_all_mjkind[3], g_all_mjkind[3],
	g_all_mjkind[4], g_all_mjkind[4], g_all_mjkind[4], g_all_mjkind[4],
	g_all_mjkind[5], g_all_mjkind[5], g_all_mjkind[5], g_all_mjkind[5],
	g_all_mjkind[6], g_all_mjkind[6], g_all_mjkind[6], g_all_mjkind[6],
	g_all_mjkind[7], g_all_mjkind[7], g_all_mjkind[7], g_all_mjkind[7],
	g_all_mjkind[8], g_all_mjkind[8], g_all_mjkind[8], g_all_mjkind[8],

	g_all_mjkind[9], g_all_mjkind[9], g_all_mjkind[9], g_all_mjkind[9],
	g_all_mjkind[10], g_all_mjkind[10], g_all_mjkind[10], g_all_mjkind[10],
	g_all_mjkind[11], g_all_mjkind[11], g_all_mjkind[11], g_all_mjkind[11],
	g_all_mjkind[12], g_all_mjkind[12], g_all_mjkind[12], g_all_mjkind[12],
	g_all_mjkind[13], g_all_mjkind[13], g_all_mjkind[13], g_all_mjkind[13],
	g_all_mjkind[14], g_all_mjkind[14], g_all_mjkind[14], g_all_mjkind[14],
	g_all_mjkind[15], g_all_mjkind[15], g_all_mjkind[15], g_all_mjkind[15],
	g_all_mjkind[16], g_all_mjkind[16], g_all_mjkind[16], g_all_mjkind[16],
	g_all_mjkind[17], g_all_mjkind[17], g_all_mjkind[17], g_all_mjkind[17],

	g_all_mjkind[18], g_all_mjkind[18], g_all_mjkind[18], g_all_mjkind[18],
	g_all_mjkind[19], g_all_mjkind[19], g_all_mjkind[19], g_all_mjkind[19],
	g_all_mjkind[20], g_all_mjkind[20], g_all_mjkind[20], g_all_mjkind[20],
	g_all_mjkind[21], g_all_mjkind[21], g_all_mjkind[21], g_all_mjkind[21],
	g_all_mjkind[22], g_all_mjkind[22], g_all_mjkind[22], g_all_mjkind[22],
	g_all_mjkind[23], g_all_mjkind[23], g_all_mjkind[23], g_all_mjkind[23],
	g_all_mjkind[24], g_all_mjkind[24], g_all_mjkind[24], g_all_mjkind[24],
	g_all_mjkind[25], g_all_mjkind[25], g_all_mjkind[25], g_all_mjkind[25],
	g_all_mjkind[26], g_all_mjkind[26], g_all_mjkind[26], g_all_mjkind[26],

	g_all_mjkind[27], g_all_mjkind[27], g_all_mjkind[27], g_all_mjkind[27],
	g_all_mjkind[28], g_all_mjkind[28], g_all_mjkind[28], g_all_mjkind[28],
	g_all_mjkind[29], g_all_mjkind[29], g_all_mjkind[29], g_all_mjkind[29],
	g_all_mjkind[30], g_all_mjkind[30], g_all_mjkind[30], g_all_mjkind[30],
	
	g_all_mjkind[31], g_all_mjkind[31], g_all_mjkind[31], g_all_mjkind[31],
	g_all_mjkind[32], g_all_mjkind[32], g_all_mjkind[32], g_all_mjkind[32],
	g_all_mjkind[33], g_all_mjkind[33], g_all_mjkind[33], g_all_mjkind[33],
};

class ConfigData 
{
public:
	ConfigData();
	virtual ~ConfigData();
	
	static ConfigData *getIns();
	void init();
	void quickSort(int *&s, int l, int r);
	int getMJ();
	int getMJ(int index);
	void initMJ();
	
	void setKezi();
	void setFengKezi(int jj);
	void setFengKeTo(int len, map<string, int> maps);
	void setShunzi();
	void init3P(int index, int kenum);
	void init3L(int shunnum, int index, vector<int>ww);

	HuItem isHu(int *pai, bool ispengqing);
	HuItem isHu(int *pai, bool ispengqing, int bao);
private:
	void setValueZero(int *a, int v, int len,int &baocount);
	HuTypeEnum isFit(vector<int>p, int baocount, HuItem &item);
	map<int, vector<int>> getKindCard(int *temppai);
	void setLiankeBao(int i);
	vector<int> isTing(int *pai,int bao);
	void test();
private:
	
	static ConfigData *m_ins;
	vector<int >m_cards;
	int m_index;
	vector<vector<int>>m_kezi;
	vector<vector<int>>m_shunzi;
	map<int, map<string, int>>m_lianke;
	map<int, map<string, int>>m_liankebao;
};


#endif // __LYNX_GAME_SERVER_ConfigData_H__