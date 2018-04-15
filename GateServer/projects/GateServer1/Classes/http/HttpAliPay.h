#ifndef __HttpAliPay_SCENE_H__
#define __HttpAliPay_SCENE_H__

#include "stdafx.h"
#include "RedisGet.h"
#include "RedisPut.h"

class HttpAliPay:public Object
{
public:
	HttpAliPay();
	~HttpAliPay();
    virtual bool init();
	static HttpAliPay *getIns();
	
	void requestOrder(string uid, string shopid, int price, string body, string ip);
public:
	
private:
	static HttpAliPay *m_Ins;
	redis *m_pRedis;
	RedisGet *m_pRedisGet;
	RedisPut *m_pRedisPut;
};

#endif 