
#include "RedisGet.h"
#include "RedisPut.h"
#include "CSVDataInfo.h"

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
	RedisPut *pRedisPut = RedisPut::getIns();
	CSVDataInfo *csvinfo = CSVDataInfo::getIns();
	vector<Prop> vecr7 = getProp();
	if (vecr7.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_PROP);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVProp *re = (CSVProp *)itr->second;
			Prop si;
			si.set_id(re->_pid);
			si.set_name(re->_name);

			pRedisPut->PushProp(si);
		}
	}
	else{
		for (int i = 0; i < vecr7.size(); i++){
			Prop rd = vecr7.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Reward> vecr = getReward();
	if (vecr.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_REWARD);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVReward *re = (CSVReward *)itr->second;
			Reward rd;
			rd.set_rid(re->_rid);
			rd.set_number(re->_pnumber);
			Prop *p = rd.mutable_prop();
			p->set_id(re->_pid);
			p->set_name(p->name());
			pRedisPut->PushReward(rd);
		}
	}
	else{
		for (int i = 0; i < vecr.size(); i++){
			Reward rd = vecr.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<ShopItem> vecr1 = getShop();
	if (vecr1.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_SHOP);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVShopItem *re = (CSVShopItem *)itr->second;
			ShopItem si;
			si.set_id(re->_sid);
			si.set_hot(re->_hot);
			Reward *rd = si.mutable_prop();
			rd->set_rid(re->_rid);
			if (re->_conid > 0){
				Reward *rd1 = si.mutable_consume();
				rd1->set_rid(re->_conid);
			}
			if (re->_giveid > 0){
				Reward *rd1 = si.mutable_give();
				rd1->set_rid(re->_giveid);
			}
			pRedisPut->PushShop(si);
		}
	}
	else{
		for (int i = 0; i < vecr1.size(); i++){
			ShopItem rd = vecr1.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Task> vecr2 = getTask();
	if (vecr2.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_TASK);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVTaskItem *re = (CSVTaskItem *)itr->second;
			Task si;
			si.set_taskid(re->_tid);
			si.set_title(re->_title);
			si.set_content(re->_content);
			si.set_type(re->_type);

			for (int i = 0; i < re->_rewardid.size(); i++){
				Reward *rd = si.add_rewardlist();
				rd->set_rid(re->_rewardid.at(i));
			}
			pRedisPut->PushTask(si);
		}
	}
	else{
		for (int i = 0; i < vecr2.size(); i++){
			Task rd = vecr2.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<SignZhuan> vecr3 = getSignZhuan();
	if (vecr3.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_ZHUANPAN);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVSignZhuan *re = (CSVSignZhuan *)itr->second;
			SignZhuan si;
			si.set_index(re->_index);

			Reward *rd = si.mutable_reward();
			rd->set_rid(re->_rid);
			pRedisPut->PushSignZhuan(si);
		}
	}
	else{
		for (int i = 0; i < vecr3.size(); i++){
			SignZhuan rd = vecr3.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<SignAward> vecr4 = getSignAward();
	if (vecr4.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_SIGN);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVSignItem *re = (CSVSignItem *)itr->second;
			SignAward si;
			si.set_id(re->_id);
			si.set_day(re->_day);

			Reward *rd = si.mutable_reward();
			rd->set_rid(re->_rid);
			pRedisPut->PushSignAward(si);
		}
	}
	else{
		for (int i = 0; i < vecr4.size(); i++){
			SignAward rd = vecr4.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<ExAward> vecr5 = getExAward();
	if (vecr5.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_EXREWARD);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVExchangeAward *re = (CSVExchangeAward *)itr->second;
			ExAward si;
			si.set_eid(re->_eid);
			si.set_title(re->_title);

			Reward *rd = si.mutable_award();
			rd->set_rid(re->_rid);

			Reward *rd1 = si.mutable_buy();
			rd1->set_rid(re->_brid);
			pRedisPut->PushExAward(si);
		}
	}
	else{
		for (int i = 0; i < vecr5.size(); i++){
			ExAward rd = vecr5.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Active> vecr6 = getActive(1);
	if (vecr6.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_ACTIVE);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVActive *re = (CSVActive *)itr->second;
			Active si;
			si.set_id(re->_id);
			si.set_type(re->_type);
			si.set_url(re->_url);

			pRedisPut->PushActive(si);
		}
	}
	else{
		for (int i = 0; i < vecr6.size(); i++){
			Active rd = vecr6.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Task> vecr8 = getFree();
	if (vecr8.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_FREE);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVTaskItem *re = (CSVTaskItem *)itr->second;
			Task si;
			si.set_taskid(re->_tid);
			si.set_title(re->_title);
			si.set_content(re->_content);
			si.set_type(re->_type);

			for (int i = 0; i < re->_rewardid.size(); i++){
				Reward *rd = si.add_rewardlist();
				rd->set_rid(re->_rewardid.at(i));
			}
			pRedisPut->PushFree(si);
		}
	}
	else{
		for (int i = 0; i < vecr8.size(); i++){
			Task rd = vecr8.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
	}

	FirstBuyItem *p = getFirstBuy();
	if (!p){
		auto vec = csvinfo->getDatas(CSV_FIRSTBUY);
		auto itr = vec.begin();
		for (itr; itr != vec.end();itr++){
			CSVFirstBuyItem *pp = (CSVFirstBuyItem *)itr->second;
			pRedisPut->PushFirstBuy(pp);
		}
	}
}

FirstBuyItem *RedisGet::getFirstBuy(){
	if (m_pFirstBuyItem){
		return m_pFirstBuyItem;
	}
	std::map<string,string> vv = m_redis->getHash("firstbuy");
	if (vv.empty()){
		return m_pFirstBuyItem;
	}
	m_pFirstBuyItem = new FirstBuyItem();
	auto itr = vv.begin();
	for (itr; itr != vv.end();itr++){
		string name = itr->first;
		string value = itr->second;
		if (name.compare("id") == 0){
			m_pFirstBuyItem->_sid = atoi(value.c_str());
		}
		else if (name.compare("rewardid") == 0){
			m_pFirstBuyItem->_rid = CSVDataInfo::getIns()->getIntFromstr(value);
		}
		else if (name.compare("conid") == 0){
			m_pFirstBuyItem->_conid = atoi(value.c_str());
		}
		else if (name.compare("giveid") == 0){
			m_pFirstBuyItem->_giveid = CSVDataInfo::getIns()->getIntFromstr(value);
		}
	}
	return m_pFirstBuyItem;
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