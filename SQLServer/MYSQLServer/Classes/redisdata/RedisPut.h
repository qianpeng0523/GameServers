#ifndef __LYNX_GAME_SERVER_RedisPut_H__
#define __LYNX_GAME_SERVER_RedisPut_H__
#include "stdafx.h"
#include "CSVDataInfo.h"
using namespace std;
using namespace CSV;

class RedisPut 
{
public:
	RedisPut();
	virtual ~RedisPut();
	
	static RedisPut *getIns();
	void init();
	bool PushShop(ShopItem item);
	bool PushActive(Active at);
	bool PushTask(Task task);
	bool PushExAward(ExAward ea);
	bool PushReward(Reward rd);
	bool PushSignAward(SignAward sa);
	bool PushSignZhuan(SignZhuan sz);
	bool PushProp(Prop p);
	bool PushFree(Task task);
	bool PushFirstBuy(CSVFirstBuyItem *sv);
	bool PushExchangeCode(CSVExchangeCode *item);
	bool PushGate(GateData *gd,SERVERTYPE type);
private:
	static RedisPut *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisPut_H__