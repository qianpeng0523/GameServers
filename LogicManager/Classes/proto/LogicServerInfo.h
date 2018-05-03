
#ifndef __LogicServerInfo__
#define __LogicServerInfo__

#include "stdafx.h"
#include "EventDispatcher.h"

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

	void SendSGateLogin(int fd, int err);
	void HandlerCGateLoginHand(ccEvent *event);

	int getFd(SERVERTYPE type);
	SERVERTYPE getServerType(string servername);
	int getFd(string servername);
private:
	static LogicServerInfo *m_shareLogicServerInfo;
	map<SERVERTYPE, int>m_gamefds;
	map<string, SERVERTYPE>m_name_type;
};

#endif 
