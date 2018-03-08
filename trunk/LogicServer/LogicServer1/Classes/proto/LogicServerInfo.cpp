﻿#include "LogicServerInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
LogicServerInfo *LogicServerInfo::m_shareLogicServerInfo=NULL;
LogicServerInfo::LogicServerInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	SLogicLogin sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName());
	
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

void LogicServerInfo::SendCLogicLogin(){
	CLogicLogin cl;
	cl.set_seession(LOGIC_TOKEN);
	cl.set_servername(SERVER_CODE);
	EventListen::getIns()->addDataPacketListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSLogicLoginHand));
	ClientSocket::getIns()->sendMsg(cl.cmd(),&cl);
}

void LogicServerInfo::HandlerSLogicLoginHand(ccEvent *event){
	SLogicLogin cl;
	cl.CopyFrom(*event->msg);
	EventListen::getIns()->removeDataPacketListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSLogicLoginHand));
	int err = cl.err();
	if (err==0){
		printf("连接成功!\n");
	}
	else{
		printf("数据有问题\n");
	}
}