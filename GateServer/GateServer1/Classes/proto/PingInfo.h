
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
	void SendSPing(SPing sp, int fd);
	void HandlerCPing(ccEvent *event);

	void SendCLPing();
	void HandSLPing(ccEvent *event);

	void update(float dt);
	void openUpdate(bool isopen);
	void setTime();
private:
	static PingInfo *m_sharePingInfo;
	time_t m_lasttime;
	int m_pingcount;
};

#endif 
