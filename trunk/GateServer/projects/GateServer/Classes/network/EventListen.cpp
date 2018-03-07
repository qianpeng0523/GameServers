#include "EventListen.h"
#include "EventDispatcher.h"


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

void EventListen::addDataPacketListener(int cmd, Object *target, EventHandler handler, string typeName){
	EventDispatcher *p = EventDispatcher::getIns();
	if (!typeName.empty()){
		p->registerProto(cmd, typeName);
	}
	EventDispatcher::getIns()->addListener(cmd, target, handler);
}

void EventListen::removeDataPacketListener(int cmd, Object *target, EventHandler handler){
	EventDispatcher::getIns()->removeListener(cmd, target, handler);
}

void EventListen::removeAllDataPacketListener(){
	EventDispatcher::getIns()->removeAllKistener();
}