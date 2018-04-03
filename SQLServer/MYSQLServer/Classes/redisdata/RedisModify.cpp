
#include "RedisModify.h"

RedisModify *RedisModify::m_ins=NULL;
RedisModify::RedisModify(){
	
}

RedisModify::~RedisModify(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RedisModify *RedisModify::getIns(){
	if (!m_ins){
		m_ins = new RedisModify();
		m_ins->init();
	}
	return m_ins;
}

void RedisModify::init(){
	
}