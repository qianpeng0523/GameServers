#include "PingInfo.h"
#include "EventDispatcher.h"
#include "ClientSocket.h"
#include "Common.h"
#include "StatTimer.h"
#include "redis.h"
PingInfo *PingInfo::m_sharePingInfo=NULL;
PingInfo::PingInfo()
{
	m_pingcount = 0;
	m_lasttime = 0;
	EventDispatcher *pe = EventDispatcher::getIns();
	
	CLPing cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName());
	openUpdate(true);
}

PingInfo::~PingInfo(){
	openUpdate(false);
	openCheckUpdate(false);
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

void PingInfo::SendCLPing(){
	CLPing cp;
	SLPing sp;
	EventDispatcher::getIns()->addListener(sp.cmd(), this, Event_Handler(PingInfo::HandSLPing));
	m_pingcount++;
	ClientSocket::getIns()->sendMsg(cp.cmd(),&cp);
}

void PingInfo::HandSLPing(ccEvent *event){
	m_pingcount = 0;
	SLPing cp;
	cp.CopyFrom(*event->msg);

}

void PingInfo::setTime(){
	m_lasttime = Common::getTime();
}

void PingInfo::update(float dt){
	time_t t = Common::getTime();
	if (m_lasttime != 0 && t - m_lasttime >= 13){
		m_lasttime = t;
		SendCLPing();
	}
	if (m_pingcount >= 4 && m_lasttime != 0 && t - m_lasttime >= 0){
		printf("pingocunt[%d]--t[%ld]---lasttime[%ld]\n", m_pingcount, t, m_lasttime);
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


void PingInfo::Check(float dt){
	if (!redis::getIns()->isConnect()){
		redis::getIns()->reconnect();
	}
}

void PingInfo::openCheckUpdate(bool isopen){
	if (isopen){
		StatTimer::getIns()->scheduleSelector(this, schedule_selector(PingInfo::Check), 8.0);
	}
	else{
		StatTimer::getIns()->unscheduleSelector(this, schedule_selector(PingInfo::Check));
	}
}