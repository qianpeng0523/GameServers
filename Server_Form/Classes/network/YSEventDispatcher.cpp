#include "YSEventDispatcher.h"

YSEventDispatcher* YSEventDispatcher::m_ins = NULL;

YSEventDispatcher::YSEventDispatcher(){
	m_eventLock = false;
	m_islock = false;
	m_curClist = NULL;
	
	CCDirector::sharedDirector()->getScheduler()->scheduleUpdate(this, 1, false);
}

YSEventDispatcher::~YSEventDispatcher(){
	CCDirector::sharedDirector()->getScheduler()->unscheduleUpdate(this);
}

bool YSEventDispatcher::init()
{
	
    return true;
}

YSEventDispatcher *YSEventDispatcher::getIns(){
	if (!m_ins){
		m_ins = new YSEventDispatcher();
		m_ins->init();
	}
	return m_ins;
}

void YSEventDispatcher::addListener(int cmd, Object *target, EventHandler handler){
	CallList *clist = new CallList();
	clist->cmd = cmd;
	clist->obj = target;
	clist->handler = handler;

	
	CallList_Vec vec;
	if (m_eventLists.find(cmd) != m_eventLists.end()){
		vec = m_eventLists.at(cmd);
		vec.insert(make_pair(cmd,clist));
		m_eventLists.at(cmd) = vec;
	}
	else{
		vec.insert(make_pair(cmd, clist));
		m_eventLists.insert(make_pair(cmd,vec));
	}
}

void YSEventDispatcher::removeListener(int cmd, Object *target, EventHandler handler){
	CallList_Vec vec;
	if (m_eventLists.find(cmd) != m_eventLists.end()){
		vec = m_eventLists.at(cmd);
		if (vec.find(cmd) != vec.end()){
			vec.erase(vec.find(cmd));
		}
// 		CallList_Vec::iterator itr = vec.begin();
// 		while (itr!=vec.end()){
// 			CallList *clist = *itr;
// 			if(clist && clist->obj == target && clist->handler == handler){
// 				vec.erase(itr++);
// 				//break;
// 			}
// 			else{
// 				itr++;
// 			}
// 		}
	}
	
}

void YSEventDispatcher::removeAllKistener(){
	m_eventLists.clear();
}

void YSEventDispatcher::disEventDispatcher(YSEvent *event){
	m_Events.push_back(event);
	EventPathch();
}

void YSEventDispatcher::EventPathch(){
	if (!m_eventLock){
		if (!m_Events.empty()){
			m_eventLock = true;
			vector<YSEvent *>::iterator itr1 = m_Events.begin();
			for (itr1; itr1 != m_Events.end(); itr1++){
				YSEvent *event = *itr1;
				int cmd = event->cmd;
				CallList_Vec vec;
				if (m_eventLists.find(cmd) != m_eventLists.end()){
					vec = m_eventLists.at(cmd);
					CallList_Vec::iterator itr = vec.begin();
					while (itr != vec.end()){
						CallList *clist1 = itr->second;
						CallList *clist = new CallList();
						clist->Copy(clist1);
						if (clist && clist->obj && clist->handler){
							clist->evt->Copy(event);
							m_pCallListVecs.push_back(clist);
							//(clist->obj->*clist->handler)(event);
							itr++;
							break;
						}
					}
				}
				CC_SAFE_DELETE(event);

				itr1 = m_Events.erase(itr1);
				if (m_Events.empty()){
					m_eventLock = false;
					break;
				}
			}
			EventPathch();
		}
	}

}

void YSEventDispatcher::update(float dt){
	if (!m_islock){
		if (!m_pCallListVecs.empty()){
			m_curClist = *m_pCallListVecs.begin();
			if (m_curClist){
				m_islock = true;
				int cmd = m_curClist->cmd;
				CallBack(0);
// 				float delayt = ProtoDelayConfig::shareProtoDelayConfig()->getTime(cmd);
// 				CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(YSEventDispatcher::CallBack), this,delayt, false);
			}
			else{
				//CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YSEventDispatcher::CallBack), this);
				m_islock = false;
			}
		}
	}
}

void YSEventDispatcher::CallBack(float dt){
	if (m_curClist && m_curClist->obj && m_curClist->handler&&m_curClist->evt){
		(m_curClist->obj->*m_curClist->handler)(m_curClist->evt);
	}
	std::vector<CallList *>::iterator itr = m_pCallListVecs.begin();
	CallList *clist = *itr;
	CC_SAFE_DELETE(clist);
	m_pCallListVecs.erase(itr);
	if (m_islock){
		CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(YSEventDispatcher::CallBack), this);
		m_islock = false;
	}
}