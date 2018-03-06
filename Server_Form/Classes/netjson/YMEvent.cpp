#include "YMEvent.h"

/** 改变 **/
const string YMEvent::YMEvent_CHANGE = "YMEvent_change";
/** 改变 **/
const string YMEvent::YMEvent_RELEASE = "YMEvent_release";

YMEvent::YMEvent(string type): m_type(type) {
    m_target = NULL;
}

YMEvent::~YMEvent() {
    m_target = NULL;
}


void YMEvent::setTarget(Object *target) {
    if (!m_target) {
        m_target = target;
    }
}

cocos2d::Object *YMEvent::getTarget() {
    return m_target;
}

string YMEvent::getType() {
    return m_type;
}

void YMEvent::stopPropagation() {
    blStop = true;
}

bool YMEvent::isStopPropagation() {
    return blStop;
}