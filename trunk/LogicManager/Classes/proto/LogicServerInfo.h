﻿
#ifndef __LogicServerInfo__
#define __LogicServerInfo__

#include "stdafx.h"



class LogicServerInfo:public Object
{
public:
	LogicServerInfo();
	~LogicServerInfo();
	static LogicServerInfo* getIns();
	bool init();
public:
	void SendSLogicLogin(int fd, int err);
	void HandlerCLogicLoginHand(ccEvent *event);
private:
	static LogicServerInfo *m_shareLogicServerInfo;
	
};

#endif 
