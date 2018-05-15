
#ifndef __LoginInfo__
#define __LoginInfo__

#include "stdafx.h"
#include "RedisPut.h"
#include "RedisGet.h"

class LoginInfo:public Object
{
public:
	LoginInfo();
	~LoginInfo();
	static LoginInfo* getIns();
	bool init();
public:
	void SendSLogin(SLogin cl, int fd);
	void HandlerCLoginHand(ccEvent *event);
	void SendSRegister(SRegister sd, int fd);
	void HandlerCRegister(ccEvent *event);
	
	void Check(float dt);
	void openCheckUpdate(bool isopen);

	void SendSWXLogin(SWXLogin sl, int fd);
	void HandlerCWXLogin(ccEvent *event);

	
	void PushUserBase(UserBase *user);

	UserBase *getUserBase(string uid);
	void eraseUserBase(string uid,bool online);
	void inserUserBase(string,UserBase *ub,bool online);
	void onLineUsers();
	map<string, UserBase *> getOnLineUser(bool online);
public:
	void eraseClientData(int fd);
private:
	static LoginInfo *m_shareLoginInfo;
	RedisPut *m_pRedisPut;
	RedisGet *m_pRedisGet;
	map<string, UserBase *>m_UserBases;
	map<string, UserBase *>m_pOnlineUsers;
	map<string, UserBase *>m_pOfflineUsers;
};

#endif 
