#include "ConfigInfo.h"
#include "EventDispatcher.h"
#include "ClientSocket.h"
#include "LoginInfo.h"
#include "LibEvent.h"
ConfigInfo *ConfigInfo::m_shareConfigInfo=NULL;
ConfigInfo::ConfigInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CConfig sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(),GAME_TYPE);
	pe->addListener(sl.cmd(), this, Event_Handler(ConfigInfo::HandlerCConfig),GAME_TYPE);
}

ConfigInfo::~ConfigInfo(){
	
}

ConfigInfo* ConfigInfo::getIns(){
	if (!m_shareConfigInfo){
		m_shareConfigInfo = new ConfigInfo();
		m_shareConfigInfo->init();
	}
	return m_shareConfigInfo;
}

bool ConfigInfo::init()
{
	
    return true;
}

void ConfigInfo::SendSConfig(SConfig sc,int fd){
	LibEvent::getIns()->SendData(sc.cmd(),&sc,fd);
}

void ConfigInfo::HandlerCConfig(ccEvent *event){
	CConfig cl;
	cl.CopyFrom(*event->msg);
	
	SConfig sl;
	SendSConfig(sl,event->m_fd);
}

void ConfigInfo::SendSPushCurrency(SPushCurrency spc, int fd){
	LibEvent::getIns()->SendData(spc.cmd(), &spc, fd);
}