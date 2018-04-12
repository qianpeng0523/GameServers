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
	string *result = (string *)stream;
	result->append((char *)buffer);
	return size*nmemb;
}

void HttpWXLogin::requestAccessToken(string code, struct evhttp_request *req){
	//通过code获取AccessToken
	char buff[500];
	sprintf(buff, "https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", APPID, APPSECRET,code.c_str());
	string content= HttpEvent::getIns()->requestData(buff, "", Code_Access_Token);
	respondAccessToken(content,req);
}
/******
1.通过code获取access_token发送给客户端
  1）客户端通过access_token授权后得到refresh_token （https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code）
  2）客户端把refresh_token发送给服务器
  3）服务器校验refresh_token是否有效（http请求方式: GET https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=APPID&grant_type=refresh_token&refresh_token=REFRESH_TOKEN）
    1】有效则获取到access_token和openid，通过openid查找数据中是否有userinfo
	如果没有则通过access_token和openid获取微信的userinfo（http请求方式: GET https://api.weixin.qq.com/sns/auth?access_token=ACCESS_TOKEN&openid=OPENID）
	如果有则直接返回userinfo数据
	2】无效则告诉客户端refresh_token失效，客户端重复1操作
2.通过refresh_token
  1）重复上面的3步骤

******/



void HttpWXLogin::respondAccessToken(string result, struct evhttp_request *req){
	//获取的是json
	YMSocketData sd(result);
	printf("sd:%s\n",sd.getJsonString().c_str());
	if (sd.isMember("errcode")==0){
		string errcode = sd["errcode"].asString();
		if (errcode.compare("0") == 0){
			//请求成功
			string access_token = sd["access_token"].asString();
			string refresh_token = sd["refresh_token"].asString();
			//获取到了access_token和refresh_token
			string openid = sd["openid"].asString();//需要和access_token与uid绑定起来
			//返回给用户access_token，用户通过access_token登录
			//如果openid不存在则为注册,存在未登录
		}
		else{
			string errmsg = sd["errmsg"].asString();
			printf("%s\n",errmsg.c_str());
		}
	}

}