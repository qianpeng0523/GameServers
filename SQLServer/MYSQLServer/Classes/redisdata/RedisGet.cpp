
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

map<string, REDISDBName *> RedisGet::getREDISDBNames(){
	if (!m_RedisDBNames.empty()){
		return m_RedisDBNames;
	}
	vector<string> vecs = m_redis->getListStr("redisname");
	for (int i = 0; i < vecs.size(); i++){
		string con = vecs.at(i);
		vector<string> vv = CSVDataInfo::getIns()->getStrFromstr(con,",");
		REDISDBName *p = new REDISDBName();
		p->_name = vv.at(0);
		p->_dbindex = atoi(vv.at(1).c_str());
		m_RedisDBNames.insert(make_pair(p->_name,p));
	}
	return m_RedisDBNames;
}

REDISDBName *RedisGet::getREDISDBName(string name){
	if (m_RedisDBNames.find(name) != m_RedisDBNames.end()){
		return m_RedisDBNames.at(name);
	}
	return NULL;
}

int RedisGet::getRedisDBIndex(string name){
	REDISDBName *p = getREDISDBName(name);
	if (p){
		return p->_dbindex;
	}
	return -1;
}

bool RedisGet::SelectDB(REDISTYPE type){
	string dbname = g_redisdbnames[type];
	int index = getRedisDBIndex(dbname);
	return m_redis->SelectDB(index);
}

void RedisGet::init(){
	RedisPut *pRedisPut = RedisPut::getIns();
	CSVDataInfo *csvinfo = CSVDataInfo::getIns();

	m_RedisDBNames = getREDISDBNames();
	if (m_RedisDBNames.empty()){
		m_RedisDBNames = csvinfo->getDBNames();
		auto itr = m_RedisDBNames.begin();
		for (itr; itr != m_RedisDBNames.end(); itr++){
			RedisPut::getIns()->PushREDISDBName(itr->second);
		}
	}

	getCSVExchangeCodes();
	
	getGates();
	if (m_pGates.empty()){
		CSVDataInfo::getIns()->openCSVFile("res/gateserver.csv", CSV_GATESERVER);
		CSVDataInfo::getIns()->openCSVFile("res/logicmanager.csv", CSV_LOGOCMANAGER);

		std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
		std::map<int, Object *>::iterator itr = maps.begin();
		
		for (itr; itr != maps.end(); itr++){
			GateData *data = (GateData *)itr->second;
			pRedisPut->PushGate(data, SERVER_TYPE_GATE);
		}

		std::map<int, Object *> maps1 = CSVDataInfo::getIns()->getDatas(CSV_LOGOCMANAGER);
		std::map<int, Object *>::iterator itr1 = maps1.begin();
		for (itr1; itr1 != maps1.end(); itr1++){
			GateData *data = (GateData *)itr1->second;
			pRedisPut->PushGate(data, SERVER_TYPE_LOGICMANAGER);
		}
	}
	else{
		CLog::log("******************** gate list ******************\n");
		auto itr = m_pGates.begin();
		int i = 0;
		for (itr; itr != m_pGates.end(); itr++){
			string key = itr->first;
			auto maps = itr->second;
			auto itr1 = maps.begin();
			for (itr1; itr1 != maps.end();itr1++){
				GateData *data = (GateData *)itr1->second;
				CLog::log("[%d].【%d】.%s\n", i + 1,data->_id, data->getDebugString());
				i++;
			}
		}
		CLog::log("******************** end ******************\n");
	}

	m_props = getProp();
	if (m_props.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_PROP);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVProp *re = (CSVProp *)itr->second;
			Prop si;
			si.set_id(re->_pid);
			si.set_name(re->_name);

			pRedisPut->PushProp(si);
			m_props.insert(make_pair(re->_pid, si));
		}
	}
	else{
		CLog::log("******************** prop list ******************\n");
		auto itr = m_props.begin();
		int i = 0;
		for (itr; itr != m_props.end(); itr++){
			Prop rd = itr->second;
			CLog::log("[%d].%s\n",i+1, rd.DebugString().c_str());
			i++;
		}
		CLog::log("******************** end ******************\n");
	}

	m_rewards = getReward();
	if (m_rewards.empty()){
		std::map<int, Object *> vec = csvinfo->getDatas(CSV_REWARD);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVReward *re = (CSVReward *)itr->second;
			Reward rd;
			rd.set_rid(re->_rid);
			rd.set_number(re->_pnumber);
			Prop *p = rd.mutable_prop();
			Prop p1 = getProp(re->_pid);
			p->CopyFrom(p1);
			pRedisPut->PushReward(rd);
			m_rewards.insert(make_pair(re->_rid, rd));
		}
	}
	else{
		CLog::log("******************** reward list******************\n");
		auto itr = m_rewards.begin();
		int i = 0;
		for (itr; itr != m_rewards.end(); itr++){
			Reward rd = itr->second;
			CLog::log("[%d].%s\n",i+1, rd.DebugString().c_str());
			i++;
		}
		CLog::log("******************** end ******************\n");
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
			Reward rd1 = getReward(re->_rid);
			rd->CopyFrom(rd1);
			if (re->_conid > 0){
				Reward *con = si.mutable_consume();
				Reward con1 = getReward(re->_conid);
				con->CopyFrom(con1);
			}
			if (re->_giveid > 0){
				Reward *give = si.mutable_give();
				Reward give1 = getReward(re->_giveid);
				give->CopyFrom(give1);
			}
			pRedisPut->PushShop(si);
		}
	}
	else{
		CLog::log("******************** reward list ******************\n");
		for (int i = 0; i < vecr1.size(); i++){
			ShopItem rd = vecr1.at(i);
			CLog::log("[%d].%s\n",i+1, rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
				Reward rd1 = getReward(re->_rewardid.at(i));
				rd->CopyFrom(rd1);
			}
			pRedisPut->PushTask(si);
		}
	}
	else{
		CLog::log("******************** task list ******************\n");
		for (int i = 0; i < vecr2.size(); i++){
			Task rd = vecr2.at(i);
			CLog::log("%s\n", rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
			Reward rd1 = getReward(re->_rid);
			rd->CopyFrom(rd1);
			pRedisPut->PushSignZhuan(si);
		}
	}
	else{
		CLog::log("******************** signzhuan list ******************\n");
		for (int i = 0; i < vecr3.size(); i++){
			SignZhuan rd = vecr3.at(i);
			CLog::log("[%d].%s\n",i+1, rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
			Reward rd1 = getReward(re->_rid);
			rd->CopyFrom(rd1);
			pRedisPut->PushSignAward(si);
		}
	}
	else{
		CLog::log("******************** signaward list ******************\n");
		for (int i = 0; i < vecr4.size(); i++){
			SignAward rd = vecr4.at(i);
			CLog::log("[%d].%s\n", i+1,rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
			Reward rd1 = getReward(re->_rid);
			rd->CopyFrom(rd1);
			
			Reward *buy = si.mutable_buy();
			Reward buy1 = getReward(re->_brid);
			buy->CopyFrom(buy1);
			pRedisPut->PushExAward(si);
		}
	}
	else{
		CLog::log("******************** exaward list ******************\n");
		for (int i = 0; i < vecr5.size(); i++){
			ExAward rd = vecr5.at(i);
			CLog::log("[%d].%s\n", i+1 ,rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
		CLog::log("******************** active list ******************\n");
		for (int i = 0; i < vecr6.size(); i++){
			Active rd = vecr6.at(i);
			CLog::log("[%d].%s\n",i+1, rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
				int rid = re->_rewardid.at(i);
				Reward rd1 = getReward(rid);
				rd->CopyFrom(rd1);
			}
			pRedisPut->PushFree(si);
		}
	}
	else{
		CLog::log("******************** task list ******************\n");
		for (int i = 0; i < vecr8.size(); i++){
			Task rd = vecr8.at(i);
			CLog::log("[%d].%s\n",i+1 ,rd.DebugString().c_str());
		}
		CLog::log("******************** end ******************\n");
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
	else{
		CLog::log("******************** CSVFirstBuyItem list ******************\n");
		CLog::log("[%d].0x%P",1,p);
		CLog::log("******************** end ******************\n");
	}
}

map<string, map<int, GateData *>> RedisGet::getGates(){
	if (!m_pGates.empty()){
		return m_pGates;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_GATE);
	if (ist){
		for (int i = SERVER_TYPE_GATE; i <= SERVER_TYPE_LOGICMANAGER; i++){
			string key = g_redisdbnames[REIDS_GATE];
			string tp;
			switch ((SERVERTYPE)i)
			{
			case SERVER_TYPE_GATE:
				tp = "_gate";
				break;
			case SERVER_TYPE_LOGICMANAGER:
				tp = "_logicmanager";
				break;
			default:
				break;
			}
			key += tp;
			map<int, GateData *> maps;
			if (m_pGates.find(key) != m_pGates.end()){
				maps = m_pGates.at(key);
			}
			vector<int> lens;
			vector<char *>vec = m_redis->getList(key, lens);
			for (int j = 0; j < vec.size(); j++){
				GateData *data = (GateData *)vec.at(j);
				data->DebugPrintf();
				int type = data->_type;
				maps.insert(make_pair(type, data));
			}
			if (!maps.empty()){
				if (m_pGates.find(key) != m_pGates.end()){
					m_pGates.at(key) = maps;
				}
				else{
					m_pGates.insert(make_pair(key, maps));
				}
			}
		}
	}
	return m_pGates;
}

GateData *RedisGet::getGateData(string name, int type){
	if (m_pGates.find(name) != m_pGates.end()){
		auto vec = m_pGates.at(name);
		if (vec.find(type) != vec.end()){
			return vec.at(type);
		}
	}
	return NULL;
}

FirstBuyItem *RedisGet::getFirstBuy(){
	if (m_pFirstBuyItem){
		return m_pFirstBuyItem;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		std::map<string, string> vv = m_redis->getHash(g_redisdbnames[REIDS_SHOP] + "first");
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
	}
	return m_pFirstBuyItem;
}

std::vector<ShopItem > RedisGet::getShop(){
	std::vector<ShopItem > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		ShopItem si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SHOP], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			ShopItem rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}


vector<Active > RedisGet::getActive(int type){
	std::vector<Active > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_ACTIVE);
	if (ist){
		Active si;
		char buff[100];
		sprintf(buff, "%s%d", g_redisdbnames[REIDS_ACTIVE].c_str(), type);
		std::vector<Message *> vv = m_redis->getList(buff, si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Active rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

vector<Task > RedisGet::getTask(){
	std::vector<Task > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_TASK);
	if (ist){
		Task si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_TASK], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Task rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

vector<ExAward> RedisGet::getExAward(){
	std::vector<ExAward > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		ExAward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_EXCHANGE] + "award", si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			ExAward rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

map<int, Reward> RedisGet::getReward(){
	if (!m_rewards.empty()){
		return m_rewards;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_REWARD);
	if (ist){
		Reward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_REWARD], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Reward rkk;
			rkk.CopyFrom(*vv.at(i));
			m_rewards.insert(make_pair(rkk.rid(), rkk));
		}
		redis::getIns()->releaseMessages(vv);
	}
	return m_rewards;
}

Reward RedisGet::getReward(int rid){
	if (m_rewards.find(rid) != m_rewards.end()){
		return m_rewards.at(rid);
	}
	Reward rd;
	return rd;
}

vector<SignAward> RedisGet::getSignAward(){
	std::vector<SignAward > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		SignAward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SIGN], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			SignAward rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

vector<SignZhuan> RedisGet::getSignZhuan(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	std::vector<SignZhuan > vecs;
	if (ist){
		SignZhuan si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SIGN] + "zhuan", si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			SignZhuan rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

map<int, Prop > RedisGet::getProp(){
	if (!m_props.empty()){
		return m_props;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PROP);
	if (ist){
		Prop si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_PROP], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Prop rkk;
			rkk.CopyFrom(*vv.at(i));
			m_props.insert(make_pair(rkk.id(), rkk));
		}
		redis::getIns()->releaseMessages(vv);
	}
	return m_props;
}

Prop RedisGet::getProp(int pid){
	if (m_props.find(pid) != m_props.end()){
		return m_props.at(pid);
	}
	Prop p;
	return p;
}

vector<Task > RedisGet::getFree(){
	std::vector<Task > vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FREE);
	if (ist){
		Task si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_FREE], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Task rkk;
			rkk.CopyFrom(*vv.at(i));
			vecs.push_back(rkk);
		}
		redis::getIns()->releaseMessages(vv);
	}
	return vecs;
}

vector<CSVExchangeCode > RedisGet::getCSVExchangeCodes(){
	vector<CSVExchangeCode > codevec;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		string key = g_redisdbnames[REIDS_EXCHANGE] + "code";
		auto vec = m_redis->getListStr(key);
		for (int i = 0; i < vec.size(); i++){
			string content = vec.at(i);
			CSVExchangeCode p;
			auto vv = CSVDataInfo::getIns()->getStrFromstr(content,",");
			p._id = atoi(vv.at(0).c_str());
			p._rewardid = vv.at(1);
			p._code = vv.at(2);
			codevec.push_back(p);
		}
	}
	return codevec;
}