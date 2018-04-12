#ifndef __HttpWXLogin_SCENE_H__
#define __HttpWXLogin_SCENE_H__

#include "stdafx.h"

class HttpWXLogin:public Object
{
public:
	HttpWXLogin();
	~HttpWXLogin();
    virtual bool init();
	static HttpWXLogin *getIns();
	
	void requestAccessToken(string code, struct evhttp_request *req);
	void respondAccessToken(string result, struct evhttp_request *req,int fd);
public:
	
private:
	static HttpWXLogin *m_Ins;
};

#endif 