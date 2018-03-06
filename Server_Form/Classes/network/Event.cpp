#include "Event.h"

namespace YS{
	/** 改变 **/
	const string Event::EVENT_CHANGE = "event_change";
	/** 改变 **/
	const string Event::EVENT_RELEASE = "event_release";


	Event::Event(int type) : m_type(type) {
		m_target = NULL;
	}

	Event::~Event() {
		m_target = NULL;
	}


	void Event::setTarget(Object *target) {
		if (!m_target) {
			m_target = target;
		}
	}

	cocos2d::Object *Event::getTarget() {
		return m_target;
	}

	int Event::getType() {
		return m_type;
	}

	void Event::stopPropagation() {
		blStop = true;
	}

	bool Event::isStopPropagation() {
		return blStop;
	}
}