#include "RoomInfo.h"
#include "LibEvent.h"
#include "LogicServerInfo.h"

static LogicServerInfo *m_pLogicServerInfo = LogicServerInfo::getIns();
RoomInfo *RoomInfo::m_shareRoomInfo=NULL;
RoomInfo::RoomInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CHMMJCreateRoom sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName(), GATE_TYPE);
	pe->addListener(sl.cmd(), this, Event_Handler(RoomInfo::HandCHMMJCreateRoom), GATE_TYPE);

	CHMMJEnterRoom sl1;
	pe->registerProto(sl1.cmd(), sl1.GetTypeName(), GATE_TYPE);
	pe->addListener(sl1.cmd(), this, Event_Handler(RoomInfo::HandCHMMJEnterRoom), GATE_TYPE);

	CBegin sl3;
	pe->registerProto(sl3.cmd(), sl3.GetTypeName(), GATE_TYPE);
	pe->addListener(sl3.cmd(), this, Event_Handler(RoomInfo::HandCBegin), GATE_TYPE);

	CReady sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName(), GATE_TYPE);
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomInfo::HandCReady), GATE_TYPE);

	CLeave sl5;
	pe->registerProto(sl5.cmd(), sl5.GetTypeName(), GATE_TYPE);
	pe->addListener(sl5.cmd(), this, Event_Handler(RoomInfo::HandCLeave), GATE_TYPE);

	CLine sl6;
	pe->registerProto(sl6.cmd(), sl6.GetTypeName(), GATE_TYPE);
	pe->addListener(sl6.cmd(), this, Event_Handler(RoomInfo::HandCLine), GATE_TYPE);

	CDissolveRoom sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName(), GATE_TYPE);
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomInfo::HandCDissolveRoom), GATE_TYPE);

	CVote sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName(), GATE_TYPE);
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomInfo::HandCVote), GATE_TYPE);

	CRChat sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName(), GATE_TYPE);
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomInfo::HandCRChat), GATE_TYPE);



	SHMMJCreateRoom sl20;
	pe->registerProto(sl20.cmd(), sl20.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl20.cmd(), this, Event_Handler(RoomInfo::HandSHMMJCreateRoom), LOGIC_TYPE);

	SHMMJEnterRoom sl21;
	pe->registerProto(sl21.cmd(), sl21.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl21.cmd(), this, Event_Handler(RoomInfo::HandSHMMJEnterRoom), LOGIC_TYPE);

	SComein sl22;
	pe->registerProto(sl22.cmd(), sl22.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl22.cmd(), this, Event_Handler(RoomInfo::HandSComein), LOGIC_TYPE);

	SBegin sl23;
	pe->registerProto(sl23.cmd(), sl23.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl23.cmd(), this, Event_Handler(RoomInfo::HandSBegin), LOGIC_TYPE);

	SReady sl24;
	pe->registerProto(sl24.cmd(), sl24.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl24.cmd(), this, Event_Handler(RoomInfo::HandSReady), LOGIC_TYPE);

	SLeave sl25;
	pe->registerProto(sl25.cmd(), sl25.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl25.cmd(), this, Event_Handler(RoomInfo::HandSLeave), LOGIC_TYPE);

	SLine sl26;
	pe->registerProto(sl26.cmd(), sl26.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl26.cmd(), this, Event_Handler(RoomInfo::HandSLine), LOGIC_TYPE);

	SDissolveRoom sl27;
	pe->registerProto(sl27.cmd(), sl27.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl27.cmd(), this, Event_Handler(RoomInfo::HandSDissolveRoom), LOGIC_TYPE);

	SVote sl28;
	pe->registerProto(sl28.cmd(), sl28.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl28.cmd(), this, Event_Handler(RoomInfo::HandSVote), LOGIC_TYPE);

	SVoteResult sl30;
	pe->registerProto(sl30.cmd(), sl3.GetTypeName(), LOGIC_TYPE);
	pe->addListener(sl3.cmd(), this, Event_Handler(RoomInfo::HandSVoteResult), LOGIC_TYPE);
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
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void RoomInfo::HandSHMMJCreateRoom(ccEvent *event){
	SHMMJCreateRoom sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	int err = sd.err();
	if (err == 0){
		string rid = sd.roomdata().roomid();
		string uid = sd.roomuser().userid();
		PushRoom(rid,uid);
	}

	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCHMMJEnterRoom(ccEvent *event){
	CHMMJEnterRoom cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSHMMJEnterRoom(ccEvent *event){
	SHMMJEnterRoom sd;
	sd.CopyFrom(*event->msg);
	string rid = sd.roomdata().roomid();
	string uid = sd.uid();
	PushRoom(rid, uid);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandSComein(ccEvent *event){
	SComein sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.roomuser().userid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SComein sd1;
		sd1.CopyFrom(sd);
		string puid = users.at(i);
		if (puid.compare(uid) != 0){
			sd1.set_uid(puid);
			LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
		}
	}
}

void RoomInfo::HandCBegin(ccEvent *event){
	CBegin cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSBegin(ccEvent *event){
	SBegin sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	LibEvent::getIns()->SendData(sd.cmd(), &sd, fd);
}

void RoomInfo::HandCReady(ccEvent *event){
	CReady cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSReady(ccEvent *event){
	SReady sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SReady sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
	
}

void RoomInfo::HandCLeave(ccEvent *event){
	CLeave cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSLeave(ccEvent *event){
	SLeave sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SLeave sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
	PopUserFromRoom(rid);
}

void RoomInfo::HandCLine(ccEvent *event){
	CLine cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSLine(ccEvent *event){
	SLine sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SLine sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
}

void RoomInfo::HandCDissolveRoom(ccEvent *event){
	CDissolveRoom cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSDissolveRoom(ccEvent *event){
	SDissolveRoom sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SDissolveRoom sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
}

void RoomInfo::HandCVote(ccEvent *event){
	CVote cr;
	cr.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(LOGIC_TYPE);
	LibEvent::getIns()->SendData(cr.cmd(), &cr, fd);
}

void RoomInfo::HandSVote(ccEvent *event){
	SVote sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SVote sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
}

void RoomInfo::HandSVoteResult(ccEvent *event){
	SVoteResult sd;
	sd.CopyFrom(*event->msg);
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	bool isjiesan = sd.dissolve();
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SVoteResult sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
	
	if (isjiesan){
		PopRoom(rid);
	}
	
}

void RoomInfo::HandCRChat(ccEvent *event){
	CRChat cr;
	cr.CopyFrom(*event->msg);
	string uid = cr.uid();
	string content = cr.content();

	SRChat sd;
	sd.set_uid(uid);
	sd.set_content(content);
	SendSRChat(sd);

	
}

void RoomInfo::SendSRChat(SRChat sd){
	int fd = m_pLogicServerInfo->getFd(GATE_TYPE);
	string uid = sd.uid();
	string rid = getRoomId(uid);
	vector<string >users = getRoomUsers(rid);
	for (int i = 0; i < users.size(); i++){
		SRChat sd1;
		sd1.CopyFrom(sd);
		sd1.set_suid(users.at(i));
		LibEvent::getIns()->SendData(sd1.cmd(), &sd1, fd);
	}
}

string RoomInfo::getRoomId(string uid){
	if (m_roomids.find(uid) != m_roomids.end()){
		return m_roomids.at(uid);
	}
	return "";
}

void RoomInfo::PushRoom(string rid, string uid){
	auto itr = m_pRooms.find(rid);
	if (itr == m_pRooms.end()){
		RoomCache *rc = new RoomCache();
		rc->_fzuid = uid;
		rc->_uids.push_back(uid);
		m_pRooms.insert(make_pair(rid, rc));
	}
	else{
		RoomCache *rc = itr->second;
		bool ist = false;
		for (int i = 0; i < rc->_uids.size(); i++){
			string puid = rc->_uids.at(i);
			if (puid.compare(uid) == 0){
				ist = true;
			}
		}
		if (!ist){
			rc->_uids.push_back(uid);
			m_pRooms.at(rid) = rc;
		}
	}

	if (m_roomids.find(uid) == m_roomids.end()){
		m_roomids.insert(make_pair(uid, rid));
	}
}

void RoomInfo::PopRoom(string rid){
	auto itr = m_pRooms.find(rid);
	if (itr != m_pRooms.end()){
		RoomCache *p = itr->second;
		for (int i = 0; i < p->_uids.size(); i++){
			string uid = p->_uids.at(i);
			auto itr1 = m_roomids.find(uid);
			if (itr1 != m_roomids.end()){
				m_roomids.erase(itr1);
			}
		}
		delete p;
		p = NULL;
		m_pRooms.erase(itr);
	}
	
}

void RoomInfo::PopUserFromRoom(string uid){
	auto itr = m_roomids.find(uid);
	if (itr != m_roomids.end()){
		string rid = itr->second;
		m_roomids.erase(itr);
		auto itr1 = m_pRooms.find(rid);
		if (itr1 != m_pRooms.end()){
			auto itr2 = itr1->second->_uids.begin();
			for (itr2; itr2 != itr1->second->_uids.end(); itr2++){
				string puid = *itr2;
				if (puid.compare(uid) == 0){
					itr1->second->_uids.erase(itr2);
					break;
				}
			}
			if (itr1->second->_uids.empty()){
				PopRoom(rid);
			}
		}
	}
}

vector<string> RoomInfo::getRoomUsers(string rid){
	vector<string> vec;
	auto itr = m_pRooms.find(rid);
	if (itr != m_pRooms.end()){
		RoomCache *p = itr->second;
		vec = p->_uids;
		return vec;
	}
	return vec;
}