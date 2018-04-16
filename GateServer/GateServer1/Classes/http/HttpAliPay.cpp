﻿#include "HttpAliPay.h"
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
	m_aliprivatekey = ALIPRIKEY;
	m_alipubkey = ALIPUBKEY;
	
}
HttpAliPay::~HttpAliPay(){
	
}

bool HttpAliPay::init()
{
	setPrivateKey(m_aliprivatekey);
	setPubKey(m_alipubkey);
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

SAliPayOrder HttpAliPay::requestOrder(string uid, string shopid, float price, string body, string ip){
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));

	JsonMap contentMap;
	contentMap.insert(JsonMap::value_type(JsonType("out_trade_no"), JsonType("20160606121212")));
	contentMap.insert(JsonMap::value_type(JsonType("total_amount"), JsonType(price)));
	contentMap.insert(JsonMap::value_type(JsonType("subject"), HttpEvent::getIns()->GBKToUTF8(body.c_str())));
	string content = JsonUtil::objectToString(contentMap);

	map<string,string> requestPairs;
	requestPairs.insert(make_pair("app_id", ALIAPPID));
	requestPairs.insert(make_pair("method", "alipay.trade.precreate"));
	requestPairs.insert(make_pair("charset", "utf-8"));
	requestPairs.insert(make_pair("biz_content", content));
	requestPairs.insert(make_pair("sign_type", "RSA2"));
	requestPairs.insert(make_pair("timestamp", tmp));
	requestPairs.insert(make_pair("version", "1.0"));
	
	string signA = XXIconv::GBK2UTF(XmlConfig::getIns()->setJsonData(requestPairs).c_str());
	
	string sign = rsaSign(signA,m_aliprivatekey);
	requestPairs.insert(make_pair("sign", sign));
	
	HttpClient httpClient;
	string responseStr = httpClient.sendSyncRequest(ALIPAYURL, requestPairs);

	DebugLog("Response:%s", responseStr.c_str());

	string responseContent = analyzeResponse(responseStr);
	
	//传给客户端
	contentMap.insert(JsonMap::value_type(JsonType("product_code"), JsonType("QUICK_MSECURITY_PAY")));
	contentMap.insert(JsonMap::value_type(JsonType("timeout_express"), JsonType("30m")));

	

	string con = JsonUtil::objectToString(contentMap);
	map<string, string>vv;
	vv.insert(make_pair("biz_content", con));
	SAliPayOrder spo;
	spo.set_appid(ALIAPPID);
	spo.set_timestamp(tmp);
	spo.set_orderinfo(con);
	spo.set_privatekey(m_aliprivatekey);
	return spo;
}

string HttpAliPay::buildContent(const map<string, string> &contentPairs) {

	string content;
	for (auto iter = contentPairs.begin();
		iter != contentPairs.end(); ++iter) {
		if (!content.empty()) {
			content.push_back('&');
		}
		content.append(iter->first);
		content.push_back('=');
		content.append(iter->second);
	}
	return content;
}

string HttpAliPay::analyzeResponse(const string &responseStr) {

	JsonType responseObj = JsonUtil::stringToObject(responseStr);
	JsonMap responseMap = responseObj.toMap();
	//获取返回报文中的alipay_xxx_xxx_response的内容;
	int beg = responseStr.find("_response\"");
	int end = responseStr.rfind("\"sign\"");
	if (beg < 0 || end < 0) {
		return string();
	}
	beg = responseStr.find('{', beg);
	end = responseStr.rfind('}', end);
	
	string responseContent = responseStr.substr(beg, end - beg + 1);

	if (!m_aliprivatekey.empty()) {

		JsonMap::const_iterator iter = responseMap.find("sign");
		if (iter == responseMap.end()) {
			DebugLog("Cannot get Sign from response, Verify Failed");
			return string();
		}
		//获取返回报文中的sign;
		string responseSign = iter->second.toString();

		//调用验签方法;
		bool verifyResult = HttpAliPay::rsaVerify(responseContent, responseSign, m_alipubkey);

		if (!verifyResult) {
			DebugLog("Verify Failed");
			return string();
		}
	}
	else {
		DebugLog("AlipayPublicKey is empty, Skip the Verify");
	}

	return responseContent;
}

string HttpAliPay::rsaSign(const string &content, const string &key) {

	string signed_str;
	int key_len = key.length();
	BIO *p_key_bio = BIO_new_mem_buf((void *)key.c_str(), key_len);

	RSA *p_rsa = PEM_read_bio_RSAPrivateKey(p_key_bio, NULL, NULL, NULL);

	if (p_rsa != NULL) {
		unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };
		SHA256((unsigned char *)content.c_str(), content.length(), hash);
		unsigned char sign[XRSA_KEY_BITS / 8] = { 0 };
		unsigned int sign_len = sizeof(sign);
		int r = RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, sign, &sign_len, p_rsa);

		if (0 != r && sizeof(sign) == sign_len) {
			signed_str = base64Encode(sign, sign_len);
		}
	}

	RSA_free(p_rsa);
	BIO_free(p_key_bio);
	return signed_str;
}

bool HttpAliPay::rsaVerify(const string &content, const string &sign, const string &key) {

	bool result = false;
	const char *key_cstr = key.c_str();
	int key_len = strlen(key_cstr);
	BIO *p_key_bio = BIO_new_mem_buf((void *)key_cstr, key_len);
	RSA *p_rsa = PEM_read_bio_RSA_PUBKEY(p_key_bio, NULL, NULL, NULL);
	if (p_rsa != NULL) {
		const char *cstr = content.c_str();
		unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };
		SHA256((unsigned char *)cstr, strlen(cstr), hash);
		unsigned char sign_cstr[XRSA_KEY_BITS / 8] = { 0 };
		int len = XRSA_KEY_BITS / 8;
		base64Decode(sign, sign_cstr, len);
		unsigned int sign_len = XRSA_KEY_BITS / 8;
		int r = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, (unsigned char *)sign_cstr, sign_len, p_rsa);

		if (r > 0) {
			result = true;
		}
	}

	RSA_free(p_rsa);
	BIO_free(p_key_bio);
	return result;
}


string HttpAliPay::setPrivateKey(string &key){
	int len = key.length();
	string tp = "-----BEGIN RSA PRIVATE KEY-----\n";
	while (len >= 64){
		string tt = key.substr(0, 64);
		//printf("tt:%02d--[%s]\n",tt.length(),tt.c_str());
		tp += tt + "\n";
		key = key.substr(64, key.length());
		len = key.length();
	}
	tp += key + "\n-----END RSA PRIVATE KEY-----\n";
	key = tp;
	return key;
}

string HttpAliPay::setPubKey(string &key){
	int len = key.length();
	string tp = "-----BEGIN PUBLIC KEY-----\n";
	while (len >= 64){
		tp += key.substr(0, 64) + "\n";
		key = key.substr(64, key.length());
		len = key.length();
	}
	tp += key + "\n-----END PUBLIC KEY-----\n";
	key = tp;
	return key;
}

string HttpAliPay::base64Encode(const unsigned char *bytes, int len) {

	BIO *bmem = NULL;
	BIO *b64 = NULL;
	BUF_MEM *bptr = NULL;

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, bytes, len);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	string str = string(bptr->data, bptr->length);
	BIO_free_all(b64);
	return str;
}

bool HttpAliPay::base64Decode(const string &str, unsigned char *bytes, int &len) {

	const char *cstr = str.c_str();
	BIO *bmem = NULL;
	BIO *b64 = NULL;

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new_mem_buf((void *)cstr, strlen(cstr));
	b64 = BIO_push(b64, bmem);
	len = BIO_read(b64, bytes, len);

	BIO_free_all(b64);
	return len > 0;
}
