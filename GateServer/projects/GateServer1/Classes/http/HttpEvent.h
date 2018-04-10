#pragma once

#include <event.h>
//for http
#include <evhttp.h>
#include <signal.h>
#include "YMSocketDataEvent.h"
#include "stdafx.h"

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"


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
	void requestData(string url,YMSocketData sd);
	void requestData(string url, string content);
	void init();
	YMSocketData getSocketDataByStr(char* str, int sz);
	void SendMsg(YMSocketData &sd, struct evhttp_request *req);
private:
	char *getData(vector<char> vec);
	
private:
	static HttpEvent *m_ins;
	
};