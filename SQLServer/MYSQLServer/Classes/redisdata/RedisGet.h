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

	char* getPass(string uid);
	UserBase *getUserBase(string uid);//需要删除
	std::vector<Rank >getRank(int type,int index);

	std::vector<ShopItem >getShop();
	vector<Mail> getMail(string uid);
	vector<char *> getFriend(string uid);
	vector<FriendNotice > getFriendNotice(string uid);
	vector<Active > getActive(int type);
	vector<Task > getTask();
	Status *getTaskStatus(string uid, int taskid);//需要删除
	vector<ExAward> getExAward();
	vector<Reward> getReward();
	Reward getReward(int rid);
	vector<ExRecord> getExRecord(string uid);
	int setExRecordStatus(string uid, int rid);
	vector<SignAward> getSignAward();
	int getSignStatus(string uid,int signid);
	SConfig *getSConfig(string uid);
	vector<SignZhuan> getSignZhuan();
	vector<Prop > getProp();
	vector<Task > getFree();
	FirstBuyItem *getFirstBuy();
		
private:
	static RedisGet *m_ins;
	redis *m_redis;
	FirstBuyItem *m_pFirstBuyItem;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__