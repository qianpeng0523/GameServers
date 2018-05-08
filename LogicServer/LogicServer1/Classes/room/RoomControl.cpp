
#include "RoomControl.h"
#include "ConfigData.h"
RoomControl *RoomControl::m_ins=NULL;
RoomControl::RoomControl(){
	
}

RoomControl::~RoomControl(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RoomControl *RoomControl::getIns(){
	if (!m_ins){
		m_ins = new RoomControl();
		m_ins->init();
	}
	return m_ins;
}

void RoomControl::init(){
	char buff[10];
	for (int i = 0; i < 1000000; i++){
		sprintf(buff,"%06d",i);
		m_roomids.insert(make_pair(buff,false));
	}
}

string RoomControl::createRoom(){
	string rid;
	while (1){
		int rd = rand() % 1000000;
		char buff[10];
		sprintf(buff, "%06d", rd);
		auto itr = m_roomids.find(buff);
		if (itr != m_roomids.end() && !itr->second){
			itr->second = true;
			rid = buff;
			break;
		}
	}
	return rid;
}

GRoom *RoomControl::createRoom(string uid, int type, int ante, int round, int bao, int bang){
	GRoom *gr=new GRoom();
	RoomData rd;
	rd.set_ante(ante);
	rd.set_type(type);
	rd.set_round(round);
	rd.set_bao(bao);
	rd.set_bang(bang);
	rd.set_uid(uid);
	string rid = createRoom();
	rd.set_roomid(rid);
	if (m_roomdatas.find(rid) == m_roomdatas.end()){
		m_roomdatas.insert(make_pair(rid,gr));
	}
	UData *ud=new UData();
	ud->init();
	ud->_uid = uid;
	gr->PushUData(ud);
	gr->setRoomData(rd);
	gr->setFangZhuUid(uid);
	return gr;
}

GRoom *RoomControl::enterRoom(string uid, string rid){
	GRoom *gr = getGRoom(rid);
	if (gr){
		if (!gr->getUData(uid)){
			UData *ud = new UData();
			ud->init();
			ud->_uid = uid;
			gr->PushUData(ud);
			m_roomdatas.at(rid) = gr;
		}
		return gr;
	}
	return NULL;
}

void RoomControl::eraseRoom(string rid){
	auto itr = m_roomdatas.find(rid);
	if (itr != m_roomdatas.end()){
		delete itr->second;
		itr->second = NULL;
		m_roomdatas.erase(itr);
	}
}

bool RoomControl::BeginMJ(int type, string uid){
	GRoom *gr = getGRoom_(uid);
	if (gr){
		gr->Begin(uid, type);
		return true;
	}
	return false;
}

bool RoomControl::ReadyMJ(string uid, bool ready){
	printf("11111\n");
	GRoom *gr = getGRoom_(uid);
	if (gr){
		printf("2222\n");
		gr->Ready(uid, ready);
		return true;
	}
	return false;
}

bool RoomControl::LeaveMJ(string uid){
	GRoom *gr = getGRoom_(uid);
	if (gr){
		gr->Leave(uid);
		return true;
	}
	return false;
}

bool RoomControl::onLine(string uid, bool online){
	GRoom *gr = getGRoom_(uid);
	if (gr){
		gr->onLine(uid,online);
		return true;
	}
	return false;
}

bool RoomControl::DissolveRoom(string uid, string rid){
	GRoom *gr = getGRoom(rid);
	if (gr){
		gr->DissolveRoom(uid);
	}
	return true;
}

bool RoomControl::Vote(string uid, bool agree){
	GRoom *gr = getGRoom_(uid);
	if (gr){
		gr->Agree(uid, agree);
		return true;
	}
	return false;
}

GRoom *RoomControl::getGRoom_(string uid){
	auto itr = m_roomdatas.begin();
	for (itr; itr != m_roomdatas.end(); itr++){
		GRoom *gr = itr->second;
		if (gr->getUData(uid)){
			return gr;
		}
	}
	return NULL;
}

GRoom *RoomControl::getGRoom(string rid){
	auto itr = m_roomdatas.find(rid);
	if (itr != m_roomdatas.end()){
		GRoom *gr = itr->second;
		if (gr){
			return gr;
		}
	}
	return NULL;
}

void RoomControl::test(){
	GRoom *gr = createRoom("123456", 1, 1, 8, g_all_mjkind[ rand() % 34],1);

	string rid = gr->getRoomData().roomid();
	enterRoom("123457", rid);
	enterRoom("123458", rid);
	enterRoom("123459", rid);
	gr->Begin("123456",1);
}