#ifndef __HttpPay_SCENE_H__
#define __HttpPay_SCENE_H__

#include "stdafx.h"

class HttpPay:public Object
{
public:
	HttpPay();
	~HttpPay();
    virtual bool init();
	static HttpPay *getIns();
	
	void requestOrder(string uid,string shopid,int price, string body, string ip);
	void respondOrder(string content, map<string, string> ordermap);

	void respondResult(string content,struct evhttp_request *req=NULL);

	void requestCheck(string xml);
	void respondCheck(string content);
public:
	void test();
	void update(float dt);
	void openUpdate(bool isopen);
	string createSign(map<string, string> valuemap);
	string getOutTradeNo();
	string getNonceId();
	void checkPay();
	
private:
	static HttpPay *m_Ins;
	bool m_isopen;
	string m_lasttime;
	redis *m_pRedis;
	int m_count;
	
};

#endif 