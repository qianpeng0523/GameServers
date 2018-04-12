
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
	m_pProps = getProp();
	m_pRewards = getReward();
	m_pShopItems = getShop();
	m_tasks = getTask();
	m_pSignZhuans = getSignZhuan();
	m_pSignAwards = getSignAward();
	m_pExAwards = getExAward();
	m_pActives = getActive(1);
	m_pFrees = getFree();
	for (int i = 0; i < 2;i++){
		vector<Rank> vec = getRank(i + 1, 1);
		m_pRanks.insert(make_pair(i+1,vec));
	}
	printf("111\n");
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
	if (m_pShopItems.empty()){
		ShopItem si;
		std::vector<Message *> vv = m_redis->getList("shop", si.GetTypeName());
		std::vector<ShopItem > vecs;
		for (int i = 0; i < vv.size(); i++){
			ShopItem rkk;
			rkk.CopyFrom(*vv.at(i));

			int rid = rkk.prop().rid();
			Reward *rd = rkk.mutable_prop();
			Reward rd1 = getReward(rid);
			rd->CopyFrom(rd1);

			int conid = rkk.consume().rid();
			if (conid>0){
				Reward *r = rkk.mutable_consume();
				Reward r1 = getReward(conid);
				r->CopyFrom(r1);
			}

			int giveid = rkk.give().rid();
			if (giveid > 0){
				Reward *r = rkk.mutable_give();
				Reward r1 = getReward(giveid);
				r->CopyFrom(r1);
			}

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pShopItems;
	}
}

ShopItem RedisGet::getShop(int shopid){
	auto itr = m_pShopItems.begin();
	for (itr; itr != m_pShopItems.end(); itr++){
		ShopItem si = *itr;
		if (si.id() == shopid){
			return si;
		}
	}
	ShopItem si;
	return si;
}

map<string, Mail> RedisGet::getMail(string uid){
	Mail si;
	std::vector<Message *> vv = m_redis->getList("mail"+uid, si.GetTypeName());
	map<string, Mail> vecs;
	char buff[50];
	for (int i = 0; i < vv.size(); i++){
		Mail rkk;
		rkk.CopyFrom(*vv.at(i));
		vector<int >ids;
		for (int j = 0; j < rkk.rewardlist_size(); j++){
			int id = rkk.rewardlist(j).rid();
			ids.push_back(id);
		}
		rkk.clear_rewardlist();
		for (int j = 0; j < ids.size(); j++){
			Reward *rd = rkk.add_rewardlist();
			int rid = ids.at(j);
			Reward rd1 = getReward(rid);
			rd->CopyFrom(rd1);
		}
		sprintf(buff,"%s%d",uid.c_str(),rkk.eid());
		vecs.insert(make_pair(buff,rkk));
	}
	redis::getIns()->releaseMessages(vv);
	return vecs;
}

Mail RedisGet::getMail(string uid, int eid){
	map<string, Mail> vec=getMail(uid);
	char buff[50];
	sprintf(buff, "%s%d", uid.c_str(), eid);
	if (vec.find(buff) != vec.end()){
		return vec.at(buff);
	}
	Mail mail;
	return	mail;
}

int RedisGet::getMailStatus(string uid, int mid){
	char buff[50];
	sprintf(buff,"mail%s%d",uid.c_str(),mid);
	int len = 0;
	return atoi(m_redis->get(buff,len));
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
	if (m_pActives.empty()){
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
	else{
		return m_pActives;
	}
}

vector<Task > RedisGet::getTask(){
	if (m_tasks.empty()){
		Task si;
		std::vector<Message *> vv = m_redis->getList("task", si.GetTypeName());
		std::vector<Task > vecs;
		for (int i = 0; i < vv.size(); i++){
			Task rkk;
			rkk.CopyFrom(*vv.at(i));

			vector<int >ids;
			for (int j = 0; j < rkk.rewardlist_size(); j++){
				int id = rkk.rewardlist(j).rid();
				ids.push_back(id);
			}
			rkk.clear_rewardlist();
			for (int j = 0; j < ids.size(); j++){
				Reward *rd = rkk.add_rewardlist();
				int rid = ids.at(j);
				Reward rd1 = getReward(rid);
				rd->CopyFrom(rd1);
			}

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_tasks;
	}
}

Status *RedisGet::getTaskStatus(string uid, int taskid){
	char buff[30];
	sprintf(buff,"task%s%d",uid.c_str(),taskid);
	Status si1;
	Status *si = (Status *)m_redis->getHash(buff, si1.GetTypeName());
	return si;
}

vector<ExAward> RedisGet::getExAward(){
	if (m_pExAwards.empty()){
		ExAward si;
		std::vector<Message *> vv = m_redis->getList("exchangereward", si.GetTypeName());
		std::vector<ExAward > vecs;
		for (int i = 0; i < vv.size(); i++){
			ExAward rkk;
			rkk.CopyFrom(*vv.at(i));

			int rid = rkk.award().rid();
			Reward *rd = rkk.mutable_award();
			Reward rd1 = getReward(rid);
			rd->CopyFrom(rd1);

			rid = rkk.buy().rid();
			Reward *r = rkk.mutable_buy();
			Reward r1 = getReward(rid);
			r->CopyFrom(r1);

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pExAwards;
	}
}

map<int, Reward> RedisGet::getReward(){
	if (m_pRewards.empty()){
		Reward si;
		std::vector<Message *> vv = m_redis->getList("reward", si.GetTypeName());
		map<int, Reward> vecs;
		for (int i = 0; i < vv.size(); i++){
			Reward rkk;
			rkk.CopyFrom(*vv.at(i));
			int pid = rkk.prop().id();
			Prop *pp = rkk.mutable_prop();
			Prop p = getProp(pid);
			pp->CopyFrom(p);
			vecs.insert(make_pair(rkk.rid(),rkk));
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pRewards;
	}
}

Reward RedisGet::getReward(int rid){
	map<int, Reward> vec = getReward();
	if (vec.find(rid) != vec.end()){
		return vec.at(rid);
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
	if (m_pSignAwards.empty()){
		SignAward si;
		std::vector<Message *> vv = m_redis->getList("sign", si.GetTypeName());
		std::vector<SignAward > vecs;
		for (int i = 0; i < vv.size(); i++){
			SignAward rkk;
			rkk.CopyFrom(*vv.at(i));

			int rid = rkk.reward().rid();
			Reward *rd = rkk.mutable_reward();
			Reward rd1 = getReward(rid);
			rd->CopyFrom(rd1);

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pSignAwards;
	}
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
	if (m_pSignZhuans.empty()){
		SignZhuan si;
		std::vector<Message *> vv = m_redis->getList("signzhuan", si.GetTypeName());
		std::vector<SignZhuan > vecs;
		for (int i = 0; i < vv.size(); i++){
			SignZhuan rkk;
			rkk.CopyFrom(*vv.at(i));
			int rid = rkk.reward().rid();
			Reward *rd = rkk.mutable_reward();
			Reward rd1 = getReward(rid);
			rd->CopyFrom(rd1);

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pSignZhuans;
	}
}

map<int, Prop > RedisGet::getProp(){
	if (m_pProps.empty()){
		Prop si;
		std::vector<Message *> vv = m_redis->getList("prop", si.GetTypeName());
		map<int, Prop > vecs;
		for (int i = 0; i < vv.size(); i++){
			Prop rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.insert(make_pair(rkk.id(),rkk));
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pProps;
	}
}

Prop RedisGet::getProp(int id){
	map<int, Prop > vec = getProp();
	if (vec.find(id) != vec.end()){
		return vec.at(id);
	}
	Prop p;
	return p;
}

vector<Task > RedisGet::getFree(){
	if (m_pFrees.empty()){
		Task si;
		std::vector<Message *> vv = m_redis->getList("free", si.GetTypeName());
		std::vector<Task > vecs;
		for (int i = 0; i < vv.size(); i++){
			Task rkk;
			rkk.CopyFrom(*vv.at(i));

			vector<int >ids;
			for (int j = 0; j < rkk.rewardlist_size(); j++){
				int id = rkk.rewardlist(j).rid();
				ids.push_back(id);
			}
			rkk.clear_rewardlist();
			for (int j = 0; j < ids.size(); j++){
				Reward *rd = rkk.add_rewardlist();
				int rid = ids.at(j);
				Reward rd1 = getReward(rid);
				rd->CopyFrom(rd1);
			}

			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
		return vecs;
	}
	else{
		return m_pFrees;
	}
}