
#include "RedisPut.h"

RedisPut *RedisPut::m_ins=NULL;
RedisPut::RedisPut(){
	m_redis = redis::getIns();
}

RedisPut::~RedisPut(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RedisPut *RedisPut::getIns(){
	if (!m_ins){
		m_ins = new RedisPut();
		m_ins->init();
	}
	return m_ins;
}

void RedisPut::init(){
	
}

bool RedisPut::PushUserBase(UserBase ub){
	string uid = ub.userid();
	return m_redis->Hash(ub.GetTypeName() + uid, &ub);
}

bool RedisPut::PushRank(Rank rk){
	int type = rk.type();
	char buff[50];
	sprintf(buff,"%s%d",rk.GetTypeName(),type);
	return m_redis->List(buff,&rk);
}

bool RedisPut::PushPass(string uid, string pass){
	return m_redis->set("pass" + uid, pass);
}

bool RedisPut::PushShop(ShopItem item){
	return m_redis->List("shop",&item);
}