#include "LogicServerInfo.h"
#include "ClientSocket.h"

#include "EventDispatcher.h"
#include "HttpLogic.h"

LogicServerInfo *LogicServerInfo::m_shareLogicServerInfo=NULL;
LogicServerInfo::LogicServerInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	SGateLogin sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(),LOGIC_MANAGER_TYPE);
	
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

void LogicServerInfo::SendCGateLogin(){
	CGateLogin cl;
	cl.set_seession(LOGIC_TOKEN);
	cl.set_servername(HttpLogic::SERVER_CODE);
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSGateLoginHand),LOGIC_MANAGER_TYPE);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void LogicServerInfo::HandlerSGateLoginHand(ccEvent *event){
	SGateLogin cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSGateLoginHand), LOGIC_MANAGER_TYPE);
	int err = cl.err();
	if (err == 0){
		printf("连接成功!\n");
	}
	else{
		printf("数据有问题\n");
	}
}