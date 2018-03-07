#include "EventListen.h"
#include "EventInfo.h"


EventListen *EventListen::m_ins=NULL;
EventListen *EventListen::getIns() {
	if(!m_ins){
		m_ins=new EventListen();
	}
    return m_ins;
}

EventListen::EventListen(){
	
}


EventListen::~EventListen() {

}

void EventListen::addDataPacketListener(int code, Object *target, EventHandler handler, string typeName){
	if (!typeName.empty()){
		EventInfo::getIns()->pushCMD(code, typeName);
	}
	EventDispatcher::getIns()->addListener(code, target, handler);
}

void EventListen::removeDataPacketListener(int code, Object *target, EventHandler handler){
	EventDispatcher::getIns()->removeListener(code, target, handler);
}

void EventListen::removeAllDataPacketListener(){
	EventDispatcher::getIns()->removeAllKistener();
}