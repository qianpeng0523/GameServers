#include "EventDispatcher.h"
#include "stdafx.h"
#include <event.h>
#include <WinSock2.h>

EventDispatcher* EventDispatcher::m_ins = NULL;

EventDispatcher::EventDispatcher(){
	m_lock = false;
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

void EventDispatcher::addListener(int cmd, Object *target, EventHandler handler, SERVERTYPE type){
	

	if (m_eventLists.find(type) != m_eventLists.end()){
		map<int, CallList *>mmp = m_eventLists.at(type);
		if (mmp.find(cmd) == mmp.end()){
			CallList *clist = new CallList();
			clist->cmd = cmd;
			clist->obj = target;
			clist->handler = handler;
			clist->type = type;

			mmp.insert(make_pair(cmd, clist));
			m_eventLists.at(type) = mmp;
		}
	}
	else{
		CallList *clist = new CallList();
		clist->cmd = cmd;
		clist->obj = target;
		clist->handler = handler;
		clist->type = type;

		map<int, CallList *>mmp;
		mmp.insert(make_pair(cmd, clist));
		m_eventLists.insert(make_pair(type, mmp));
	}
}

void EventDispatcher::removeListener(int cmd, Object *target, EventHandler handler, SERVERTYPE type){
	if (m_eventLists.find(type) != m_eventLists.end()){
		map<int, CallList *>mmp = m_eventLists.at(type);
		if (mmp.find(cmd) != mmp.end()){
			CallList *clist = mmp.at(cmd);
			if (clist && clist->obj == target && clist->handler == handler){
				mmp.erase(mmp.find(cmd));
			}
		}
	}

}

void EventDispatcher::removeAllListener(){
	auto eitr = m_eventLists.begin();
	for (eitr; eitr != m_eventLists.end();){
		auto mmp = eitr->second;
		auto mitr = mmp.begin();
		for (mitr; mitr != mmp.end();){
			CallList *clist = mitr->second;
			delete clist;
			mitr = mmp.erase(mitr);
		}
		eitr = m_eventLists.erase(eitr);
	}
}

void EventDispatcher::disEventDispatcher(ccEvent *event){
	if (event&&event->m_cmd > 0){
		m_Events.push_back(event);
		if (!m_lock){
			m_lock = true;
			EventPathch();
			m_lock = false;
		}
	}
}

void EventDispatcher::EventPathch(){
	while (!m_Events.empty()){
		auto itr = m_Events.begin();
		if (itr != m_Events.end()){
			ccEvent *event = *itr;
			int cmd = event->m_cmd;
			SERVERTYPE type = event->m_type;

			if (m_eventLists.find(type) != m_eventLists.end()){
				auto mmp = m_eventLists.at(type);
				if (mmp.find(cmd) != mmp.end()){
					CallList *clist = mmp.at(cmd);
					if (clist && clist->obj && clist->handler){
						(clist->obj->*clist->handler)(event);
					}
				}
			}
			
			m_Events.erase(itr);
			if (event){
				delete event;
				event = NULL;
			}
		}
	}
}

void EventDispatcher::registerProto(int cmd, string tname, SERVERTYPE type){
	if (m_protos.find(type) == m_protos.end()){
		map<int, string>mmp;
		mmp.insert(make_pair(cmd, tname));
		m_protos.insert(make_pair(type, mmp));
	}
	else{
		map<int, string>mmp = m_protos.at(type);
		mmp.insert(make_pair(cmd, tname));
		m_protos.at(type) = mmp;
	}
}

string EventDispatcher::getProtoName(int cmd, SERVERTYPE type){
	if (m_protos.find(type) != m_protos.end()){
		map<int, string>mmp = m_protos.at(type);
		if (mmp.find(cmd) != mmp.end()){
			return mmp.at(cmd);
		}
	}
	return "";
}