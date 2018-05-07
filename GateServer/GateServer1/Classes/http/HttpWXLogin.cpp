#include "HttpWXLogin.h"
#include "HttpEvent.h"
#include "XmlConfig.h"
#include "redis.h"
#include "Common.h"
#include "StatTimer.h"

HttpWXLogin *HttpWXLogin::m_Ins = NULL;

HttpWXLogin::HttpWXLogin(){
	m_pRedis = redis::getIns();
	m_pRedisGet = RedisGet::getIns();
	m_pRedisPut = RedisPut::getIns();
	int len = 0;
	char *index1 = m_pRedis->get("userid_index",len);
	if (!index1){
		index1 = "10000000";
		m_pRedis->set("userid_index",index1,len);
	}
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

UserBase HttpWXLogin::requestWXLogin(string code, string &token){
	if (!token.empty()){
		return requestRefreshToken(token,token);
	}
	else if(!code.empty()){
		return requestAccessToken(code,token);
	}
	else{
		UserBase ub;
		return ub;
	}
}

size_t Code_Access_Token(void* buffer, size_t size, size_t nmemb, void *stream)
{
	string *result = (string *)stream;
	result->append((char *)buffer);
	return size*nmemb;
}

UserBase HttpWXLogin::requestAccessToken(string code, string &token){
	//通过code获取AccessToken
	char buff[500];
	sprintf(buff, "https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", APPID, APPSECRET,code.c_str());
	string content= HttpEvent::getIns()->requestData(buff, "", Code_Access_Token);
	return respondAccessToken(content,token);
}
/******
1.通过code获取access_token发送给客户端（https://api.weixin.qq.com/sns/oauth2/access_token?appid=APPID&secret=SECRET&code=CODE&grant_type=authorization_code）
  1）客户端通过access_token授权后得到refresh_token 
  2）客户端把refresh_token发送给服务器
  3）服务器校验refresh_token是否有效（http请求方式: GET https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=APPID&grant_type=refresh_token&refresh_token=REFRESH_TOKEN）
    1】有效则获取到access_token和openid，通过openid查找数据中是否有userinfo
	如果没有则通过access_token和openid获取微信的userinfo（http请求方式: GET https://api.weixin.qq.com/sns/auth?access_token=ACCESS_TOKEN&openid=OPENID）
	如果有则直接返回userinfo数据
	2】无效则告诉客户端refresh_token失效，客户端重复1操作
2.通过refresh_token
  1）重复上面的3步骤

******/



UserBase HttpWXLogin::respondAccessToken(string result, string &token){
	//获取的是json
	YMSocketData sd(result);
	printf("sd:%s\n",sd.getJsonString().c_str());
	if (!sd.isMember("errcode")){
		//请求成功
		string access_token = sd["access_token"].asString();
		string refresh_token = sd["refresh_token"].asString();
		token = refresh_token;
		//获取到了access_token和refresh_token
		string openid = sd["openid"].asString();//需要和access_token与uid绑定起来
		int len = 0;
		UserBase ub;
		char *u = m_pRedis->get("openid"+openid,len);
		if (!u){
			//获取userinfo
			ub= requestUserinfo(access_token,openid);
		}
		else{
			UserBase *ub1 = m_pRedisGet->getUserBase(u);
			ub.CopyFrom(*ub1);
				
		}
		return ub;
	}
	else{
		string errmsg = sd["errmsg"].asString();
		printf("%s\n", errmsg.c_str());
	}
	UserBase ub;
	return ub;
}

UserBase HttpWXLogin::requestRefreshToken(string refreshtoken, string &token){
	char buff[300];
	sprintf(buff, "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s", APPID, refreshtoken.c_str());
	string result = HttpEvent::getIns()->requestData(buff, "", Code_Access_Token);
	return respondRefreshToken(result,token);
}

UserBase HttpWXLogin::respondRefreshToken(string result, string &token){
	UserBase ub;
	YMSocketData sd(result);
	printf("sd:%s\n", sd.getJsonString().c_str());
	if (!sd.isMember("errcode")){
		string refresh_token = sd["refresh_token"].asString();
		token = refresh_token;
		string openid = sd["openid"].asString();
		int len = 0;
		char *u = m_pRedis->get("openid" + openid, len);
		if (u){
			UserBase *ub1 = m_pRedisGet->getUserBase(u);
			ub.CopyFrom(*ub1);
			return ub;
		}
	}
	else{
		return ub;
	}
}

UserBase HttpWXLogin::requestUserinfo(string acctoken, string openid){
	char buff[300];
	sprintf(buff,"https://api.weixin.qq.com/sns/userinfo?access_token=%s&openid=%s",acctoken.c_str(),openid.c_str());
	string result = HttpEvent::getIns()->requestData(buff, "", Code_Access_Token);
	return respondUserinfo(result);
}

UserBase HttpWXLogin::respondUserinfo(string result){
	YMSocketData sd(result);
	printf("sd:%s\n", sd.getJsonString().c_str());
	if (!sd.isMember("errcode")){
		string openid = sd["openid"].asString();
		int len = 0;
		char* u = m_pRedis->get("userid_index", len);
		char buff[50];
		sprintf(buff, "%d", atoi(u) + 1);
		string uid = "wx";
		m_pRedis->set("userid_index", buff, len);
		uid += u;
		m_pRedis->set("openid" + openid, (char*)uid.c_str(), len);
		
		string nickname = sd["nickname"].asString();
		int sex = sd["sex"].asInt();
		string photo = sd["headimgurl"].asString();
		string city = sd["city"].asString();
		string province = sd["province"].asString();
		UserBase ub;
		ub.set_userid(uid);
		ub.set_username(nickname);
		ub.set_picurl(photo);
		ub.set_sex(sex);
		ub.set_address(province + city);
		m_pRedisPut->PushUserBase(ub);

		uint32 gold = ub.gold();
		Rank rk;
		rk.set_uid(ub.userid());
		m_pRedisPut->PushRank(rk);
		Rank rk1;
		rk1.set_uid(ub.userid());
		m_pRedisPut->PushRank(rk1);


		return ub;
	}
	UserBase ub;
	return ub;
}

UserBase HttpWXLogin::getUserinfo(string name, string pwd){
	int len = 0;
	char* u = m_pRedis->get("userid_index", len);
	char buff[50];
	sprintf(buff, "%d", atoi(u) + 1);
	string uid = "yk";
	m_pRedis->set("userid_index", buff, len);
	uid += u;

	UserBase ub;
	ub.set_userid(uid);
	ub.set_username(name);
	ub.set_picid(rand()%2+1);
	ub.set_sex(rand()%2);
	ub.set_picurl("http://www.lesharecs.com/1.jpg");
	m_pRedisPut->PushUserBase(ub);

	uint32 gold = ub.gold();
	Rank rk;
	rk.set_uid(ub.userid());
	m_pRedisPut->PushRank(rk);
	Rank rk1;
	rk1.set_uid(ub.userid());
	m_pRedisPut->PushRank(rk1);
	return ub;
}