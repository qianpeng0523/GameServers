#ifndef __HttpAliPay_SCENE_H__
#define __HttpAliPay_SCENE_H__

#include "stdafx.h"
#include "RedisGet.h"
#include "RedisPut.h"
#include "http/http_client.h"
#include "json/json_util.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <time.h>

#ifndef XRSA_KEY_BITS
#define XRSA_KEY_BITS (2048)
#endif



class HttpAliPay:public Object
{
public:
	HttpAliPay();
	~HttpAliPay();
    virtual bool init();
	static HttpAliPay *getIns();
	
	SAliPayOrder requestOrder(string uid, string shopid, float price, string body, string ip);
	void respondResult(string content, struct evhttp_request *req = NULL);

	void update(float dt);
	void openUpdate(bool isopen);
private:
	static string base64Encode(const unsigned char *bytes, int len);
	static bool base64Decode(const string &str, unsigned char *bytes, int &len);

	static string rsaSign(const string &content, const string &key);
	static bool rsaVerify(const string &content, const string &sign, const string &key);

	string buildContent(const map<string,string> &contentPairs);
	string analyzeResponse(const string &responseStr);

	string setPrivateKey(string &key);
	string setPubKey(string &key);
	string getOutTradeNo();
private:
	static HttpAliPay *m_Ins;
	redis *m_pRedis;
	RedisGet *m_pRedisGet;
	RedisPut *m_pRedisPut;
	string m_aliprivatekey;
	string m_alipubkey;
	bool m_isopen;
	string m_lasttime;
	int m_count;
};

#endif 