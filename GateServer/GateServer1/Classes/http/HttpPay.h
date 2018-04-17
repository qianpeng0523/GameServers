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
	
	SWxpayOrder requestOrder(string uid, string shopid, int price, string body, string ip);
	SWxpayOrder respondOrder(string content, map<string, string> ordermap);

	void respondResult(string content,struct evhttp_request *req=NULL);

	bool requestCheck(string xml);
	bool respondCheck(string content);

	bool requestCheckKH(string transaction_id, bool traid=false);
public:
	void test();
	void update(float dt);
	void openUpdate(bool isopen);
	string createSign(map<string, string> valuemap);
	string getOutTradeNo();
	string getNonceId();
	void checkPay();
	
	void NoticePushCurrency(Reward rd,string uid);
	static string payrecord[9];
private:
	static HttpPay *m_Ins;
	bool m_isopen;
	string m_lasttime;
	redis *m_pRedis;
	int m_count;
	
};

#endif 