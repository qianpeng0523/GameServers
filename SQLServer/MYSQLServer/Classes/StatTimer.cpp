#include "StatTimer.h"

StatTimer *StatTimer::m_ins=NULL;

StatTimer::StatTimer() : timer(0, ONCETIME), callback(*this, &StatTimer::onTimer), end(false), m_lock(false)
{
}
// timer(0, 2000)，第一个参数默认设置为0,2000代表时间间隔为2秒
// callback的第二个参数指定定时器需要做的具体事情

bool StatTimer::start()
{
	try
	{
		timer.start(callback);   // 启动定时器线程
		//started = true;
	}
	catch (...)
	{
		return false;
		end = true;
	}

	return true;
}

void StatTimer::onTimer(Timer&/*t*/)
{
	StatTimer::getIns()->m_lock = true;
	map<Object *, TimerEvent *> *maps=&StatTimer::getIns()->m_timeevents;
	map<Object *, TimerEvent *>::iterator itr = maps->begin();
	auto itrend = maps->end();
	for (itr; itr != maps->end();){
		TimerEvent *t = itr->second;
		if (t){
			auto itr1 = t->_funs.begin();
			for (itr1; itr1 != t->_funs.end();){
				TimeFun *tf = *itr1;
				if (tf&&!tf->_pause){
					if (t){
						if (*itr1&&tf->_count*ONCETIME >= tf->_interval * 1000){
							tf->_count = 0;
							(t->_target->*tf->_selector)(0);
						}
						tf->_count++;
					}
					itr1++;
				}
				else{
					itr1= t->_funs.erase(itr1);
					delete tf;
					tf = NULL;
				}
				
			}
			if (t->_funs.empty()){
				delete t;
				t = NULL;
				itr = maps->erase(itr);
			}
			else{
				itr++;
			}
		}
		else{
			if (t){
				delete t;
				t = NULL;
			}
			itr=maps->erase(itr);
		}
	}
	StatTimer::getIns()->m_lock = false;
	auto ttir = StatTimer::getIns()->m_timeeventstemp.begin();
	for (ttir; ttir != StatTimer::getIns()->m_timeeventstemp.end();){
		Object *target = ttir->first;
		TimerEvent *t = ttir->second;
		if (StatTimer::getIns()->m_timeevents.find(target) == StatTimer::getIns()->m_timeevents.end()){
			StatTimer::getIns()->m_timeevents.insert(make_pair(target, t));
		}
		else{
			TimerEvent *tt = StatTimer::getIns()->m_timeevents.at(target);
			for (int i = 0; i < t->_funs.size(); i++){
				tt->_funs.push_back(t->_funs.at(i));
			}
			delete t;
			t = NULL;
		}
		ttir = StatTimer::getIns()->m_timeeventstemp.erase(ttir);
	}
}

void StatTimer::stop()
{
	timer.stop();  // 终止定时器线程
}

StatTimer *StatTimer::getIns(){
	if (!m_ins){
		m_ins = new StatTimer();
		m_ins->start();
	}
	return m_ins;
}

void StatTimer::scheduleUpdate(Object *target){
	schedule(target, schedule_selector(Object::update));
}

void StatTimer::schedule(Object *target, SEL_SCHEDULE selector, float interval, float delaytime, bool once){
	auto temp = &m_timeevents;
	if (m_lock){
		temp = &m_timeeventstemp;
	}
	TimerEvent *t = NULL;
	if (temp->find(target) == temp->end()){
		t = new TimerEvent();
		t->_target = target;
		TimeFun *fun = new TimeFun();
		fun->_interval = interval;
		fun->_delaytime = delaytime;
		fun->_selector = selector;
		t->_funs.push_back(fun);
		temp->insert(make_pair(target, t));
	}
	else{
		t = temp->at(target);
		bool ist = false;
		for (int i = 0; i < t->_funs.size(); i++){
			TimeFun *tf = t->_funs.at(i);
			if (tf->_selector == selector){
				ist = true;
			}
		}
		if (!ist){
			TimeFun *fun = new TimeFun();
			fun->_interval = interval;
			fun->_delaytime = delaytime;
			fun->_selector = selector;
			t->_funs.push_back(fun);
			temp->at(target) = t;
		}
	}
}

void StatTimer::unscheduleUpdate(Object *target){
	unschedule(target,schedule_selector(Object::update));
}

void StatTimer::unscheduleSelector(Object *target, SEL_SCHEDULE selector){
	unschedule(target, selector);
}

void StatTimer::unschedule(Object *obj, SEL_SCHEDULE selector){
	if (m_timeevents.find(obj) != m_timeevents.end()){
		TimerEvent *t = m_timeevents.at(obj);
		if (t){
			auto itr = t->_funs.begin();
			for (itr; itr != t->_funs.end();){
				TimeFun *tf = *itr;
				if (tf->_selector == selector){
					tf->_pause = true;
					break;
				}
				else{
					itr++;
				}
			}
			
		}
	}
}

void StatTimer::scheduleSelector(Object *target, SEL_SCHEDULE selector, float interval){
	schedule(target, selector, interval, 0);
}

void StatTimer::scheduleOnce(Object *target, SEL_SCHEDULE selector, float delaytime){
	schedule(target, selector, ONCETIME, delaytime, true);
}