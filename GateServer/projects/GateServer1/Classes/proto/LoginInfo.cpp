#include "LoginInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "LibEvent.h"

LoginInfo *LoginInfo::m_shareLoginInfo=NULL;
LoginInfo::LoginInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	CLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName());
	EventListen::getIns()->addDataPacketListener(cl.cmd(), this, Event_Handler(LoginInfo::HandlerCLoginHand));
}

LoginInfo::~LoginInfo(){
	
}

LoginInfo* LoginInfo::getIns(){
	if (!m_shareLoginInfo){
		m_shareLoginInfo = new LoginInfo();
		m_shareLoginInfo->init();
	}
	return m_shareLoginInfo;
}

bool LoginInfo::init()
{
	
    return true;
}

void LoginInfo::SendSLogin(int fd){
	SLogin cl;
	cl.set_err(0);
	LibEvent::getIns()->SendData(cl.cmd(),&cl,fd);
}

void LoginInfo::HandlerCLoginHand(ccEvent *event){
	CLogin cl;
	cl.CopyFrom(*event->msg);
	EventListen::getIns()->removeDataPacketListener(cl.cmd(), this, Event_Handler(LoginInfo::HandlerCLoginHand));
	string uid = cl.uid();
	string pwd = cl.pwd();
	printf("login:uid:%s,pwd:%s",uid.c_str(),pwd.c_str());
	SendSLogin(event->m_fd);
}