#ifndef __LYNX_GAME_SERVER_RedisGet_H__
#define __LYNX_GAME_SERVER_RedisGet_H__
#include "stdafx.h"
#include "Common.h"
#include "CSVDataInfo.h"

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

struct AliPayNoData :public Object
{
	string _uid;
	string _out_trade_no;
	time_t _endtime;
	int _shopid;
	map<string, string> _maps;
};

struct WXPayNoData
{
	string _uid;
	string _out_trade_no;
	time_t _endtime;
};

typedef map<string, UserBase *> UserBaseMap;
typedef map<string, map<string,Friend *>> FriendMap;



class RedisGet :public Object
{
public:
	RedisGet();
	virtual ~RedisGet();
	
	static RedisGet *getIns();
	void init();

	map<string, REDISDBName *> getREDISDBNames();
	bool SelectDB(REDISTYPE type);
	int getRedisDBIndex(string name);

	map<string, SConfig *> getSConfig();
	SConfig *getSConfig(string uid);
	void setSConfig(string uid, SConfig *sc);

	string getPass(string uid);
	map<string, string> getPass();
	void setPass(string uid,string pass);

	map<string, string> getOpenids();
	string getOpenidPass(string openid);
	void setOpenidPass(string openid,string uid);

	UserBaseMap getUserBases();
	UserBase *getUserBase(string uid);
	void setUserBase(UserBase *ub);

	int getUserIndex();

	void setUserLoginTime(string uid,time_t t);
	time_t getUserLoginTime(string uid);
	map<string, time_t> getUserLoginTimes();

	map<int, vector<Rank *>> getRanks();
	vector<Rank *>getRank(int type);
	void setRank(Rank *rk,int type);

	map<int, ShopItem*> getShop();
	ShopItem *getShop(int shopid);
	FirstBuyItem *getFirstBuy();
	string getNoTime();
	string getAliOuttradeNo();
	map<string, AliPayNoData *> getAliPayNoDatas();
	AliPayNoData *getAliPayNoData(string out_trade_no);
	void eraseAliPayNoData(string out_trade_no);
	void setAliPayNoData(AliPayNoData *p);
	void setPayRecord(PayRecord *pr);
	map<string, map<string, PayRecord *>> getPayRecords();
	
	string getWXNonceid();
	void setWXNonceid(int nonceid);
	map<string, WXPayNoData *> getWXPayNoDatas();
	WXPayNoData *getWXPayNoData(string outno);
	void setWXPayNoData(WXPayNoData *p);
	void eraseWXPayNoData(WXPayNoData *p);

	map<string, map<int, Mail *>>getMails();
	map<int, Mail *> getMail(string uid);
	Mail *getMail(string uid, int eid);
	void setMail(string uid,Mail *ml);
	void eraseMail(string uid,Mail *ml);

	FriendMap getFriend();
	map<string, Friend *> getFriend(string uid);
	Friend *getFriend(string uid, string fuid);
	void setFriend(string uid,Friend *fri,bool isadd);

	map<string, map<string, vector<FriendChat *>>> getFriendChats();
	map<string, vector<FriendChat *>>getFriendChat(string uid);
	vector<FriendChat *>getFriendChat(string uid,string fuid);
	void setFriendChat(string suid, FriendChat *fc);
	void eraseFriendChat(string suid, FriendChat fc);

	map<string, map<int, FriendNotice *>> getFriendNotices();
	void setFriendNotice(string uid,FriendNotice *p);
	void eraseFriendNotice(string uid, FriendNotice *p);
	map<int, FriendNotice *> getFriendNotice(string uid);
	FriendNotice *getFriendNotice(string uid,string fuid);
	FriendNotice *getFriendNotice(string uid, int nid);

	map<int, Active *> getActive(int type);
	map<int, map<int, Active *>> getActives();

	map<int, Task *> getTask();
	Task *getTask(int tid);
	Status *getTaskStatus(string uid, int taskid);//需要删除
	map<string, map<int, Status *>> getTaskStatus();
	map<int, Status *> getTaskStatus(string uid);
	void setTaskStatus(string uid,int taskid,Status *ss);

	map<int, ExAward *> getExAward();
	ExAward *getExAward(int id);

	map<int,Reward *> getReward();
	Reward *getReward(int rid);

	map<string, map<int, ExRecord *>> getExRecords();
	map<int, ExRecord *> getExRecord(string uid);
	void PushExRecord(string uid,ExRecord *p);

	map<int, SignAward*> getSignAward();
	SignStatus *getSignStatus(string uid);
	void setSignStatus(SignStatus *ss);//不写入数据库
	map<string, SignStatus *> getSignStatuss();

	map<int, SignZhuan *> getSignZhuan();
	map<int,Prop *> getProp();
	Prop *getProp(int id);

	map<int, Task*> getFree();
	map<string, map<int, Status *>> getFreeStatus();
	map<int, Status *> getFreeStatus(string uid);
	Status *getFreeStatus(string uid,int fid);
	void setFreeStatus(string uid,int fid,Status *ss);

	string getExchangeCode();
	int getExchangeRecordId(string uid);

	map<string, PExchangeCode*> getCSVExchangeCode();
	PExchangeCode* getPExchangeCode(string code);
	void erasePExchangeCode(string code);

	int getMailID();
	int getFriendNoticeID();
private:
	map<int, FriendNotice *> getFriendNoticeB(string uid);

	void sortRank();
	void update(float);
private:
	static RedisGet *m_ins;
	redis *m_redis;
	map<int, Prop *> m_pProps;
	map<int, Reward*> m_pRewards;

	map<int,ShopItem*> m_pShopItems;
	FirstBuyItem *m_pFirstBuyItem;

	map<int,Task*> m_tasks;
	map<string, map<int, Status *>> m_taskstatus;

	map<int,SignZhuan *> m_pSignZhuans;
	map<int,SignAward *> m_pSignAwards;
	map<string, SignStatus*>m_pSignStatuss;

	map<string, PExchangeCode*> m_pExchangeCodes;
	map<string, map<int, ExRecord *>> m_pExRecords;
	map<int,ExAward *> m_pExAwards;

	map<int, map<int, Active *>> m_pActives;

	map<int, Task*> m_pFrees;
	map<string, map<int, Status *>> m_pFreeStatus;

	map<int, vector<Rank *>> m_pRanks;
	map<string, map<int, Mail *>> m_pMails;

	map<string, SConfig *>m_pSConfigs;
	
	FriendMap m_pfriends;
	map<string, map<int, FriendNotice *>>m_pFriendNotices;

	UserBaseMap m_pUserBases;
	map<string, time_t> m_pUserLoginTime;
	map<string, string>m_pPass;
	map<string, string>m_pOpenids;

	map<string, AliPayNoData *>m_pAliPayNoDatas;
	map<string, map<string, PayRecord *>> m_pPayRecords;
	map<string, WXPayNoData *> m_pWXPayNoDatas;

	map<string, REDISDBName *>m_RedisDBNames;
	int m_wxnonceid;
	map<string, map<string, vector<FriendChat *>>> m_pFriendChats;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__