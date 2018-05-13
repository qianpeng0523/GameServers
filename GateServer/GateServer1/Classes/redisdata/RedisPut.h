#ifndef __LYNX_GAME_SERVER_RedisPut_H__
#define __LYNX_GAME_SERVER_RedisPut_H__
#include "stdafx.h"
#include "RedisGet.h"
using namespace std;


class RedisPut 
{
public:
	RedisPut();
	virtual ~RedisPut();
	
	static RedisPut *getIns();
	void init();

	bool PushConfig(string uid,SConfig scf);
	bool PushPass(string uid,string pass);
	bool PushUserBase(UserBase ub);
	bool setUserBase(string uid, string key, string value);
	bool addUserBase(string uid, string key, int value);
	bool PushRank(Rank rk);
	bool PushShop(ShopItem item);
	bool PushMail(string uid,Mail mail);
	bool setMailStatus(string uid,int mid,int status);
	bool PushFriend(string uid,string friuid);
	bool PushFriendNotice(string uid, FriendNotice fn);
	bool PushActive(Active at);
	bool PushTask(Task task);
	bool PushTaskStatus(string uid, int taskid, Status status);
	bool PushExAward(ExAward ea);
	bool PushReward(Reward rd);
	bool PushExRecord(string uid,ExRecord er);
	bool setExRecordStatus(string uid,int rid,int status);
	bool PushSignAward(SignAward sa);
	bool setSignStatus(SignStatus ss);
	
	bool PushFeedBack(CFeedBack cfb);

	bool setSConfig(string uid, SConfig sc);
	bool PushSignZhuan(SignZhuan sz);
	bool PushProp(Prop p);
	bool PushFree(Task task);

	bool setMailID(int mid);
	void PopMail(string uid, Mail ml);
	void ZeroChange(char *&data, int sz);

	bool setConfig(string uid,POINTTIP type,bool ist);
	
	bool setExchangeCode(string code);
	bool setExchangeRecordId(string uid,int id);
	bool setEXCode(string code,bool isdui);
	bool changeEXCode(string code,bool isdui);
private:
	static RedisPut *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisPut_H__