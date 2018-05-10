#include "LogicServerInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "HttpLogic.h"
#include "RoomInfo.h"
LogicServerInfo *LogicServerInfo::m_shareLogicServerInfo=NULL;
LogicServerInfo::LogicServerInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	SLogiCLogin sl;
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

void LogicServerInfo::SendCLogiCLogin(){
	CLogiCLogin cl;
	cl.set_seession(LOGIC_TOKEN);
	cl.set_servername(LOGICNAME);
	EventListen::getIns()->addDataPacketListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSLogiCLoginHand));
	ClientSocket::getIns()->sendMsg(cl.cmd(),&cl);
}

void LogicServerInfo::HandlerSLogiCLoginHand(ccEvent *event){
	SLogiCLogin cl;
	cl.CopyFrom(*event->msg);
	EventListen::getIns()->removeDataPacketListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerSLogiCLoginHand));
	int err = cl.err();
	if (err==0){
		CLog::log("连接成功!\n");
// 		CHMMJCreateRoom cr;
// 		cr.set_uid("100001");
// 		cr.set_type(1);
// 		int len = cr.ByteSize();
// 		char* sm = new char[len];
// 		cr.SerializePartialToArray(sm, len);
// 		//char *out = new char[len + 1];
// 		//HttpLogic::getIns()->aes_encrypt(sm, len, out);
// 		ccEvent *ev = new ccEvent(cr.cmd(),sm,len,0);
// 		RoomInfo::getIns()->HandCHMMJCreateRoom(ev);
		//delete sm;
		//delete out;
	}
	else{
		CLog::log("数据有问题\n");
	}
}