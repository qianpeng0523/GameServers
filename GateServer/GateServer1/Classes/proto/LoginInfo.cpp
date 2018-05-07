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


	SLogin sl;
	sl.set_cmd(sl.cmd());
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion=uid+pwd+LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
		data->_uid = uid;
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
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			string uid = ub1->userid();
			string seesion = uid + pwd + LOGIC_TOKEN;
			MD5 md5;
			md5.update(seesion);
			data->_sessionID = md5.toString();
			data->_uid = uid;
			ub1->set_ip(data->_ip);
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
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			string uid = ub1->userid();
			data->_sessionID = uid + token + LOGIC_TOKEN;
			data->_uid = uid;
		}
	}
	SendSWXLogin(sl, event->m_fd);
}

void LoginInfo::eraseClientData(int fd){
	LibEvent::getIns()->eraseClientData(fd);
}