#pragma once

#include "Object.h"
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>     //for strcat
//for struct evkeyvalq
#include <event.h>
//for http
#include <evhttp.h>
#include <signal.h>
#include "YMSocketData.h"
#include "YMSocketDataEvent.h"

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"


enum GetWechtData{
	USER_INFO,
	LOGIN_INFO,
	FRESH_TOKEN,
};

struct WechatInfos
{
	string access_token;
	int expires_in;
	string refresh_token;
	string openid;
	string scope;
	string unionid;
};

struct HttpEventItem
{
	struct evhttp_request *req;
	void *arg;
};

class HttpEvent : public Object{
public:
	HttpEvent();
	~HttpEvent();

	static HttpEvent* getIns();
	void curlPost(string url, struct evbuffer *buf, struct evhttp_request *&req, GetWechtData gwd);
	void init();
	YMSocketData getSocketDataByStr(string str);

	void EventDispath(struct evhttp_request *&req,std::vector<string> vecs);

	//test
	void update(float dt);
private:
	static HttpEvent *m_ins;
	int m_maxid;
	string m_userid;
	std::map<string, WechatInfos *>m_wechatinfos;//unionid 
};