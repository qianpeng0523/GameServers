
#ifndef __ConfigInfo__
#define __ConfigInfo__

#include "stdafx.h"
#include "ccEvent.h"

class ConfigInfo:public Object
{
public:
	ConfigInfo();
	~ConfigInfo();
	static ConfigInfo* getIns();
	bool init();
public:
	void SendSConfig(SConfig sc, int fd);
	void HandlerCConfig(ccEvent *event);

	void SendSPushCurrency(SPushCurrency spc,int fd);
private:
	static ConfigInfo *m_shareConfigInfo;
	
};

#endif 
