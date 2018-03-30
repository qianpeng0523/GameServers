#include "EventDispatcher.h"
#include "stdafx.h"
#include <event.h>
#include <WinSock2.h>

EventDispatcher* EventDispatcher::m_ins = NULL;

EventDispatcher::EventDispatcher(){
	
}

EventDispatcher::~EventDispatcher(){
	
}

bool EventDispatcher::init()
{
	
    return true;
}

EventDispatcher *EventDispatcher::getIns(){
	if (!m_ins){
		m_ins = new EventDispatcher();
		m_ins->init();
	}
	return m_ins;
}

void EventDispatcher::registerProto(int cmd, string tname){
	if (m_protos.find(cmd) == m_protos.end()){
		m_protos.insert(make_pair(cmd,tname));
	}
}

string EventDispatcher::getProtoName(int cmd){
	if (m_protos.find(cmd) != m_protos.end()){
		return m_protos.at(cmd);
	}
	return "";
}