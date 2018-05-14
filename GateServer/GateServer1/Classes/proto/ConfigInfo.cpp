#include "ConfigInfo.h"
#include "EventDispatcher.h"
#include "ClientSocket.h"
#include "LoginInfo.h"
#include "LibEvent.h"
#include "RedisGet.h"

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
	
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		SConfig *sl1 = RedisGet::getIns()->getSConfig(uid);
		if (sl1){
			return sl1;
		}
		else{
			SConfig sl;
			sl1 = (SConfig *)ccEvent::create_message(sl.GetTypeName());
			sl.set_mail(false);
			sl.set_active(true);
			sl.set_firstbuy(true);
			sl.set_fri(false);
			sl.set_task(false);
			sl.set_free(false);
			sl.set_yqs(true);
			sl1->CopyFrom(sl);
			RedisPut::getIns()->setSConfig(uid, sl1);
		}
		SendSConfig(*sl1, event->m_fd);
	}
}

void ConfigInfo::SendSPushCurrency(SPushCurrency spc, int fd){
	LibEvent::getIns()->SendData(spc.cmd(), &spc, fd);
}