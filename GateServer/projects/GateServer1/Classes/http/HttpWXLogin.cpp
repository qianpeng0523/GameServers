#include "HttpWXLogin.h"
#include "HttpEvent.h"
#include "XmlConfig.h"
#include "redis.h"
#include "Common.h"
#include "StatTimer.h"

HttpWXLogin *HttpWXLogin::m_Ins = NULL;

HttpWXLogin::HttpWXLogin(){
	
}
HttpWXLogin::~HttpWXLogin(){
	
}

bool HttpWXLogin::init()
{
	
    return true;
}

HttpWXLogin *HttpWXLogin::getIns(){
	if (!m_Ins){
		m_Ins = new HttpWXLogin();
		m_Ins->init();
	}
	return m_Ins;
}

size_t Code_Access_Token(void* buffer, size_t size, size_t nmemb, void *stream)
{
	vector<char> *vec = (vector<char> *)stream;
	for (int i = 0; i < size*nmemb; i++){
		vec->push_back(((char*)buffer)[i]);
	}
	return size*nmemb;
}

void HttpWXLogin::requestAccessToken(string code){
	//通过code获取AccessToken
	char buff[500];
	sprintf(buff, "https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", APPID, APPSECRET,code.c_str());
	string content= HttpEvent::getIns()->requestData(buff, "", Code_Access_Token);
}