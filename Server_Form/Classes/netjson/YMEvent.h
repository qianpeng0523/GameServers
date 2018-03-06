#ifndef __YMEvent_H__
#define __YMEvent_H__

#include "cocos2d.h"
#include <string>

USING_NS_CC;
using namespace std;


class YMEvent {
public:
	/** 改变 **/
	const static string YMEvent_CHANGE;
	const static string YMEvent_RELEASE;

public:
	YMEvent(string type);
	virtual ~YMEvent();

	void setTarget(Object *target);

	cocos2d::Object *getTarget();
	string getType();

	void stopPropagation();
	bool isStopPropagation();

protected:
    string m_type;

private:
	bool blStop;
	cocos2d::Object *m_target;
};

#endif // __YMEvent_H__
