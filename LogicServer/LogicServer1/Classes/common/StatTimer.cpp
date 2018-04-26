#include "StatTimer.h"

StatTimer *StatTimer::m_ins=NULL;

StatTimer::StatTimer() : timer(0, ONCETIME), callback(*this, &StatTimer::onTimer), end(false)
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
	map<Object *, TimerEvent *> *maps=&StatTimer::getIns()->m_timeevents;
	map<Object *, TimerEvent *>::iterator itr = maps->begin();
	for (itr; itr != maps->end();itr++){
		TimerEvent *t = itr->second;
		if (itr->first){
			for (int i = 0; i < t->_funs.size(); i++){
				TimeFun *tf = t->_funs.at(i);
				if (!tf->_pause){
					if (t){
						if (tf->_count*ONCETIME >= tf->_interval * 1000){
							tf->_count = 0;
							(t->_target->*tf->_selector)(0);
						}
						tf->_count++;
					}
				}
				else{
					t->_funs.erase(t->_funs.begin()+i);
					delete tf;
					tf = NULL;
				}
				
			}
		}
		else{
			if (t){
				delete t;
				t = NULL;
			}
			maps->erase(itr++);
		}
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
	TimerEvent *t = NULL;
	if (m_timeevents.find(target) == m_timeevents.end()){
		t = new TimerEvent();
		t->_target = target;
		TimeFun *fun = new TimeFun();
		fun->_interval = interval;
		fun->_delaytime = delaytime;
		fun->_selector = selector;
		t->_funs.push_back(fun);
		m_timeevents.insert(make_pair(target, t));
	}
	else{
		t = m_timeevents.at(target);
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
			m_timeevents.at(target) = t;
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
			for (int i = 0; i < t->_funs.size();i++){
				TimeFun *tf = t->_funs.at(i);
				if (tf->_selector == selector){
					tf->_pause = true;
					break;
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