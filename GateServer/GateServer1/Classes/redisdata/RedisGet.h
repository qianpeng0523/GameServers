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
		//_time = Common::getLocalTimeDay1();
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
	void setFriend(string uid,string fuid,bool isadd);

	vector<FriendNotice > getFriendNotice(string uid);
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
	SignStatus getSignStatus(string uid);
	void setSignStatus(SignStatus ss);//不写入数据库

	SConfig *getSConfig(string uid);
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
	map<string, SignStatus>m_pSignStatuss;
	map<string, vector<char *>>m_pfriends;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__