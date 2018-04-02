#include "LoginInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "LibEvent.h"
#include "HttpLogic.h"
#include "StatTimer.h"
LoginInfo *LoginInfo::m_shareLoginInfo=NULL;
LoginInfo::LoginInfo()
{
	m_pRedisPut = RedisPut::getIns();
	m_pRedisGet = RedisGet::getIns();
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	CLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName());
	EventListen::getIns()->addDataPacketListener(cl.cmd(), this, Event_Handler(LoginInfo::HandlerCLoginHand));

	CRegister cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName());
	EventListen::getIns()->addDataPacketListener(cl1.cmd(), this, Event_Handler(LoginInfo::HandlerCRegister));

	StatTimer::getIns()->scheduleSelector(this, schedule_selector(LoginInfo::Check), 8.0);
}

LoginInfo::~LoginInfo(){
	StatTimer::getIns()->unscheduleSelector(this, schedule_selector(LoginInfo::Check));
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


	SLogin sl;
	sl.set_cmd(sl.cmd());
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion=uid+pwd+LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
		string ip = data->_ip;
		int len;
		char* pass = m_pRedisGet->getPass(uid);
		if (pass&&data->_sessionID.compare(pass) == 0){
			UserBase *info = m_pRedisGet->getUserBase(uid);
			if (info){
				sl.set_allocated_info(info);
				info->set_ip(ip);
			}
			else{
				sl.set_err(1);
			}
			delete pass;
			pass = NULL;
		}
		else{
			if (pass){
				delete pass;
				pass = NULL;
			}
			sl.set_err(1);
		}
		
	}
	else{
		sl.set_err(1);
	}
	printf("login:uid:%s,pwd:%s",uid.c_str(),pwd.c_str());
	
	SendSLogin(sl,event->m_fd);
}


void LoginInfo::SendSRegister(SRegister cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void LoginInfo::HandlerCRegister(ccEvent *event){
	CRegister cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();
	string uname = cl.uname();

	SRegister sl;
	sl.set_cmd(sl.cmd());
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion = uid + pwd + LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
		
		UserBase *user =m_pRedisGet->getUserBase(uid);
		if (user){
			delete user;
			user = NULL;
			sl.set_err(1);
		}
		else{
			user = sl.mutable_info();
			user->set_userid(uid);
			user->set_username(uname);
			user->set_ip(data->_ip);
			
			m_pRedisPut->PushPass(uid, data->_sessionID);
			bool ist = m_pRedisPut->PushUserBase(*user);
			if (ist){
				sl.set_err(0);
				uint32 gold = user->gold();
				Rank rk;
				rk.set_uid(user->userid());
				m_pRedisPut->PushRank(rk);
				Rank rk1;
				rk1.set_uid(user->userid());
				m_pRedisPut->PushRank(rk1);
			}
			else{
				sl.set_err(1);
				sl.release_info();
			}
		}
	}
	else{
		sl.set_err(1);
	}
	
	SendSRegister(sl, event->m_fd);
}


void LoginInfo::Check(float dt){
	if (!redis::getIns()->isConnect()){
		redis::getIns()->reconnect();
	}
}