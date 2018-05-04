#include "PingInfo.h"
#include "EventDispatcher.h"
#include "LibEvent.h"
PingInfo *PingInfo::m_sharePingInfo=NULL;
PingInfo::PingInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	
	CLPing cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName(),GATE_TYPE);
	pe->addListener(cl1.cmd(), this, Event_Handler(PingInfo::HandCLPing), GATE_TYPE);

	CLPing cl2;
	pe->registerProto(cl2.cmd(), cl2.GetTypeName(), LOGIC_TYPE);
	pe->addListener(cl2.cmd(), this, Event_Handler(PingInfo::HandCLPing), LOGIC_TYPE);
}

PingInfo::~PingInfo(){
	
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

void PingInfo::SendSLPing(int fd){
	SLPing cp;
	LibEvent::getIns()->SendData(cp.cmd(),&cp,fd);
}

void PingInfo::HandCLPing(ccEvent *event){
	CLPing cp;
	cp.CopyFrom(*event->msg);
	SendSLPing(event->m_fd);
}