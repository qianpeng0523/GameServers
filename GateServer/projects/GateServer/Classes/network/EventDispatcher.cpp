#include "EventDispatcher.h"
#include "stdafx.h"
#include <event.h>
#include <WinSock2.h>

EventDispatcher* EventDispatcher::m_ins = NULL;

EventDispatcher::EventDispatcher(){
	m_isopen = false;
	m_eventLock = 0;
	
	//m_keycount = -99999;
	//this->schedulUpdate(this,1000);
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
	//if (m_Events.find(m_keycount) == m_Events.end()){
	//	m_Events.insert(make_pair(m_keycount++, event));
	//	m_keycount++;
	//}
	//else{
	//	m_keycount++;
	//}
	if (m_eventLock==0||m_eventLock==3){
		m_Events.push_back(event);
		update(0);
	}
	else if(m_eventLock==1||m_eventLock==2){
		m_Events1.push_back(event);
		update(0);
	}
}

void EventDispatcher::update(float dt){
	EventDis();
}

void EventDispatcher::EventDis(){
	if (m_eventLock == 0){
		if (!m_Events.empty()){
			EventPathch(m_Events, m_eventLock);
		}
	}
	else if (m_eventLock == 2){
		if (!m_Events1.empty()){
			EventPathch(m_Events1, m_eventLock);
		}
	}
}

void EventDispatcher::EventPathch(std::vector<ccEvent *> &ep,int &eventLock){
	if (!ep.empty()){
		if (eventLock == 0){
			eventLock = 1;
		}
		else if (eventLock == 2){
			eventLock =3;
		}
		printf("update!!!!!!!!!!!!!!\n");
		vector<ccEvent *>::iterator itr1 = ep.begin();
		for (itr1; itr1 != ep.end(); itr1++){
			ccEvent *event = *itr1;
			int cmd = event->cmd;
			if (cmd < 0){
				itr1 = ep.erase(itr1);
				continue;
			}
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
						break;
					}
				}
			}

			delete event;
			itr1 = ep.erase(itr1);
			if (ep.empty()){
				if (eventLock == 1){
					eventLock = 2;
				}
				else if (eventLock == 3){
					eventLock = 0;
				}
				break;
			}
		}
		EventDis();
	}
	else{
		EventDis();
	}

}

void EventDispatcher::AddUpdate(Object *obj, float dt){
	while (m_isopen){
		Sleep(dt);
		obj->update(dt);
	}
}

void EventDispatcher::schedulUpdate(Object *obj, float dt){
	if (!m_isopen){
		m_isopen = true;
		AddUpdate(obj,dt);
	}
}

void EventDispatcher::unschulUpdate(Object *obj, float dt){
	if (m_isopen){
		m_isopen = false;
	}
}