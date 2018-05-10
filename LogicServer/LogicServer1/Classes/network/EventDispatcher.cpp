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

void EventDispatcher::addListener(int cmd, Object *target, EventHandler handler){
	CallList *clist = new CallList();
	clist->cmd = cmd;
	clist->obj = target;
	clist->handler = handler;

	
	CallList_Vec vec;
	if (m_eventLists.find(cmd) != m_eventLists.end()){
		vec = m_eventLists.at(cmd);
		vec.push_back(clist);
		m_eventLists.at(cmd) = vec;
	}
	else{
		vec.push_back(clist);
		m_eventLists.insert(make_pair(cmd,vec));
	}
}

void EventDispatcher::removeListener(int cmd, Object *target, EventHandler handler){
	CallList_Vec vec;
	if (m_eventLists.find(cmd) != m_eventLists.end()){
		vec = m_eventLists.at(cmd);
		CallList_Vec::iterator itr = vec.begin();
		while (itr!=vec.end()){
			CallList *clist = *itr;
			if(clist && clist->obj == target && clist->handler == handler){
				vec.erase(itr++);
				break;
			}
			else{
				itr++;
			}
		}
	}
	
}

void EventDispatcher::removeAllKistener(){
	std::map<int, CallList_Vec>::iterator eitr = m_eventLists.begin();
	while (eitr!=m_eventLists.end()){
		CallList_Vec vec = eitr->second;
		CallList_Vec::iterator itr = vec.begin();
		while (itr != vec.end()){
			vec.erase(itr++);
		}
		m_eventLists.erase(eitr++);
	}
}

void EventDispatcher::disEventDispatcher(ccEvent *event){
	if (event&&event->cmd > 0){
		m_Events.push_back(event);
		while (!m_Events.empty()){
			EventPathch(m_Events);
		}
	}
}

void EventDispatcher::EventPathch(std::vector<ccEvent *> &ep){
	if (!ep.empty()){
		vector<ccEvent *>::iterator itr = ep.begin();
		if (itr != ep.end()){
			ccEvent *event = *itr;
			int cmd = event->cmd;
			CallList_Vec vec;
			if (m_eventLists.find(cmd) != m_eventLists.end()){
				vec = m_eventLists.at(cmd);
				CallList_Vec::iterator itr = vec.begin();
				while (itr != vec.end()){
					CallList *clist = *itr;
					if (clist && clist->obj && clist->handler){
						(clist->obj->*clist->handler)(event);
						itr++;
						break;
					}
					else{
						CLog::log("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
						break;
					}
				}
			}

			delete event;
			ep.erase(itr);
		}
	}
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