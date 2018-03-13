
#ifndef __LoginInfo__
#define __LoginInfo__

#include "stdafx.h"



class LoginInfo:public Object
{
public:
	LoginInfo();
	~LoginInfo();
	static LoginInfo* getIns();
	bool init();
public:
	void SendSLogin(int fd);
	void HandlerCLoginHand(ccEvent *event);
private:
	static LoginInfo *m_shareLoginInfo;
	
};

#endif 
