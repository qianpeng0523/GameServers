
#include "RedisGet.h"
#include "RedisPut.h"
#include "CSVDataInfo.h"
#include "LoginInfo.h"
RedisGet *RedisGet::m_ins=NULL;
RedisGet::RedisGet(){
	m_pFirstBuyItem = NULL;
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
	m_pExchangeCodes = getCSVExchangeCode();
	m_pEXCodes = getExcode();
	getFirstBuy();
	getUserBases();
	getPass();
	getFriend();
	getOpenids();
	getSConfig();
	LoginInfo::getIns()->onLineUsers();
	getFriendNotices();
	for (int i = 0; i < 2;i++){
		vector<Rank> vec = getRank(i + 1, 1);
		m_pRanks.insert(make_pair(i+1,vec));
	}
	CLog::log("111\n");
}

string RedisGet::getPass(string uid){
	if (m_pPass.find(uid) != m_pPass.end()){
		return m_pPass.at(uid);
	}
	return "";
}

map<string, string> RedisGet::getPass(){
	if (m_pPass.empty()){
		string key = "userpass";
		vector<string>vec = m_redis->getListStr(key);
		for (int i = 0; i < vec.size(); i++){
			string content = vec.at(i);
			vector<string> ts = CSVDataInfo::getIns()->getStrFromstr(content,",");
			m_pPass.insert(make_pair(ts.at(0), ts.at(1)));
		}
		return m_pPass;
	}
	else{
		return m_pPass;
	}
}

void RedisGet::setPass(string uid, string pass){
	if (m_pPass.find(uid) == m_pPass.end()){
		m_pPass.insert(make_pair(uid, pass));
	}
}

map<string, UserBase *> RedisGet::getUserBases(){
	string key = "userbase";
	UserBase ub;
	auto vec = m_redis->getList(key,ub.GetTypeName());
	auto itr = vec.begin();
	int index = 0;
	for (itr; itr != vec.end();itr++){
		UserBase *ub1=(UserBase *)*itr;
		string uid = ub1->userid();
		if (m_pUserBases.find(uid)!=m_pUserBases.end()){
			m_pUserBases.at(uid) = ub1;
		}
		else{
			m_pUserBases.insert(make_pair(uid, ub1));
		}
		if (m_pUserIndexs.find(uid) != m_pUserIndexs.end()){
			m_pUserIndexs.at(uid) = index;
		}
		else{
			m_pUserIndexs.insert(make_pair(uid, index));
		}
		getUserLoginTime(uid);
		getPass(uid);
	}
	return m_pUserBases;
}

void RedisGet::getFriend(){
	auto vec = getUserBases();
	auto itr = vec.begin();
	for (itr; itr != vec.end();itr++){
		string uid = itr->first;
		getFriend(uid);
	}
}

UserBase *RedisGet::getUserBase(int index){
	auto itr = m_pUserIndexs.begin();
	for (itr; itr != m_pUserIndexs.end(); itr++){
		int pindex = itr->second;
		if (pindex == index){
			return getUserBase(itr->first);
		}
	}
	return NULL;
}

UserBase *RedisGet::getUserBase(string uid){
	if (m_pUserBases.find(uid)!=m_pUserBases.end()){
		return m_pUserBases.at(uid);
	}
	return NULL;
}

void RedisGet::setUserBase(UserBase *ub){
	string uid = ub->userid();
	if (m_pUserBases.find(uid) != m_pUserBases.end()){
		m_pUserBases.at(uid) = ub;
	}
	else{
		int index = m_pUserBases.size();
		if (m_pUserIndexs.find(uid) != m_pUserIndexs.end()){
			m_pUserIndexs.at(uid) = index;
		}
		else{
			m_pUserIndexs.insert(make_pair(uid, index));
		}
		m_pUserBases.insert(make_pair(uid, ub));
	}
}

int RedisGet::getUserBaseIndex(string uid){
	if (m_pUserIndexs.find(uid) != m_pUserIndexs.end()){
		return m_pUserIndexs.at(uid);
	}
	return -1;
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

map<string,UserBase *> RedisGet::getFriend(string uid){
	if (m_pfriends.find(uid)!=m_pfriends.end()){
		return m_pfriends.at(uid);
	}
	vector<int >lens;
	vector<char *> vv = m_redis->getList(uid, lens);
	map<string,UserBase *> vec;
	for (int i = 0; i < vv.size(); i++){
		string puid = vv.at(i);
		UserBase *ub = getUserBase(puid);
		if (ub){
			vec.insert(make_pair(puid, ub));
		}
	}
	m_pfriends.insert(make_pair(uid, vec));
	for (int i = 0; i < vv.size(); i++){
		delete vv.at(i);
	}
	vv.clear();
	return vec;
}

UserBase *RedisGet::getFriend(string uid,string fuid){
	auto vec = getFriend(uid);
	if (vec.find(fuid) != vec.end()){
		return vec.at(fuid);
	}
	return NULL;
}

void RedisGet::setFriend(string uid, string fuid, bool isadd){
	if (m_pfriends.find(uid) != m_pfriends.end()){
		auto vec = m_pfriends.at(uid);
		if (!isadd){
			if (vec.find(fuid) != vec.end()){
				vec.erase(vec.find(fuid));
			}
		}
		else{
			if (vec.find(fuid) == vec.end()){
				UserBase *ub = getUserBase(fuid);
				vec.insert(make_pair(fuid,ub));
			}
		}
	}
	else{
		if (isadd){
			map<string,UserBase *>vec;
			UserBase *ub = getUserBase(fuid);
			if (ub){
				vec.insert(make_pair(fuid, ub));
				m_pfriends.insert(make_pair(uid, vec));
			}
		}
	}
}

void RedisGet::setUserLoginTime(string uid, time_t t){
	if (m_pUserLoginTime.find(uid) != m_pUserLoginTime.end()){
		m_pUserLoginTime.at(uid) = t;
	}
	else{
		m_pUserLoginTime.insert(make_pair(uid, t));
	}
}

time_t RedisGet::getUserLoginTime(string uid){
	if (m_pUserLoginTime.find(uid) != m_pUserLoginTime.end()){
		return m_pUserLoginTime.at(uid);
	}
	else{
		string tt = "userlogintime";
		int index = getUserBaseIndex(uid);
		vector<int> lens;
		vector<char *>dd = m_redis->getList(tt,lens,index,index);
		if (!dd.empty()){
			time_t time = atol(dd.at(0));
			m_pUserLoginTime.insert(make_pair(uid, time));
			return time;
		}
	}
	return 0;
}

map<string, string> RedisGet::getOpenids(){
	map<string, string> maps;
	string key = "openid";
	vector<string> vec = m_redis->getListStr(key);
	for (int i = 0; i < vec.size(); i++){
		string con = vec.at(i);
		vector<string>vv = CSVDataInfo::getIns()->getStrFromstr(con,",");
		string t1 = vv.at(0);
		string t2 = vv.at(1);
		maps.insert(make_pair(t1, t2));
	}
	m_pOpenids = maps;
	return maps;
}

string RedisGet::getOpenidPass(string openid){
	if (m_pOpenids.find(openid) != m_pOpenids.end()){
		return m_pOpenids.at(openid);
	}
	return "";
}

map<int, FriendNotice *> RedisGet::getFriendNotice(string uid){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		return m_pFriendNotices.at(uid);
	}
	FriendNotice si;
	std::vector<Message *> vv = m_redis->getList("friendnotice" + uid, si.GetTypeName());
	map<int, FriendNotice *> vecs;
	for (int i = 0; i < vv.size(); i++){
		FriendNotice *rkk = (FriendNotice *)vv.at(i);
		vecs.insert(make_pair(rkk->nid(),rkk));
	}
	return vecs;
}

map<int, FriendNotice *> RedisGet::getFriendNoticeB(string uid){
	FriendNotice si;
	std::vector<Message *> vv = m_redis->getList("friendnotice"+uid, si.GetTypeName());
	map<int,FriendNotice *> vecs;
	for (int i = 0; i < vv.size(); i++){
		FriendNotice *rkk = (FriendNotice *)vv.at(i);
		vecs.insert(make_pair(rkk->nid(),rkk));
	}
	return vecs;
}

FriendNotice *RedisGet::getFriendNotice(string uid, string fuid){
	auto vec = getFriendNotice(uid);
	auto itr = vec.begin();
	for (itr; itr != vec.end(); itr++){
		FriendNotice *p = itr->second;
		if (p->uid().compare(fuid) == 0){
			return p;
		}
	}
	return NULL;
}

FriendNotice *RedisGet::getFriendNotice(string uid, int nid){
	auto vec = getFriendNotice(uid);
	if (vec.find(nid) != vec.end()){
		return vec.at(nid);
	}
	return NULL;
}

map<string, map<int, FriendNotice *>> RedisGet::getFriendNotices(){
	if (!m_pFriendNotices.empty()){
		return m_pFriendNotices;
	}
	auto vecs = getUserBases();
	auto itr = vecs.begin();
	for (itr; itr != vecs.end();itr++){
		string uid = itr->first;
		map<int, FriendNotice *> vec = getFriendNoticeB(uid);
		m_pFriendNotices.insert(make_pair(uid, vec));
	}
	return m_pFriendNotices;
}

void RedisGet::setFriendNotice(string uid, FriendNotice *p){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		auto vec = m_pFriendNotices.at(uid);
		if (vec.find(p->nid()) == vec.end()){
			vec.insert(make_pair(p->nid(),p));
		}
		else{
			vec.at(p->nid()) = p;
		}
		m_pFriendNotices.at(uid) = vec;
	}
	else{
		map<int,FriendNotice *> vec;
		vec.insert(make_pair(p->nid(),p));
		m_pFriendNotices.insert(make_pair(uid, vec));
	}
}

void RedisGet::eraseFriendNotice(string uid, FriendNotice *p){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		m_pFriendNotices.erase(m_pFriendNotices.find(uid));
		delete p;
		p = NULL;
	}
}

int RedisGet::getFriendNoticeID(){
	string key = "friendnoticeid";
	int len = 0;
	char *dd = m_redis->get(key,len);
	int id;
	if (dd){
		id = atoi(dd)+1;
		
	}
	else{
		dd = "1";
		id = 1;
		
	}
	RedisPut::getIns()->setFriendNoticeID(id);
	return id;
}

int RedisGet::getFriendNoticeIndex(string uid, int nid){
	auto vec = getFriendNotice(uid);
	auto itr = vec.begin();
	int index = 0;
	for (itr; itr != vec.end(); itr++){
		int pnid = itr->first;
		if (pnid == nid){
			return index;
		}
		index++;
	}
	return -1;
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
	if (m_pExRecords.find(uid) != m_pExRecords.end()){
		return m_pExRecords.at(uid);
	}

	ExRecord si;
	std::vector<Message *> vv = m_redis->getList("exchangerecord"+uid, si.GetTypeName());
	std::vector<ExRecord > vecs;
	for (int i = 0; i < vv.size(); i++){
		ExRecord rkk;
		rkk.CopyFrom(*vv.at(i));
		vecs.push_back(rkk);
	}
	redis::getIns()->releaseMessages(vv);
	m_pExRecords.insert(make_pair(uid,vecs));
	return vecs;
}

void RedisGet::PushExRecord(string uid, ExRecord p){
	if (m_pExRecords.find(uid) != m_pExRecords.end()){
		auto vec = m_pExRecords.at(uid);
		vec.push_back(p);
		m_pExRecords.at(uid) = vec;
	}
	else{
		std::vector<ExRecord > vecs;
		vecs.push_back(p);
		m_pExRecords.insert(make_pair(uid, vecs));
	}
}

int RedisGet::setExRecordStatus(string uid, int rid){
	char buff[50];
	sprintf(buff, "exchangestatus%s%d", uid.c_str(), rid);
	int len = 0;
	char *dd = m_redis->get(buff, len);
	if (dd){
		return atoi(dd);
	}
	else{
		return 0;
	}
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

SignStatus *RedisGet::getSignStatus(string uid){
	if (m_pSignStatuss.find(uid) != m_pSignStatuss.end()){
		return m_pSignStatuss.at(uid);
	}
	
	return NULL;
}

void RedisGet::setSignStatus( SignStatus *ss){
	if (m_pSignStatuss.find(ss->_uid) != m_pSignStatuss.end()){
		m_pSignStatuss.at(ss->_uid) = ss;
	}
	else{
		m_pSignStatuss.insert(make_pair(ss->_uid, ss));
	}
}

map<string, SignStatus *> RedisGet::getSignStatuss(){
	if (!m_pSignStatuss.empty()){
		return m_pSignStatuss;
	}
	string key = "signstatus";
	vector<string> vecs=m_redis->getListStr(key);
	for (int i = 0; i < vecs.size(); i++){
		vector<string> tt = CSVDataInfo::getIns()->getStrFromstr(vecs.at(i),",");
		SignStatus *ss = new SignStatus();
		ss->_uid = tt.at(0);
		ss->_signcount = atoi(tt.at(1).c_str());
		ss->_issign = atoi(tt.at(2).c_str());
		ss->_left = atoi(tt.at(3).c_str());
		ss->_time = tt.at(4);
		m_pSignStatuss.insert(make_pair(ss->_uid,ss));
	}
	return m_pSignStatuss;
}

map<string, SConfig *> RedisGet::getSConfig(){
	string key = "config";
	SConfig sc;
	vector<Message *> vec = m_redis->getList(key,sc.GetTypeName());
	for (int i = 0; i < vec.size(); i++){
		SConfig *sc1 = (SConfig *)vec.at(i);
		UserBase *ub = getUserBase(i);
		string uid = ub->userid();
		m_pSConfigs.insert(make_pair(uid,sc1));
	}
	return m_pSConfigs;
}

SConfig* RedisGet::getSConfig(string uid){
	SConfig *sc1=NULL;
	if (m_pSConfigs.find(uid) != m_pSConfigs.end()){
		sc1 = m_pSConfigs.at(uid);
	}
	return sc1;
}

void RedisGet::setSConfig(string uid,SConfig *sc){
	if (m_pSConfigs.find(uid) != m_pSConfigs.end()){
		m_pSConfigs.at(uid)=sc;
	}
	else{
		m_pSConfigs.insert(make_pair(uid, sc));
	}
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

int RedisGet::MailID(){
	int len = 0;
	char *dd= m_redis->get("mailid_index", len);
	if (dd){
		return atoi(dd) + 1;
	}
	
	return 1;
}

FirstBuyItem *RedisGet::getFirstBuy(){
	if (m_pFirstBuyItem){
		return m_pFirstBuyItem;
	}
	std::map<string, string> vv = m_redis->getHash("firstbuy");
	if (vv.empty()){
		return m_pFirstBuyItem;
	}
	m_pFirstBuyItem = new FirstBuyItem();
	auto itr = vv.begin();
	for (itr; itr != vv.end(); itr++){
		string name = itr->first;
		string value = itr->second;
		if (name.compare("id") == 0){
			m_pFirstBuyItem->_sid = atoi(value.c_str());
		}
		else if (name.compare("rewardid") == 0){
			m_pFirstBuyItem->_rid = CSVDataInfo::getIns()->getIntFromstr(value,"|");
		}
		else if (name.compare("conid") == 0){
			m_pFirstBuyItem->_conid = atoi(value.c_str());
		}
		else if (name.compare("giveid") == 0){
			m_pFirstBuyItem->_giveid = CSVDataInfo::getIns()->getIntFromstr(value, "|");
		}
	}
	return m_pFirstBuyItem;
}

ExAward RedisGet::getExAward(int id){
	auto vec = getExAward();
	ExAward ex;
	for (int i = 0; i < vec.size(); i++){
		ExAward ex1 = vec.at(i);
		if (ex1.eid() == id){
			return ex1;
		}
	}
	return ex;
}

string RedisGet::getExchangeCode(){
	int len = 0;
	char *dd = m_redis->get("exchangecode", len);
	if (!dd){
		dd = "10000000";
		RedisPut::getIns()->setExchangeCode(dd);
	}
	return dd;
}

int RedisGet::getExchangeRecordId(string uid){
	int len = 0;
	char *dd = m_redis->get("exchangerecordid" + uid,len);
	if (!dd){
		dd = "1";
		RedisPut::getIns()->setExchangeRecordId(uid, atoi(dd));
	}
	return atoi(dd);
}

map<string, PExchangeCode*> RedisGet::getCSVExchangeCode(){
	if (!m_pExchangeCodes.empty()){
		return m_pExchangeCodes;
	}
	map<string, PExchangeCode*> codes;
	string key = "CSVExchangeCode";
	vector<bool> veccodes=getExcode();
	vector<string> mpas = m_redis->getListStr(key);
	for (int i = 0; i < mpas.size(); i++){
		vector<string >vec;
		std::string field = mpas.at(i);
		int index = field.find(",");
		while (index != -1) {
			vec.push_back(field.substr(0, index));
			field = field.substr(index+1, field.length());
			index = field.find(",");
		}
		vec.push_back(field);
		PExchangeCode* cc=new PExchangeCode();
		if (vec.size() == 3){
			cc->_id = atoi(vec.at(0).c_str());
			cc->_rewardid = CSVDataInfo::getIns()->getIntFromstr(vec.at(1), "|");
			cc->_code = vec.at(2);
			codes.insert(make_pair(cc->_code,cc));
		}
		else{
			printf("vvvvvvvvvvvvvvvvvvv\n");
		}
		if (veccodes.empty()){
			printf("%d.",i+1);
			RedisPut::getIns()->setEXCode(cc->_code, false);
		}
	}
	return codes;
}

PExchangeCode* RedisGet::getPExchangeCode(string code){
	if (m_pExchangeCodes.find(code) != m_pExchangeCodes.end()){
		return m_pExchangeCodes.at(code);
	}
	return NULL;
}

vector<bool> RedisGet::getExcode(){
	if (!m_pEXCodes.empty()){
		return m_pEXCodes;
	}
	vector<bool> vec1;
	string key = "EXCodeStatus";
	vector<string> vec = m_redis->getListStr(key);
	for (int i = 0; i < vec.size(); i++){
		vec1.push_back(atoi(vec.at(i).c_str()));
	}
	return vec1;
}

bool RedisGet::getExcode(string code){
	PExchangeCode *p = getPExchangeCode(code);
	if (p){
		int index = p->_id-1;
		if (index < m_pEXCodes.size()){
			return m_pEXCodes.at(index);
		}
		return true;
	}
	return true;
}

void RedisGet::setEXCodeStatus(int id,bool ist){
	int index = id - 1;
	if (index < m_pEXCodes.size()){
		m_pEXCodes.at(index) = ist;
	}
}