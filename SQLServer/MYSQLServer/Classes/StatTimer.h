#ifndef __STARTTIMER_H__
#define __STARTTIMER_H__

#include <stdio.h>
#include <iostream>
#include <string>
#include "Poco/Timer.h"
#include "Object.h"
#include <map>
#include <vector>

using namespace std;
using Poco::Timer;         // 使用Timer基类的成员
using Poco::TimerCallback; // 使用TimerCallback基类的成员

#define ONCETIME 1000/60.0

struct TimeFun
{
	TimeFun():
	_delaytime(0),
	_interval(ONCETIME),
	_pause(false),
	_count(0),
	_once(false)
	{

	}
	float _delaytime;
	float _interval;
	bool _pause;
	float _count;
	SEL_SCHEDULE _selector;
	bool _once;
};

struct TimerEvent
{
	TimerEvent():
	_target(NULL)
	{

	}
	Object *_target;
	std::vector< TimeFun *>_funs;
	
};

class StatTimer
{
public:
	StatTimer();
	bool start();
	void onTimer(Timer& t);
	void stop();

	void scheduleUpdate(Object *target);
	void unscheduleUpdate(Object *target);
	void scheduleSelector(Object *target, SEL_SCHEDULE selector, float interval);
	void scheduleOnce(Object *target, SEL_SCHEDULE selector,float delaytime);
	void unscheduleSelector(Object *target,SEL_SCHEDULE selector);


	static StatTimer *getIns();
private:
	void schedule(Object *obj, SEL_SCHEDULE selector, float interval = ONCETIME, float delaytime = 0, bool once = false);
	void unschedule(Object *obj, SEL_SCHEDULE selector);
private:
	static StatTimer *m_ins;
	bool end;
	TimerCallback<StatTimer> callback;  // 套用模板，将StatTimer类关联起来
	Timer timer;
	map<Object *, TimerEvent *>m_timeevents;
};
#endif // __Common_H__
