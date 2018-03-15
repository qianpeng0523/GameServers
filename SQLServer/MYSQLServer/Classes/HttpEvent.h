#pragma once
#include "stdafx.h"
#include "event2/event.h"
#include "YMSocketData.h"
#include "YMSocketDataEvent.h"

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"

class HttpEvent{
public:
	HttpEvent();
	~HttpEvent();

	static HttpEvent* getIns();
	void init();
	YMSocketData getSocketDataByStr(const char* str, int sz);

	void SendMsg(YMSocketData &sd, struct evhttp_request *req);
private:
	static HttpEvent *m_ins;
	struct evhttp *m_httpd;
};