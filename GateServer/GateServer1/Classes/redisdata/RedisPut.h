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

	bool PushConfig(string uid, SConfig *sc);
	bool setConfig(string uid, POINTTIP type, bool ist);

	bool PushUserBase(UserBase *ub);
	bool setUserBase(string uid, string key, string value);
	bool addUserBase(string uid, string key, int value);

	bool PushUserLoginTime(string uid);

	bool PushPass(string uid, string pass);
	bool PushOpenid(string openid, string uid);

	bool PushRank(Rank rk);

	bool PushMail(string uid,Mail mail);
	void PopMail(string uid, Mail ml);

	bool PushFriend(string uid,string friuid);
	bool PushFriendNotice(string uid, FriendNotice *fn);
	bool eraseFriendNotice(string uid, FriendNotice *fn);
	
	bool PushTaskStatus(string uid, int taskid, Status status);

	bool PushExRecord(string uid,ExRecord er);
	bool setEXCode(string code, bool isdui);
	
	bool setSignStatus(int sid, SignStatus *ss);
	
	bool PushFeedBack(CFeedBack cfb);

	void ZeroChange(char *&data, int sz);
private:
	static RedisPut *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisPut_H__