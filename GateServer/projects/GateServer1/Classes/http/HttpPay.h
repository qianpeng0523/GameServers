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
	void respond(map<string, string> maps, map<string, string> ordermap);
public:
	void test();
	void update(float dt);
	void openUpdate(bool isopen);
	string createSign(map<string, string> valuemap);
	string getOutTradeNo();
	string getNonceId();
	
private:
	static HttpPay *m_Ins;
	bool m_isopen;
	string m_lasttime;
public:
	
};

#endif 