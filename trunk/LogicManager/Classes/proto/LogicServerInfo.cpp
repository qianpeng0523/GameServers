﻿#include "LogicServerInfo.h"
#include "LibEvent.h"

LogicServerInfo *LogicServerInfo::m_shareLogicServerInfo=NULL;
LogicServerInfo::LogicServerInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	CLogicLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName());
	p->addDataPacketListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerCLogicLoginHand));
}

LogicServerInfo::~LogicServerInfo(){
	
}

LogicServerInfo* LogicServerInfo::getIns(){
	if (!m_shareLogicServerInfo){
		m_shareLogicServerInfo = new LogicServerInfo();
		m_shareLogicServerInfo->init();
	}
	return m_shareLogicServerInfo;
}

bool LogicServerInfo::init()
{
	
    return true;
}

void LogicServerInfo::SendSLogicLogin(int fd,int err){
	SLogicLogin sl;
	sl.set_err(err);
	LibEvent::getIns()->SendData(sl.cmd(), &sl, fd);
}

void LogicServerInfo::HandlerCLogicLoginHand(ccEvent *event){
	CLogicLogin cl;
	cl.CopyFrom(*event->msg);
	string servername = cl.servername();
	string seession = cl.seession();
	if (/*event->m_servername.compare(servername) == 0 &&*/ seession.compare(LOGIC_TOKEN) == 0){
		SendSLogicLogin(event->m_fd,0);
	}
	else{
		SendSLogicLogin(event->m_fd,1);
	}
}