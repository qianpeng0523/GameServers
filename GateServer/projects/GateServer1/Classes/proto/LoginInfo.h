
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

	void SendCWXLogin();
	void HandlerSWXLogin(string code, string access_token);


private:
	static LoginInfo *m_shareLoginInfo;
	RedisPut *m_pRedisPut;
	RedisGet *m_pRedisGet;
};

#endif 
