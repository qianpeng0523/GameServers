#include "RoomLogicInfo.h"
#include "EventDispatcher.h"
#include "XXIconv.h"
#include "LibEvent.h"

RoomLogicInfo *RoomLogicInfo::m_shareRoomLogicInfo=NULL;
RoomLogicInfo::RoomLogicInfo()
{
	
	EventDispatcher *pe = EventDispatcher::getIns();
	CDice sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName());
	pe->addListener(sl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice));

	CDiscard sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName());
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomLogicInfo::HandCDiscard));

	CChi sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName());
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomLogicInfo::HandChi));

	CPeng sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName());
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomLogicInfo::HandCPeng));

	CMingGang sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName());
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomLogicInfo::HandCMingGang));

	CAnGang sl10;
	pe->registerProto(sl10.cmd(), sl10.GetTypeName());
	pe->addListener(sl10.cmd(), this, Event_Handler(RoomLogicInfo::HandCAnGang));

	SHu sl12;
	pe->registerProto(sl12.cmd(), sl12.GetTypeName());
	pe->addListener(sl12.cmd(), this, Event_Handler(RoomLogicInfo::HandCHu));

}

RoomLogicInfo::~RoomLogicInfo(){
	
}

RoomLogicInfo* RoomLogicInfo::getIns(){
	if (!m_shareRoomLogicInfo){
		m_shareRoomLogicInfo = new RoomLogicInfo();
		m_shareRoomLogicInfo->init();
	}
	return m_shareRoomLogicInfo;
}

bool RoomLogicInfo::init()
{
	
    return true;
}

void RoomLogicInfo::SendSDice(SDice sd, int fd){
	CDice cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice));
	
}

void RoomLogicInfo::HandCDice(ccEvent *event){
	SDice cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice));
	int err = cl.err();
	if (err==0){
		
	}
	else{
		
	}
}

void RoomLogicInfo::SendSSelectZhuang(SSelectZhuang ssz, int fd){
	LibEvent::getIns()->SendData(ssz.cmd(),&ssz, fd);
}

void RoomLogicInfo::SendSZhuangCard(SZhuangCard sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSCard(SCard sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCDiscard(ccEvent *event){
	CDiscard cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSDiscard(SDiscard sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSDraw(SDraw sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSOtherDraw(SOtherDraw sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandChi(ccEvent *event){
	CChi cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSChi(SChi sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCPeng(ccEvent *event){
	CPeng cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSPeng(SPeng sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCMingGang(ccEvent *event){
	CMingGang cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSMingGang(SMingGang sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCAnGang(ccEvent *event){
	CAnGang cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSAnGang(SAnGang sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSFa(SFa sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCHu(ccEvent *event){
	CHu cl;
	cl.CopyFrom(*event->msg);
	
}

void RoomLogicInfo::SendSHu(SHu sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSGameover(SGameover sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSRoundOver(SRoundOver sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::SendSDissolveTip(SDissolveTip sd, int fd){
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}