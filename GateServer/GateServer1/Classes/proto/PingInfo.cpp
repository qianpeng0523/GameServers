#include "PingInfo.h"
#include "EventDispatcher.h"
#include "ClientSocket.h"
#include "LoginInfo.h"
#include "LibEvent.h"
PingInfo *PingInfo::m_sharePingInfo=NULL;
PingInfo::PingInfo()
{
	m_pingcount = 0;
	m_lasttime = 0;
	EventDispatcher *pe = EventDispatcher::getIns();
	CPing sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(), GAME_TYPE);
	pe->addListener(sl.cmd(), this, Event_Handler(PingInfo::HandlerCPing), GAME_TYPE);

	SLPing cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName(),LOGIC_MANAGER_TYPE);
	openUpdate(true);
}

PingInfo::~PingInfo(){
	openUpdate(false);
}

PingInfo* PingInfo::getIns(){
	if (!m_sharePingInfo){
		m_sharePingInfo = new PingInfo();
		m_sharePingInfo->init();
	}
	return m_sharePingInfo;
}

bool PingInfo::init()
{
	
    return true;
}


void PingInfo::SendSPing(SPing sp, int fd){
	LibEvent::getIns()->SendData(sp.cmd(), &sp, fd);
}

void PingInfo::HandlerCPing(ccEvent *event){
	CPing cp;
	cp.CopyFrom(*event->msg);
	SPing sp;
	sp.set_cmd(sp.cmd());
	SendSPing(sp, event->m_fd);
}

void PingInfo::SendCLPing(){
	CLPing cp;
	cp.set_cmd(cp.cmd());
	SLPing sp;
	EventDispatcher::getIns()->addListener(sp.cmd(), this, Event_Handler(PingInfo::HandSLPing),LOGIC_MANAGER_TYPE);
	m_pingcount++;
	ClientSocket::getIns()->sendMsg(cp.cmd(),&cp);
}

void PingInfo::HandSLPing(ccEvent *event){
	m_pingcount = 0;
	SLPing cp;
	cp.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cp.cmd(), this, Event_Handler(PingInfo::HandSLPing),LOGIC_MANAGER_TYPE);
}

void PingInfo::setTime(){
	m_lasttime = Common::getTime();
}

void PingInfo::update(float dt){
	time_t t = Common::getTime();
	if (m_lasttime != 0 && t - m_lasttime >= 60){
		m_lasttime = t;
		SendCLPing();
	}
	if (m_pingcount >= 3 && m_lasttime != 0 && t - m_lasttime >= 0){
		CLog::log("pingocunt[%d]--t[%ld]---lasttime[%ld]\n", m_pingcount, t, m_lasttime);
		m_lasttime = t;
		m_pingcount = 0;
		//判定客户端已经与服务器端断开
		ClientSocket::getIns()->close();
	}
}

void PingInfo::openUpdate(bool isopen){
	if (isopen){
		StatTimer::getIns()->scheduleSelector(this, schedule_selector(PingInfo::update), 1.0);
	}
	else{
		StatTimer::getIns()->unscheduleSelector(this, schedule_selector(PingInfo::update));
	}
}