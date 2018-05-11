#include "RoomInfo.h"
#include "ClientSocket.h"
#include "GRoom.h"
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
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomInfo::HandCReady));

	CLeave sl5;
	pe->registerProto(sl5.cmd(), sl5.GetTypeName());
	pe->addListener(sl5.cmd(), this, Event_Handler(RoomInfo::HandCLeave));

	CLine sl6;
	pe->registerProto(sl6.cmd(), sl6.GetTypeName());
	pe->addListener(sl6.cmd(), this, Event_Handler(RoomInfo::HandCLine));

	CDissolveRoom sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName());
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomInfo::HandCDissolveRoom));

	CVote sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName());
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomInfo::HandCVote));

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
			UData *ud = udatas[i];
			if (ud){
				RoomUser *ru = sd.mutable_roomuser();
				ru->set_userid(ud->_uid);
				ru->set_position(i+1);
				ru->set_ready(ud->_ready);
				ru->set_begin(gr->isBegin());
				ru->set_zhuang(gr->getZhuang());
				ru->set_online(ud->_online);
			}
		}
	}
	else{
		sd.set_err(1);
	}
	SendSHMMJCreateRoom(sd);
}

void RoomInfo::SendSHMMJCreateRoom(SHMMJCreateRoom sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCHMMJEnterRoom(ccEvent *event){
	CHMMJEnterRoom cr;
	cr.CopyFrom(*event->msg);
	string rid = cr.roomid();
	int rtype = cr.rtype();
	string uid = cr.uid();

	SHMMJEnterRoom sr;
	sr.set_uid(uid);
	GRoom *gr = m_pRoomControl->enterRoom(uid, rid);
	RoomUser scomeinuser;
	if (gr){
		sr.set_begin(gr->isBegin());
		gr->onLine(uid, true);
		RoomData *rd = sr.mutable_roomdata();
		rd->CopyFrom(gr->getRoomData());
		UData ** udatas = gr->getUDatas();
		for (int i = 0; i < 4; i++){
			UData *ud = udatas[i];
			if (ud){
				RoomUser *ru = sr.add_roomusers();
				string puid = ud->_uid;
				ru->set_userid(puid);
				ru->set_position(i + 1);
				ru->set_ready(ud->_ready);
				ru->set_begin(gr->isBegin());
				ru->set_zhuang(gr->getZhuang());
				ru->set_online(ud->_online);
				if (puid.compare(uid)==0){
					scomeinuser.CopyFrom(*ru);
				}
				//SVote
			}
		}
	}
	else{
		sr.set_err(1);
	}
	SendSHMMJEnterRoom(sr);
	SComein sc;
	RoomUser *user = sc.mutable_roomuser();
	user->CopyFrom(scomeinuser);
	SendSComein(sc);
}

void RoomInfo::SendSHMMJEnterRoom(SHMMJEnterRoom sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::SendSComein(SComein sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCBegin(ccEvent *event){
	CBegin cr;
	cr.CopyFrom(*event->msg);
	
	int type = cr.type();
	string uid = cr.uid();
	m_pRoomControl->BeginMJ(type, uid);
}

void RoomInfo::SendSBegin(SBegin sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCReady(ccEvent *event){
	CReady cr;
	cr.CopyFrom(*event->msg);
	bool ready = cr.ready();
	string uid = cr.uid();
	m_pRoomControl->ReadyMJ(uid, ready);
}

void RoomInfo::SendSReady(SReady sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCLeave(ccEvent *event){
	CLeave cr;
	cr.CopyFrom(*event->msg);
	string uid = cr.uid();

	m_pRoomControl->LeaveMJ(uid,cr.zhudong());
}

void RoomInfo::SendSLeave(SLeave sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCLine(ccEvent *event){
	CLine cr;
	cr.CopyFrom(*event->msg);
	string uid = cr.uid();
	bool online = cr.online();

	m_pRoomControl->onLine(uid, online);
}

void RoomInfo::SendSLine(SLine sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCDissolveRoom(ccEvent *event){
	CDissolveRoom cr;
	cr.CopyFrom(*event->msg);
	
	string rid = cr.rid();
	string uid = cr.uid();
	m_pRoomControl->DissolveRoom(uid, rid);
}

void RoomInfo::SendSDissolveRoom(SDissolveRoom sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::HandCVote(ccEvent *event){
	CVote cr;
	cr.CopyFrom(*event->msg);
	
	bool agree = cr.agree();
	string uid = cr.uid();
	m_pRoomControl->Vote(uid, agree);
}

void RoomInfo::SendSVote(SVote sd){
	sd.set_cmd(sd.cmd());
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomInfo::SendSVoteResult(SVoteResult sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}