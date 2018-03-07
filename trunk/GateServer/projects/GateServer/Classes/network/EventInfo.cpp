#include "EventInfo.h"

EventInfo *EventInfo::m_ins=NULL;

EventInfo::EventInfo(){
	
}

EventInfo::~EventInfo(){
	
}

bool EventInfo::init()
{
	
    return true;
}

EventInfo* EventInfo::getIns(){
	if (!m_ins){
		m_ins = new EventInfo();
		m_ins->init();
	}
	return m_ins;
}



void EventInfo::pushCMD(int cmd, string type_name){
	if (!type_name.empty()){
		if (m_cmds.find(cmd) != m_cmds.end()){
			m_cmds.at(cmd) = type_name;
		}
		else{
			m_cmds.insert(make_pair(cmd, type_name));
		}
	}
}

string EventInfo::getTypeName(int cmd){
	if (m_cmds.find(cmd) != m_cmds.end()){
		return m_cmds.at(cmd);
	}
	return "";
}