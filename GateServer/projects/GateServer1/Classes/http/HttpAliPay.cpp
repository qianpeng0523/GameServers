#include "HttpAliPay.h"
#include "HttpEvent.h"
#include "XmlConfig.h"
#include "redis.h"
#include "Common.h"
#include "StatTimer.h"


HttpAliPay *HttpAliPay::m_Ins = NULL;

HttpAliPay::HttpAliPay(){
	m_pRedis = redis::getIns();
	m_pRedisGet = RedisGet::getIns();
	m_pRedisPut = RedisPut::getIns();
	
}
HttpAliPay::~HttpAliPay(){
	
}

bool HttpAliPay::init()
{
	
    return true;
}

HttpAliPay *HttpAliPay::getIns(){
	if (!m_Ins){
		m_Ins = new HttpAliPay();
		m_Ins->init();
	}
	return m_Ins;
}

size_t read_data2(void* buffer, size_t size, size_t nmemb, void *stream)
{
	string *result = (string *)stream;
	result->append((char *)buffer);
	return size*nmemb;
}

void HttpAliPay::requestOrder(string uid, string shopid, int price, string body, string ip){
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));

	map<string,string> requestPairs;
	requestPairs.insert(make_pair("app_id", ALIAPPID));
	requestPairs.insert(make_pair("method", "alipay.trade.precreate"));
	requestPairs.insert(make_pair("charset", "utf-8"));
	requestPairs.insert(make_pair("biz_content", ALIAPPID));
	requestPairs.insert(make_pair("sign_type", "RSA2"));
	requestPairs.insert(make_pair("timestamp", tmp));
	requestPairs.insert(make_pair("version", "1.0"));
	requestPairs.insert(make_pair("out_trade_no","20160606121212"));
	requestPairs.insert(make_pair("total_amount","0.01"));
	requestPairs.insert(make_pair("subject","好东西"));
	string signA = XXIconv::GBK2UTF(XmlConfig::getIns()->setJsonData(requestPairs).c_str());
	MD5 md5;
	md5.update(signA);
	string sign = md5.toString();
	printf("sign:%s",sign.c_str());
	requestPairs.insert(make_pair("sign", sign));
	
	string content = XXIconv::GBK2UTF(XmlConfig::getIns()->setJsonData(requestPairs).c_str());
	string result = HttpEvent::getIns()->requestData(ALIPAYURL, content.c_str(), read_data2);
	printf("%s\n", result.c_str());
}