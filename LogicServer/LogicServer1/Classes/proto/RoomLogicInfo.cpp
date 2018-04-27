#include "RoomLogicInfo.h"
#include "ClientSocket.h"
#include "EventDispatcher.h"
#include "XXIconv.h"

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

void RoomLogicInfo::SendSDice(SDice sd){
	CDice cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice));
	ClientSocket::getIns()->sendMsg(cl.cmd(),&cl);
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

void RoomLogicInfo::SendSSelectZhuang(SSelectZhuang ssz){
	ClientSocket::getIns()->sendMsg(ssz.cmd(),&ssz);
}

void RoomLogicInfo::SendSZhuangCard(SZhuangCard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSCard(SCard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCDiscard(ccEvent *event){
	CDiscard cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSDiscard(SDiscard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSDraw(SDraw sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSOtherDraw(SOtherDraw sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandChi(ccEvent *event){
	CChi cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSChi(SChi sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCPeng(ccEvent *event){
	CPeng cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSPeng(SPeng sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCMingGang(ccEvent *event){
	CMingGang cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSMingGang(SMingGang sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCAnGang(ccEvent *event){
	CAnGang cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSAnGang(SAnGang sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSFa(SFa sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCHu(ccEvent *event){
	CHu cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::SendSHu(SHu sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSGameover(SGameover sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSRoundOver(SRoundOver sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSDissolveTip(SDissolveTip sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}