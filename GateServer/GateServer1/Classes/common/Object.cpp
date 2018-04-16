#include "Object.h"

Object::Object(){
	m_retainCount = 0;
}

Object::~Object() {
		
}

void Object::update(float dt){
	//printf("eeeeeee\n");
}

void Object::retain(){
	m_retainCount++;
}

void Object::release(){
	m_retainCount--;
	if (m_retainCount <= 0){
		delete this;
	}
}