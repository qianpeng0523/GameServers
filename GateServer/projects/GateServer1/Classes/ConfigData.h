#ifndef __LYNX_GAME_SERVER_ConfigData_H__
#define __LYNX_GAME_SERVER_ConfigData_H__
#include "stdafx.h"

using namespace std;

#define CARDNUMBER 136

static int g_all_mjkind[] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
	0x31, 0x32, 0x33, 0x34,
	0x41, 0x42, 0x43
};

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

	bool isHu(int *p,int len);
	bool isDui(int *p,int index1,int index2);
	bool is3Same(int *p, int index1, int index2,int index3);
	bool is3Lian(int *p, int index1, int index2, int index3);
	bool is7Dui(int *dui7, int len);
	bool isQing(int *p, int len);

	void quickSort(int *&s, int l, int r);
	void setZero(int *&a,int index);
	void copy(int *&a,int *b,int len);//将b复制到a

	int getMJ();
	int getMJ(int index);
	void initMJ();
private:
	static ConfigData *m_ins;
	vector<int >m_cards;
	int m_index;
	map<int, vector<vector<int>>>m_hus;
};


#endif // __LYNX_GAME_SERVER_ConfigData_H__