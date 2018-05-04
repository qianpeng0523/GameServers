
#ifndef __PingInfo__
#define __PingInfo__

#include "stdafx.h"
#include "ccEvent.h"
#include "Ping.h"
class PingInfo:public Object
{
public:
	PingInfo();
	~PingInfo();
	static PingInfo* getIns();
	bool init();
public:
	void SendSLPing(int fd);
	void HandCLPing(ccEvent *event);

private:
	static PingInfo *m_sharePingInfo;
	
};

#endif 
