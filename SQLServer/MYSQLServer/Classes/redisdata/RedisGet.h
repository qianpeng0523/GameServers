#ifndef __LYNX_GAME_SERVER_RedisGet_H__
#define __LYNX_GAME_SERVER_RedisGet_H__
#include "stdafx.h"
#include "CSVDataInfo.h"
using namespace std;

struct FirstBuyItem :public Object
{
	int _sid;
	vector<int> _rid;
	int _conid;
	vector<int> _giveid;
};

class RedisGet 
{
public:
	RedisGet();
	virtual ~RedisGet();
	
	static RedisGet *getIns();
	void init();

	map<string, REDISDBName *> getREDISDBNames();
	REDISDBName *getREDISDBName(string name);

	bool SelectDB(REDISTYPE type);
	int getRedisDBIndex(string name);

	map<string, map<int, GateData *>> getGates();
	GateData *getGateData(string name,int type);
	std::vector<ShopItem >getShop();
	vector<Active > getActive(int type);
	vector<Task > getTask();
	vector<ExAward> getExAward();
	map<int, Reward> getReward();
	Reward getReward(int rid);
	vector<SignAward> getSignAward();
	vector<SignZhuan> getSignZhuan();
	map<int, Prop > getProp();
	Prop getProp(int pid);
	vector<Task > getFree();
	FirstBuyItem *getFirstBuy();
	vector<CSVExchangeCode >getCSVExchangeCodes();
private:
	static RedisGet *m_ins;
	redis *m_redis;
	FirstBuyItem *m_pFirstBuyItem;
	map<string, REDISDBName *>m_RedisDBNames;
	map<int, Prop >m_props;
	map<int, Reward>m_rewards;
	map<string, map<int, GateData *>> m_pGates;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__