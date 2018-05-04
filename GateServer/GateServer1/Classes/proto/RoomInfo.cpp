#include "RoomInfo.h"
#include "ClientSocket.h"
#include "LibEvent.h"

RoomInfo *RoomInfo::m_shareRoomInfo=NULL;
RoomInfo::RoomInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CHMMJCreateRoom sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(), GAME_TYPE);
	pe->addListener(sl.cmd(), this, Event_Handler(RoomInfo::HandCHMMJCreateRoom),GAME_TYPE);

	CHMMJEnterRoom sl1;
	pe->registerProto(sl1.cmd(), sl1.GetTypeName(),GAME_TYPE);
	pe->addListener(sl1.cmd(), this, Event_Handler(RoomInfo::HandCHMMJEnterRoom),GAME_TYPE);

	CBegin sl3;
	pe->registerProto(sl3.cmd(), sl3.GetTypeName(),GAME_TYPE);
	pe->addListener(sl3.cmd(), this, Event_Handler(RoomInfo::HandCBegin),GAME_TYPE);

	CReady sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName(),GAME_TYPE);
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomInfo::HandCReady),GAME_TYPE);

	CLeave sl5;
	pe->registerProto(sl5.cmd(), sl5.GetTypeName(),GAME_TYPE);
	pe->addListener(sl5.cmd(), this, Event_Handler(RoomInfo::HandCLeave),GAME_TYPE);

	CLine sl6;
	pe->registerProto(sl6.cmd(), sl6.GetTypeName(),GAME_TYPE);
	pe->addListener(sl6.cmd(), this, Event_Handler(RoomInfo::HandCLine),GAME_TYPE);

	CDissolveRoom sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName(),GAME_TYPE);
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomInfo::HandCDissolveRoom),GAME_TYPE);

	CVote sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName(),GAME_TYPE);
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomInfo::HandCVote),GAME_TYPE);

	CRChat sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName(),GAME_TYPE);
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomInfo::HandCRChat),GAME_TYPE);










	SHMMJCreateRoom sl20;
	pe->registerProto(sl20.cmd(), sl20.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl20.cmd(), this, Event_Handler(RoomInfo::HandSHMMJCreateRoom), LOGIC_MANAGER_TYPE);

	SHMMJEnterRoom sl21;
	pe->registerProto(sl21.cmd(), sl21.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl21.cmd(), this, Event_Handler(RoomInfo::HandSHMMJEnterRoom), LOGIC_MANAGER_TYPE);

	SBegin sl22;
	pe->registerProto(sl22.cmd(), sl22.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl22.cmd(), this, Event_Handler(RoomInfo::HandSBegin), LOGIC_MANAGER_TYPE);

	SReady sl23;
	pe->registerProto(sl23.cmd(), sl23.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl23.cmd(), this, Event_Handler(RoomInfo::HandSReady), LOGIC_MANAGER_TYPE);

	SLeave sl24;
	pe->registerProto(sl24.cmd(), sl24.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl24.cmd(), this, Event_Handler(RoomInfo::HandSLeave), LOGIC_MANAGER_TYPE);

	SLine sl25;
	pe->registerProto(sl25.cmd(), sl25.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl25.cmd(), this, Event_Handler(RoomInfo::HandSLine), LOGIC_MANAGER_TYPE);

	SDissolveRoom sl26;
	pe->registerProto(sl26.cmd(), sl26.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl26.cmd(), this, Event_Handler(RoomInfo::HandSDissolveRoom), LOGIC_MANAGER_TYPE);

	SVote sl27;
	pe->registerProto(sl27.cmd(), sl27.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl27.cmd(), this, Event_Handler(RoomInfo::HandSVote), LOGIC_MANAGER_TYPE);

	SRChat sl28;
	pe->registerProto(sl28.cmd(), sl28.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl28.cmd(), this, Event_Handler(RoomInfo::HandSRChat), LOGIC_MANAGER_TYPE);

	SComein sl29;
	pe->registerProto(sl29.cmd(), sl29.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl29.cmd(), this, Event_Handler(RoomInfo::HandSComein), LOGIC_MANAGER_TYPE);

	SVoteResult sl30;
	pe->registerProto(sl30.cmd(), sl30.GetTypeName(), LOGIC_MANAGER_TYPE);
	pe->addListener(sl30.cmd(), this, Event_Handler(RoomInfo::HandSVoteResult), LOGIC_MANAGER_TYPE);
}

RoomInfo::~RoomInfo(){
	
}

RoomInfo* RoomInfo::getIns(){
	if (!m_shareRoomInfo){
		m_shareRoomInfo = new RoomInfo();
		m_shareRoomInfo->init();
	}
	return m_shareRoomInfo;
}

bool RoomInfo::init()
{
	
    return true;
}

void RoomInfo::HandCHMMJCreateRoom(ccEvent *event){
	CHMMJCreateRoom cl;
	cl.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cl.cmd(),&cl);
}

void RoomInfo::HandSHMMJCreateRoom(ccEvent *event){
	SHMMJCreateRoom sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.roomuser().userid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCHMMJEnterRoom(ccEvent *event){
	CHMMJEnterRoom cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSHMMJEnterRoom(ccEvent *event){
	SHMMJEnterRoom sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.uid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandSComein(ccEvent *event){
	SComein sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.uid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(),&sd,fd);

}

void RoomInfo::HandCBegin(ccEvent *event){
	CBegin cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSBegin(ccEvent *event){
	SBegin sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.uid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCReady(ccEvent *event){
	CReady cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSReady(ccEvent *event){
	SReady sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCLeave(ccEvent *event){
	CLeave cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSLeave(ccEvent *event){
	SLeave sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCLine(ccEvent *event){
	CLine cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSLine(ccEvent *event){
	SLine sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCDissolveRoom(ccEvent *event){
	CDissolveRoom cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSDissolveRoom(ccEvent *event){
	SDissolveRoom sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCVote(ccEvent *event){
	CVote cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSVote(ccEvent *event){
	SVote sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandSVoteResult(ccEvent *event){
	SVoteResult sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCRChat(ccEvent *event){
	CRChat cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::HandSRChat(ccEvent *event){
	SRChat sd;
	sd.CopyFrom(*event->msg);
	string uid = sd.suid();
	int fd = LibEvent::getIns()->getFd(uid);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}