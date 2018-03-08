#pragma once

#include <event.h>
//for http
#include <evhttp.h>
#include <signal.h>
#include "YMSocketData.h"
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
	void requestData(string url);
	void init();
	
	void EventDispath(struct evhttp_request *&req, string uri);
private:
	void SendMsg(YMSocketData &sd, struct evhttp_request *req);
	YMSocketData getSocketDataByStr(string str, int sz);
private:
	static HttpEvent *m_ins;
	
};