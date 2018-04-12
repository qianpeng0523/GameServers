#ifndef __HttpWXLogin_SCENE_H__
#define __HttpWXLogin_SCENE_H__

#include "stdafx.h"
#include "RedisGet.h"
#include "RedisPut.h"

class HttpWXLogin:public Object
{
public:
	HttpWXLogin();
	~HttpWXLogin();
    virtual bool init();
	static HttpWXLogin *getIns();
	
	UserBase requestWXLogin(string code,string token);
public:
	UserBase requestAccessToken(string code);
	UserBase respondAccessToken(string result);

	UserBase requestRefreshToken(string refreshtoken);
	UserBase respondRefreshToken(string result);

	UserBase requestUserinfo(string acctoken, string openid);
	UserBase respondUserinfo(string result);
private:
	static HttpWXLogin *m_Ins;
	redis *m_pRedis;
	RedisGet *m_pRedisGet;
	RedisPut *m_pRedisPut;
	int m_uidindex;
};

#endif 