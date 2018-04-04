#ifndef __LYNX_GAME_SERVER_RedisGet_H__
#define __LYNX_GAME_SERVER_RedisGet_H__
#include "stdafx.h"

using namespace std;


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
	map<string,Mail> getMail(string uid);
	Mail getMail(string uid, int eid);
	int getMailStatus(string uid, int mid);
	vector<char *> getFriend(string uid);
	vector<FriendNotice > getFriendNotice(string uid);
	vector<Active > getActive(int type);
	vector<Task > getTask();
	Status *getTaskStatus(string uid, int taskid);//需要删除
	vector<ExAward> getExAward();
	map<int,Reward> getReward();
	Reward getReward(int rid);
	vector<ExRecord> getExRecord(string uid);
	int setExRecordStatus(string uid, int rid);
	vector<SignAward> getSignAward();
	int getSignStatus(string uid,int signid);
	SConfig *getSConfig(string uid);
	vector<SignZhuan> getSignZhuan();
	map<int,Prop > getProp();
	Prop getProp(int id);
	vector<Task > getFree();
private:
	static RedisGet *m_ins;
	redis *m_redis;
	map<int, Prop > m_pProps;
	map<int, Reward> m_pRewards;
	vector<ShopItem> m_pShopItems;
	vector<Task> m_tasks;
	vector<SignZhuan> m_pSignZhuans;
	vector<SignAward> m_pSignAwards;
	vector<ExAward> m_pExAwards;
	vector<Active> m_pActives;
	vector<Task> m_pFrees;
	map<int,vector<Rank >> m_pRanks;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__