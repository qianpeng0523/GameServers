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

void EventDispatcher::addListener(int cmd, Object *target, EventHandler handler, SERVERTYPE type){
	CallList *clist = new CallList();
	clist->cmd = cmd;
	clist->obj = target;
	clist->handler = handler;
	clist->type = type;

	CallList_Vec vec;
	if (m_eventLists.find(type) != m_eventLists.end()){
		map<int, CallList_Vec>mmp = m_eventLists.at(type);
		if (mmp.find(cmd) != mmp.end()){
			vec = mmp.at(cmd);
			vec.push_back(clist);
			mmp.at(cmd) = vec;
			m_eventLists.at(type) = mmp;
		}
		else{
			vec.push_back(clist);
			mmp.insert(make_pair(cmd, vec));
			m_eventLists.at(type) = mmp;
		}
	}
	else{
		map<int, CallList_Vec>mmp;
		vec.push_back(clist);
		mmp.insert(make_pair(cmd, vec));
		m_eventLists.insert(make_pair(type, mmp));
	}
}

void EventDispatcher::removeListener(int cmd, Object *target, EventHandler handler, SERVERTYPE type){
	CallList_Vec vec;
	if (m_eventLists.find(type) != m_eventLists.end()){
		map<int, CallList_Vec>mmp = m_eventLists.at(type);
		if (mmp.find(cmd) != mmp.end()){
			vec = mmp.at(cmd);
			CallList_Vec::iterator itr = vec.begin();
			while (itr != vec.end()){
				CallList *clist = *itr;
				if (clist && clist->obj == target && clist->handler == handler){
					vec.erase(itr++);
					break;
				}
				else{
					itr++;
				}
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
			CallList_Vec vec = mitr->second;
			CallList_Vec::iterator itr = vec.begin();
			for (itr; itr != vec.end();){
				itr = vec.erase(itr);
			}
			mitr = mmp.erase(mitr);
		}
		eitr = m_eventLists.erase(eitr);
	}
}

void EventDispatcher::disEventDispatcher(ccEvent *event){
	if (event&&event->m_cmd > 0){
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
			int cmd = event->m_cmd;
			SERVERTYPE type = event->m_type;

			if (m_eventLists.find(type) != m_eventLists.end()){
				CallList_Vec vec;
				auto mmp = m_eventLists.at(type);
				if (mmp.find(cmd) != mmp.end()){
					vec = mmp.at(cmd);
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
			}
			if (event){
				delete event;
			}
			ep.erase(itr);
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