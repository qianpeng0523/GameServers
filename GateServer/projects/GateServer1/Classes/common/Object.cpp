#include "Object.h"

Object::Object(){
	m_ischedule = false;
	m_timer = NULL;
}

Object::~Object() {
	if (m_timer){
		delete m_timer;
		m_timer = NULL;
	}
}

void Object::update(float dt){
	//printf("eeeeeee\n");
}

void Object::run(){
	update(1.0 / 60);
}

void Object::scheduleUpdate(float delay, float duration){
	if (!m_ischedule){
		m_ischedule = true;
		if (!m_timer){
			m_timer = new Poco::Util::Timer();
		}
		float dl = 1000.0*delay;
		float dur = 1000.0*duration;
		m_timer->schedule(this,dl,dur);
	}
}

void Object::unscheduleUpdate(){
	if (m_ischedule){
		m_ischedule = false;
		if (m_timer){
			m_timer->cancel();
		}
	}
}