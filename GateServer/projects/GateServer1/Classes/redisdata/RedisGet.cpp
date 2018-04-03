
#include "RedisGet.h"
#include "RedisPut.h"
#include "CSVDataInfo.h"

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

vector<Mail> RedisGet::getMail(string uid){
	Mail si;
	std::vector<Message *> vv = m_redis->getList("mail", si.GetTypeName());
	std::vector<Mail > vecs;
	for (int i = 0; i < vv.size(); i++){
		Mail rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<char *> RedisGet::getFriend(string uid){
	vector<int >lens;
	return m_redis->getList(uid, lens);
}

vector<FriendNotice > RedisGet::getFriendNotice(string uid){
	FriendNotice si;
	std::vector<Message *> vv = m_redis->getList("friendnotice"+uid, si.GetTypeName());
	std::vector<FriendNotice > vecs;
	for (int i = 0; i < vv.size(); i++){
		FriendNotice rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<Active > RedisGet::getActive(int type){
	Active si;
	char buff[100];
	sprintf(buff, "active%d", type);
	std::vector<Message *> vv = m_redis->getList(buff, si.GetTypeName());
	std::vector<Active > vecs;
	for (int i = 0; i < vv.size(); i++){
		Active rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<Task > RedisGet::getTask(){
	Task si;
	std::vector<Message *> vv = m_redis->getList("task", si.GetTypeName());
	std::vector<Task > vecs;
	for (int i = 0; i < vv.size(); i++){
		Task rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

Status *RedisGet::getTaskStatus(string uid, int taskid){
	char buff[30];
	sprintf(buff,"task%s%d",uid.c_str(),taskid);
	Status si1;
	Status *si = (Status *)m_redis->getHash(buff, si1.GetTypeName());
	return si;
}

vector<ExAward> RedisGet::getExAward(){
	ExAward si;
	std::vector<Message *> vv = m_redis->getList("exchangereward", si.GetTypeName());
	std::vector<ExAward > vecs;
	for (int i = 0; i < vv.size(); i++){
		ExAward rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<Reward> RedisGet::getReward(){
	Reward si;
	std::vector<Message *> vv = m_redis->getList("reward", si.GetTypeName());
	std::vector<Reward > vecs;
	for (int i = 0; i < vv.size(); i++){
		Reward rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

Reward RedisGet::getReward(int rid){
	vector<Reward> vec=getReward();
	for (int i = 0; i < vec.size();i++){
		Reward rd = vec.at(i);
		if (rd.rid() == rid){
			return rd;
		}
	}
	Reward rd;
	return rd;
}

vector<ExRecord> RedisGet::getExRecord(string uid){
	ExRecord si;
	std::vector<Message *> vv = m_redis->getList("exchangerecord"+uid, si.GetTypeName());
	std::vector<ExRecord > vecs;
	for (int i = 0; i < vv.size(); i++){
		ExRecord rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

int RedisGet::setExRecordStatus(string uid, int rid){
	char buff[50];
	sprintf(buff, "exchangestatus%s%d", uid.c_str(), rid);
	int len = 0;
	return atoi(m_redis->get(buff, len));
}

vector<SignAward> RedisGet::getSignAward(){
	SignAward si;
	std::vector<Message *> vv = m_redis->getList("sign", si.GetTypeName());
	std::vector<SignAward > vecs;
	for (int i = 0; i < vv.size(); i++){
		SignAward rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

int RedisGet::getSignStatus(string uid, int signid){
	char buff[50];
	sprintf(buff, "%s%d", uid.c_str(), signid);
	int len = 0;
	return atoi(m_redis->get(buff,len));
}

SConfig* RedisGet::getSConfig(string uid){
	SConfig sc;
	return (SConfig *)m_redis->getHash("config" + uid, sc.GetTypeName());
}

vector<SignZhuan> RedisGet::getSignZhuan(){
	SignZhuan si;
	std::vector<Message *> vv = m_redis->getList("signzhuan", si.GetTypeName());
	std::vector<SignZhuan > vecs;
	for (int i = 0; i < vv.size(); i++){
		SignZhuan rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<Prop > RedisGet::getProp(){
	Prop si;
	std::vector<Message *> vv = m_redis->getList("prop", si.GetTypeName());
	std::vector<Prop > vecs;
	for (int i = 0; i < vv.size(); i++){
		Prop rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

vector<Task > RedisGet::getFree(){
	Task si;
	std::vector<Message *> vv = m_redis->getList("free", si.GetTypeName());
	std::vector<Task > vecs;
	for (int i = 0; i < vv.size(); i++){
		Task rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}