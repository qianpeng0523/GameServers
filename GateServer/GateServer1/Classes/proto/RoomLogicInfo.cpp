#include "RoomLogicInfo.h"
#include "ClientSocket.h"
#include "EventDispatcher.h"
#include "XXIconv.h"
#include "LibEvent.h"

RoomLogicInfo *RoomLogicInfo::m_shareRoomLogicInfo=NULL;
RoomLogicInfo::RoomLogicInfo()
{
	
	EventDispatcher *pe = EventDispatcher::getIns();
	CDice sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(), GAME_TYPE);
	pe->addListener(sl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice), GAME_TYPE);

	CDiscard sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName(), GAME_TYPE);
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomLogicInfo::HandCDiscard), GAME_TYPE);

	CChi sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName(), GAME_TYPE);
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomLogicInfo::HandChi), GAME_TYPE);

	CPeng sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName(), GAME_TYPE);
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomLogicInfo::HandCPeng), GAME_TYPE);

	CMingGang sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName(), GAME_TYPE);
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomLogicInfo::HandCMingGang), GAME_TYPE);

	CAnGang sl10;
	pe->registerProto(sl10.cmd(), sl10.GetTypeName(), GAME_TYPE);
	pe->addListener(sl10.cmd(), this, Event_Handler(RoomLogicInfo::HandCAnGang), GAME_TYPE);

	CHu sl12;
	pe->registerProto(sl12.cmd(), sl12.GetTypeName(), GAME_TYPE);
	pe->addListener(sl12.cmd(), this, Event_Handler(RoomLogicInfo::HandCHu), GAME_TYPE);





	SDice sl13;
	pe->registerProto(sl13.cmd(), sl13.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl13.cmd(), this, Event_Handler(RoomLogicInfo::HandSDice), LOGIC_MANAGER_TYPE);

	SDiscard sl14;
	pe->registerProto(sl14.cmd(), sl14.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl14.cmd(), this, Event_Handler(RoomLogicInfo::HandSDiscard), LOGIC_MANAGER_TYPE);

	SChi sl15;
	pe->registerProto(sl15.cmd(), sl15.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl15.cmd(), this, Event_Handler(RoomLogicInfo::HandSChi), LOGIC_MANAGER_TYPE);

	SPeng sl16;
	pe->registerProto(sl16.cmd(), sl16.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl16.cmd(), this, Event_Handler(RoomLogicInfo::HandSPeng), LOGIC_MANAGER_TYPE);

	SMingGang sl17;
	pe->registerProto(sl17.cmd(), sl17.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl17.cmd(), this, Event_Handler(RoomLogicInfo::HandSMingGang), LOGIC_MANAGER_TYPE);

	SAnGang sl18;
	pe->registerProto(sl18.cmd(), sl18.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl18.cmd(), this, Event_Handler(RoomLogicInfo::HandSAnGang), LOGIC_MANAGER_TYPE);

	SHu sl19;
	pe->registerProto(sl19.cmd(), sl19.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl19.cmd(), this, Event_Handler(RoomLogicInfo::HandSHu), LOGIC_MANAGER_TYPE);

	SSelectZhuang sl20;
	pe->registerProto(sl20.cmd(), sl20.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl20.cmd(), this, Event_Handler(RoomLogicInfo::HandSSelectZhuang), LOGIC_MANAGER_TYPE);

	SZhuangCard sl21;
	pe->registerProto(sl21.cmd(), sl21.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl21.cmd(), this, Event_Handler(RoomLogicInfo::HandSZhuangCard), LOGIC_MANAGER_TYPE);

	SCard sl22;
	pe->registerProto(sl22.cmd(), sl22.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl22.cmd(), this, Event_Handler(RoomLogicInfo::HandSCard), LOGIC_MANAGER_TYPE);

	SDraw sl23;
	pe->registerProto(sl23.cmd(), sl23.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl23.cmd(), this, Event_Handler(RoomLogicInfo::HandSDraw), LOGIC_MANAGER_TYPE);

	SOtherDraw sl24;
	pe->registerProto(sl24.cmd(), sl24.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl24.cmd(), this, Event_Handler(RoomLogicInfo::HandSOtherDraw), LOGIC_MANAGER_TYPE);

	SFa sl25;
	pe->registerProto(sl25.cmd(), sl25.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl25.cmd(), this, Event_Handler(RoomLogicInfo::HandSFa), LOGIC_MANAGER_TYPE);

	SGameover sl26;
	pe->registerProto(sl26.cmd(), sl26.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl26.cmd(), this, Event_Handler(RoomLogicInfo::HandSGameover), LOGIC_MANAGER_TYPE);

	SRoundOver sl27;
	pe->registerProto(sl27.cmd(), sl27.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl27.cmd(), this, Event_Handler(RoomLogicInfo::HandSRoundOver), LOGIC_MANAGER_TYPE);

	SDissolveTip sl28;
	pe->registerProto(sl28.cmd(), sl28.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl28.cmd(), this, Event_Handler(RoomLogicInfo::HandSDissolveTip), LOGIC_MANAGER_TYPE);
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

void RoomLogicInfo::HandSDice(ccEvent *event){
	SDice sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	ClientSocket::getIns()->sendMsg(sd.cmd(),&sd);
}

void RoomLogicInfo::HandCDice(ccEvent *event){
	SDice cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice),GAME_TYPE);
	int err = cl.err();
	if (err==0){
		
	}
	else{
		
	}
}

void RoomLogicInfo::HandSSelectZhuang(ccEvent *event){
	SSelectZhuang sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(),&sd, fd);
}

void RoomLogicInfo::HandSZhuangCard(ccEvent *event){
	SZhuangCard sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.uid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSCard(ccEvent *event){
	SCard sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.uid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCDiscard(ccEvent *event){
	CDiscard cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSDiscard(ccEvent *event){
	SDiscard sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSDraw(ccEvent *event){
	SDraw sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSOtherDraw(ccEvent *event){
	SOtherDraw sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandChi(ccEvent *event){
	CChi cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSChi(ccEvent *event){
	SChi sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCPeng(ccEvent *event){
	CPeng cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSPeng(ccEvent *event){
	SPeng sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCMingGang(ccEvent *event){
	CMingGang cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSMingGang(ccEvent *event){
	SMingGang sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCAnGang(ccEvent *event){
	CAnGang cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSAnGang(ccEvent *event){
	SAnGang sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSFa(ccEvent *event){
	SFa sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandCHu(ccEvent *event){
	CHu cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(), &cl);
}

void RoomLogicInfo::HandSHu(ccEvent *event){
	SHu sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSGameover(ccEvent *event){
	SGameover sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSRoundOver(ccEvent *event){
	SRoundOver sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomLogicInfo::HandSDissolveTip(ccEvent *event){
	SDissolveTip sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}