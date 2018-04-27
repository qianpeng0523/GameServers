#include "RoomInfo.h"
#include "ClientSocket.h"
#include "RoomControl.h"
static RoomControl *m_pRoomControl = RoomControl::getIns();

RoomInfo *RoomInfo::m_shareRoomInfo=NULL;
RoomInfo::RoomInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CHMMJCreateRoom sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName());
	pe->addListener(sl.cmd(), this, Event_Handler(RoomInfo::HandCHMMJCreateRoom));

	CHMMJEnterRoom sl1;
	pe->registerProto(sl1.cmd(), sl1.GetTypeName());
	pe->addListener(sl1.cmd(), this, Event_Handler(RoomInfo::HandCHMMJEnterRoom));

	CBegin sl3;
	pe->registerProto(sl3.cmd(), sl3.GetTypeName());
	pe->addListener(sl3.cmd(), this, Event_Handler(RoomInfo::HandCBegin));

	CReady sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName());
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomInfo::SendSReady));

	CLeave sl5;
	pe->registerProto(sl5.cmd(), sl5.GetTypeName());
	pe->addListener(sl5.cmd(), this, Event_Handler(RoomInfo::SendSLeave));

	CLine sl6;
	pe->registerProto(sl6.cmd(), sl6.GetTypeName());
	pe->addListener(sl6.cmd(), this, Event_Handler(RoomInfo::SendSLine));

	CDissolveRoom sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName());
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomInfo::SendSDissolveRoom));

	CVote sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName());
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomInfo::SendSVote));

	CRChat sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName());
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomInfo::SendSRChat));

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
	int type = cl.type();
	int ante = cl.ante();
	int round = cl.round();
	int bao = cl.bao();
	int bang = cl.bang();
	string uid = cl.uid();
	GRoom *gr = m_pRoomControl->createRoom(uid, type, ante, round, bao, bang);

	SHMMJCreateRoom sd;
	if (gr){
		RoomData *rd= sd.mutable_roomdata();
		rd->CopyFrom(gr->getRoomData());
		UData ** udatas = gr->getUDatas();
		for (int i = 0; i < 4;i++){
			if (udatas[i]){
				RoomUser *ru = sd.mutable_roomuser();
				ru->set_userid(udatas[i]->_uid);
				ru->set_position(i+1);
			}
		}
	}
	else{
		sd.set_err(1);
	}
	SendSHMMJCreateRoom(sd);
}

void RoomInfo::SendSHMMJCreateRoom(SHMMJCreateRoom sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCHMMJEnterRoom(ccEvent *event){
	CHMMJEnterRoom cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSHMMJEnterRoom(SHMMJEnterRoom sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::SendSComein(SComein sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(),&sd);

}

void RoomInfo::HandCBegin(ccEvent *event){
	CBegin cr;
	cr.CopyFrom(*event->msg);
	
}

void RoomInfo::SendSBegin(SBegin sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCReady(ccEvent *event){
	CReady cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSReady(SReady sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCLeave(ccEvent *event){
	CLeave cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSLeave(SLeave sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCLine(ccEvent *event){
	CLine cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSLine(SLine sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCDissolveRoom(ccEvent *event){
	CDissolveRoom cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSDissolveRoom(SDissolveRoom sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCVote(ccEvent *event){
	CVote cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSVote(SVote sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::SendSVoteResult(SVoteResult sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCRChat(ccEvent *event){
	CRChat cr;
	cr.CopyFrom(*event->msg);
	ClientSocket::getIns()->sendMsg(cr.cmd(), &cr);
}

void RoomInfo::SendSRChat(SRChat sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}