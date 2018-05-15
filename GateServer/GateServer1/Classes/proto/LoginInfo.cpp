#include "LoginInfo.h"
#include "ClientSocket.h"

#include "EventDispatcher.h"
#include "LibEvent.h"
#include "HttpLogic.h"
#include "StatTimer.h"
#include "HttpWXLogin.h"

LoginInfo *LoginInfo::m_shareLoginInfo=NULL;
LoginInfo::LoginInfo()
{
	m_pRedisPut = RedisPut::getIns();
	m_pRedisGet = RedisGet::getIns();
	EventDispatcher *pe = EventDispatcher::getIns();
	CLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName(), GAME_TYPE);
	pe->addListener(cl.cmd(), this, Event_Handler(LoginInfo::HandlerCLoginHand),GAME_TYPE);

	CRegister cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName(), GAME_TYPE);
	pe->addListener(cl1.cmd(), this, Event_Handler(LoginInfo::HandlerCRegister), GAME_TYPE);

	CWXLogin cl2;
	pe->registerProto(cl2.cmd(), cl2.GetTypeName(), GAME_TYPE);
	pe->addListener(cl2.cmd(), this, Event_Handler(LoginInfo::HandlerCWXLogin), GAME_TYPE);

	
}

LoginInfo::~LoginInfo(){
	openCheckUpdate(false);
}

LoginInfo* LoginInfo::getIns(){
	if (!m_shareLoginInfo){
		m_shareLoginInfo = new LoginInfo();
		m_shareLoginInfo->init();
	}
	return m_shareLoginInfo;
}

bool LoginInfo::init()
{
	
    return true;
}

void LoginInfo::SendSLogin(SLogin cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(),&cl,fd);
}

void LoginInfo::HandlerCLoginHand(ccEvent *event){
	CLogin cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();
	
	string pwd1 = m_pRedisGet->getPass(uid);
	SLogin sl;
	if (!pwd1.empty()){
		sl.set_cmd(sl.cmd());
		if (pwd.compare(pwd1) == 0){
			ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
			if (data){
				string seesion = uid + pwd + LOGIC_TOKEN;
				MD5 md5;
				md5.update(seesion);
				data->_sessionID = md5.toString();
				data->_uid = uid;
				string ip = data->_ip;
				UserBase *info = m_pRedisGet->getUserBase(uid);
				if (info){
					UserBase *info1 = sl.mutable_info();
					info1->CopyFrom(*info);
					info->set_ip(ip);
					PushUserBase(info);

					inserUserBase(uid, info, true);
				}
				else{
					sl.set_err(1);
				}
			}
			else{
				sl.set_err(1);
			}
		}
		else{
			sl.set_err(1);
		}
	}
	else{
		sl.set_err(1);
	}
	CLog::log("login:uid:%s,pwd:%s",uid.c_str(),pwd.c_str());
	
	SendSLogin(sl,event->m_fd);
}


void LoginInfo::SendSRegister(SRegister cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void LoginInfo::HandlerCRegister(ccEvent *event){
	CRegister cl;
	cl.CopyFrom(*event->msg);
	
	string pwd = cl.pwd();
	string uname = cl.uname();

	SRegister sl;
	sl.set_cmd(sl.cmd());

	UserBase ub = HttpWXLogin::getIns()->getUserinfo(uname,pwd);
	if (ub.userid().empty()){
		sl.set_err(1);
	}
	else{
		UserBase *ub1 = sl.mutable_info();
		ub1->CopyFrom(ub);
		PushUserBase(ub1);
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			string uid = ub1->userid();
			string seesion = uid + pwd + LOGIC_TOKEN;
			MD5 md5;
			md5.update(seesion);
			data->_sessionID = md5.toString();
			data->_uid = uid;
			ub1->set_ip(data->_ip);

			inserUserBase(uid, RedisGet::getIns()->getUserBase(uid), true);
		}
		else{
			sl.set_err(1);
		}
	}

	SendSRegister(sl, event->m_fd);
}


void LoginInfo::Check(float dt){
	if (!redis::getIns()->isConnect()){
		redis::getIns()->reconnect();
	}
}

void LoginInfo::openCheckUpdate(bool isopen){
	if (isopen){
		StatTimer::getIns()->scheduleSelector(this, schedule_selector(LoginInfo::Check), 8.0);
	}
	else{
		StatTimer::getIns()->unscheduleSelector(this, schedule_selector(LoginInfo::Check));
	}
}

void LoginInfo::SendSWXLogin(SWXLogin sl, int fd){
	LibEvent::getIns()->SendData(sl.cmd(), &sl, fd);
}

void LoginInfo::HandlerCWXLogin(ccEvent *event){
	CWXLogin cl;
	cl.CopyFrom(*event->msg);
	string code = cl.code();
	string token = cl.token();

	SWXLogin sl;
	sl.set_cmd(sl.cmd());
	UserBase ub = HttpWXLogin::getIns()->requestWXLogin(code,token);
	if (ub.userid().empty()){
		sl.set_err(1);
	}
	else{
		sl.set_token(token);
		UserBase *ub1= sl.mutable_info();
		ub1->CopyFrom(ub);
		PushUserBase(ub1);
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			string uid = ub1->userid();
			data->_sessionID = uid + token + LOGIC_TOKEN;
			data->_uid = uid;
		}
		inserUserBase(ub1->userid(),ub1,true);
	}
	SendSWXLogin(sl, event->m_fd);
}

void LoginInfo::eraseClientData(int fd){
	LibEvent::getIns()->eraseClientData(fd);
}

void LoginInfo::PushUserBase(UserBase *user){
	string uid = user->userid();
	if (m_UserBases.find(uid)==m_UserBases.end()){
		UserBase *u = (UserBase *)ccEvent::create_message(user->GetTypeName());
		u->CopyFrom(*user);
		m_UserBases.insert(make_pair(uid, u));
	}
}

UserBase *LoginInfo::getUserBase(string uid){
	if (m_UserBases.find(uid) != m_UserBases.end()){
		return m_UserBases.at(uid);
	}
	return NULL;
}

void LoginInfo::onLineUsers(){
	auto vec = RedisGet::getIns()->getUserBases();
	auto itr = vec.begin();
	LibEvent *p = LibEvent::getIns();
	for (itr; itr != vec.end(); itr++){
		string uid = itr->first;
		if (p->isHave(uid)){
			m_pOnlineUsers.insert(make_pair(uid, itr->second));
		}
		else{
			m_pOfflineUsers.insert(make_pair(uid, itr->second));
		}
	}
}

void LoginInfo::eraseUserBase(string uid, bool online){
	if (online){
		if (m_pOnlineUsers.find(uid) != m_pOnlineUsers.end()){
			m_pOnlineUsers.erase(m_pOnlineUsers.find(uid));
		}
	}
	else{
		if (m_pOfflineUsers.find(uid) != m_pOfflineUsers.end()){
			m_pOfflineUsers.erase(m_pOfflineUsers.find(uid));
		}
	}
}

void LoginInfo::inserUserBase(string uid, UserBase *ub, bool online){
	if (online){
		if (m_pOnlineUsers.find(uid) == m_pOnlineUsers.end()){
			m_pOnlineUsers.insert(make_pair(uid,ub));
		}
	}
	else{
		if (m_pOfflineUsers.find(uid) == m_pOfflineUsers.end()){
			m_pOfflineUsers.insert(make_pair(uid, ub));
		}
	}
	eraseUserBase(uid, !online);
}

map<string, UserBase *> LoginInfo::getOnLineUser(bool online){
	return online ? m_pOnlineUsers : m_pOfflineUsers;
}