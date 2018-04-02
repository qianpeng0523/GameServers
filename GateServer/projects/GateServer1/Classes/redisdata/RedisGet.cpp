
#include "RedisGet.h"

RedisGet *RedisGet::m_ins=NULL;
RedisGet::RedisGet(){
	m_redis = redis::getIns();
}

RedisGet::~RedisGet(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RedisGet *RedisGet::getIns(){
	if (!m_ins){
		m_ins = new RedisGet();
		m_ins->init();
	}
	return m_ins;
}

void RedisGet::init(){
	
}

char* RedisGet::getPass(string uid){
	int len;
	return m_redis->get("pass"+uid,len);
}

UserBase *RedisGet::getUserBase(string uid){
	UserBase ub;
	return (UserBase *)m_redis->getHash(ub.GetTypeName() + uid,ub.GetTypeName());
}

std::vector<Rank > RedisGet::getRank(int type, int index){
	char buff[50];
	Rank rk;
	sprintf(buff,"%s%d",rk.GetTypeName().c_str(),type);
	std::vector<Message *> vv = m_redis->getList(buff, rk.GetTypeName(), 10 * index, 10 * (index + 1));
	std::vector<Rank > vecs;
	for (int i = 0; i < vv.size();i++){
		Rank rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

std::vector<ShopItem > RedisGet::getShop(){
	ShopItem si;
	std::vector<Message *> vv = m_redis->getList("shop",si.GetTypeName());
	std::vector<ShopItem > vecs;
	for (int i = 0; i < vv.size(); i++){
		ShopItem rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}