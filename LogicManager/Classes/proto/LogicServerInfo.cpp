#include "LogicServerInfo.h"
#include "LibEvent.h"

LogicServerInfo *LogicServerInfo::m_shareLogicServerInfo=NULL;
LogicServerInfo::LogicServerInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CLogicLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName(), NO_TYPE);
	pe->addListener(cl.cmd(), this, Event_Handler(LogicServerInfo::HandlerCLogicLoginHand), NO_TYPE);

	CGateLogin cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName(), NO_TYPE);
	pe->addListener(cl1.cmd(), this, Event_Handler(LogicServerInfo::HandlerCGateLoginHand), NO_TYPE);
}

LogicServerInfo::~LogicServerInfo(){
	
}

LogicServerInfo* LogicServerInfo::getIns(){
	if (!m_shareLogicServerInfo){
		m_shareLogicServerInfo = new LogicServerInfo();
		m_shareLogicServerInfo->init();
	}
	return m_shareLogicServerInfo;
}

bool LogicServerInfo::init()
{
	
    return true;
}

void LogicServerInfo::SendSLogicLogin(int fd,int err){
	SLogicLogin sl;
	sl.set_err(err);
	
	LibEvent::getIns()->SendData(sl.cmd(), &sl, fd);
}

void LogicServerInfo::HandlerCLogicLoginHand(ccEvent *event){
	CLogicLogin cl;
	cl.CopyFrom(*event->msg);
	string servername = cl.servername();
	if (m_name_type.find(servername) == m_name_type.end()){
		if (!servername.empty()){
			ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
			if (data&&data->_conn){
				data->_conn->_servername = servername;
			}
		}
		string seession = cl.seession();
		if (seession.compare(LOGIC_TOKEN) == 0){
			m_gamefds.insert(make_pair(LOGIC_TYPE, event->m_fd));
			m_name_type.insert(make_pair(servername, LOGIC_TYPE));
			SendSLogicLogin(event->m_fd, 0);

		}
		else{
			SendSLogicLogin(event->m_fd, 1);
		}
	}
	else{
		SendSLogicLogin(event->m_fd, 1);
		LibEvent::getIns()->eraseClientData(event->m_fd);
	}
}

void LogicServerInfo::SendSGateLogin(int fd, int err){
	SGateLogin sl;
	sl.set_err(err);

	LibEvent::getIns()->SendData(sl.cmd(), &sl, fd);
}

void LogicServerInfo::HandlerCGateLoginHand(ccEvent *event){
	CGateLogin cl;
	cl.CopyFrom(*event->msg);
	string servername = cl.servername();
	if (m_name_type.find(servername) == m_name_type.end()){
		if (!servername.empty()){
			ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
			if (data&&data->_conn){
				data->_conn->_servername = servername;
			}
		}
		string seession = cl.seession();
		if (seession.compare(LOGIC_TOKEN) == 0){
			m_name_type.insert(make_pair(servername, GATE_TYPE));
			m_gamefds.insert(make_pair(GATE_TYPE, event->m_fd));
			SendSGateLogin(event->m_fd, 0);
		}
		else{
			SendSGateLogin(event->m_fd, 1);
		}
	}
	else{
		SendSGateLogin(event->m_fd, 1);
		LibEvent::getIns()->eraseClientData(event->m_fd);
	}
}

int LogicServerInfo::getFd(SERVERTYPE type){
	if (m_gamefds.find(type) != m_gamefds.end()){
		return m_gamefds.at(type);
	}
	return -1;
}

SERVERTYPE LogicServerInfo::getServerType(string servername){
	if (m_name_type.find(servername) != m_name_type.end()){
		return m_name_type.at(servername);
	}
	return NO_TYPE;
}

int LogicServerInfo::getFd(string servername){
	SERVERTYPE type = getServerType(servername);
	return getFd(type);
}

void LogicServerInfo::eraseFds(int fd){
	auto itr = m_gamefds.begin();
	for (itr; itr != m_gamefds.end();itr++){
		if (itr->second == fd){
			SERVERTYPE type = itr->first;
			auto itr1 = m_name_type.begin();
			for (itr1; itr1 != m_name_type.end(); itr1++){
				if (itr1->second == type){
					m_name_type.erase(itr1);
					break;
				}
			}
			m_gamefds.erase(itr);
			return;
		}
	}
}

void LogicServerInfo::eraseClientData(int fd){
	LibEvent::getIns()->eraseClientData(fd);
}