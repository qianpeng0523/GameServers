#ifndef __LYNX_GAME_SERVER_RedisGet_H__
#define __LYNX_GAME_SERVER_RedisGet_H__
#include "stdafx.h"
#include "Common.h"
using namespace std;

struct SignStatus
{
	SignStatus(){
		_signcount = 0;
		_issign = false;
		_left = 1;
		_time = Common::getLocalTimeDay1();
	}
	string _uid;
	int _signcount;
	bool _issign;
	int _left;
	string _time;//日期
};

struct FirstBuyItem :public Object
{
	int _sid;
	vector<int> _rid;
	int _conid;
	vector<int> _giveid;
};

struct PExchangeCode :public Object
{
	int _id;
	vector<int> _rewardid;
	string _code;
	bool _isdui;
};

typedef map<string, UserBase *> UserBaseMap;
typedef map<string, UserBaseMap> FriendMap;

class RedisGet 
{
public:
	RedisGet();
	virtual ~RedisGet();
	
	static RedisGet *getIns();
	void init();

	bool SelectDB(REDISTYPE type);
	int getRedisDBIndex(string name);

	map<string, SConfig *> getSConfig();
	SConfig *getSConfig(string uid);
	void setSConfig(string uid, SConfig *sc);

	string getPass(string uid);
	map<string, string> getPass();
	void setPass(string uid,string pass);

	string getOpenidPass(string openid);
	UserBaseMap getUserBases();
	UserBase *getUserBase(string uid);
	UserBase *getUserBase(int index);
	void setUserBase(UserBase *ub);
	int getUserBaseIndex(string uid);
	void setUserLoginTime(string uid,time_t t);
	time_t getUserLoginTime(string uid);

	map<string, string> getOpenids();
	

	std::vector<Rank >getRank(int type,int index);

	std::vector<ShopItem >getShop();
	map<string,Mail> getMail(string uid);
	Mail getMail(string uid, int eid);
	int getMailStatus(string uid, int mid);

	FriendMap getFriend();
	UserBaseMap getFriend(string uid);
	UserBase *getFriend(string uid,string fuid);
	void setFriend(string uid,string fuid,bool isadd);

	map<string, map<int, FriendNotice *>> getFriendNotices();
	void setFriendNotice(string uid,FriendNotice *p);
	void eraseFriendNotice(string uid, FriendNotice *p);
	map<int, FriendNotice *> getFriendNotice(string uid);
	FriendNotice *getFriendNotice(string uid,string fuid);
	int getFriendNoticeID();
	FriendNotice *getFriendNotice(string uid, int nid);
	int getFriendNoticeIndex(string uid,int nid);

	map<string, bool> getFriendGiveB(string uid);
	map<string, bool> getFriendGive(string uid);
	map<string, map<string, bool>> getFriendGive();
	void setFriendGive(string uid,string fruid,bool have);
	int getFriendGiveIndex(string uid,string friuid);
	void setFriendGiveIndex(string uid,string friuid,int index);

	vector<Active > getActive(int type);
	vector<Task > getTask();
	Status *getTaskStatus(string uid, int taskid);//需要删除
	vector<ExAward> getExAward();
	ExAward getExAward(int id);

	map<int,Reward> getReward();
	Reward getReward(int rid);
	vector<ExRecord> getExRecord(string uid);
	void PushExRecord(string uid,ExRecord p);

	int setExRecordStatus(string uid, int rid);
	vector<SignAward> getSignAward();
	SignStatus *getSignStatus(string uid);
	void setSignStatus(SignStatus *ss);//不写入数据库
	map<string, SignStatus *> getSignStatuss();

	vector<SignZhuan> getSignZhuan();
	map<int,Prop > getProp();
	Prop getProp(int id);
	vector<Task > getFree();

	ShopItem getShop(int shopid);
	FirstBuyItem *getFirstBuy();
	int MailID();
	
	string getExchangeCode();
	int getExchangeRecordId(string uid);

	map<string, PExchangeCode*> getCSVExchangeCode();
	PExchangeCode* getPExchangeCode(string code);

	vector<bool> getExcode();
	bool getExcode(string code);
	void setEXCodeStatus(int id, bool ist);//不记录到数据库中
private:
	map<int, FriendNotice *> getFriendNoticeB(string uid);
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
	map<string, SConfig *>m_pSConfigs;
	FirstBuyItem *m_pFirstBuyItem;
	map<string, PExchangeCode*> m_pExchangeCodes;
	vector<bool> m_pEXCodes;
	map<string, vector<ExRecord>> m_pExRecords;
	map<string, SignStatus*>m_pSignStatuss;
	map<string, map<string, bool>>m_pfriendgives;
	map<string, map<string, int>>m_pfriendgiveindexs;
	FriendMap m_pfriends;
	UserBaseMap m_pUserBases;
	map<string, int>m_pUserIndexs;
	map<string, time_t> m_pUserLoginTime;
	map<string, string>m_pPass;
	map<string, string>m_pOpenids;
	map<string, map<int,FriendNotice *>>m_pFriendNotices;

	map<string, REDISDBName *>m_RedisDBNames;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__