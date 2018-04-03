#include "CSVDataInfo.h"
#include "RedisGet.h"
#include "RedisPut.h"

CSVDataInfo *CSVDataInfo::m_ins=NULL;

CSVDataInfo::CSVDataInfo(){
	
}

CSVDataInfo::~CSVDataInfo(){
	std::map<CSVSTRUCT, CSVDataHelper *>::iterator itr = m_CSVDataHelpers.begin();
	while (itr!=m_CSVDataHelpers.end())
	{
		m_CSVDataHelpers.erase(itr++);
	}

	std::map<CSVSTRUCT, std::map<int, Object *>>::iterator itr1 = m_Objects.begin();
	while (itr1 != m_Objects.end())
	{
		std::map<int, Object *> vecs = itr1->second;
		std::map<int, Object *>::iterator itr2 = vecs.begin();
		while (itr2 != vecs.end()){
			delete itr2->second;
			itr2++;
		}
		vecs.clear();
		itr1++;
	}
	m_Objects.clear();
}

bool CSVDataInfo::init()
{
	CSVDataInfo::getIns()->openCSVFile("./res/robot.csv", CSV_ROBOT);
	CSVDataInfo::getIns()->openCSVFile("./res/gateserver.csv", CSV_GATESERVER);
	CSVDataInfo::getIns()->openCSVFile("./res/logicmanager.csv", CSV_LOGOCMANAGER);

	CSVDataInfo::getIns()->openCSVFile("./res/reward.csv", CSV_REWARD);
	CSVDataInfo::getIns()->openCSVFile("./res/shop.csv", CSV_SHOP);
	CSVDataInfo::getIns()->openCSVFile("./res/task.csv", CSV_TASK);
	CSVDataInfo::getIns()->openCSVFile("./res/sign.csv", CSV_SIGN);
	CSVDataInfo::getIns()->openCSVFile("./res/zhuanpan.csv", CSV_ZHUANPAN);
	CSVDataInfo::getIns()->openCSVFile("./res/exreward.csv", CSV_EXREWARD);
	CSVDataInfo::getIns()->openCSVFile("./res/active.csv", CSV_ACTIVE);
	CSVDataInfo::getIns()->openCSVFile("./res/prop.csv", CSV_PROP);
	CSVDataInfo::getIns()->openCSVFile("./res/free.csv", CSV_FREE);

	RedisPut *pRedisPut = RedisPut::getIns();
	RedisGet *pRedisGet = RedisGet::getIns();

	vector<Prop> vecr7 = pRedisGet->getProp();
	if (vecr7.empty()){
		std::map<int, Object *> vec = getDatas(CSV_PROP);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Reward> vecr = pRedisGet->getReward();
	if (vecr.empty()){
		std::map<int, Object *> vec = getDatas(CSV_REWARD);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr!=vec.end(); itr++){
			CSVReward *re = (CSVReward *)itr->second;
			Reward rd;
			rd.set_rid(re->_rid);
			rd.set_number(re->_pnumber);
			pRedisPut->PushReward(rd);
		}
	}
	else{
		for (int i = 0; i < vecr.size();i++){
			Reward rd = vecr.at(i);
			printf("%s\n",rd.DebugString().c_str());
		}
	}

	vector<ShopItem> vecr1 = pRedisGet->getShop();
	if (vecr1.empty()){
		std::map<int, Object *> vec = getDatas(CSV_SHOP);
		std::map<int, Object *>::iterator itr = vec.begin();
		for (itr; itr != vec.end(); itr++){
			CSVShopItem *re = (CSVShopItem *)itr->second;
			ShopItem si;
			si.set_id(re->_sid);
			si.set_hot(re->_hot);
			Reward *rd=si.mutable_prop();
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Task> vecr2 = pRedisGet->getTask();
	if (vecr2.empty()){
		std::map<int, Object *> vec = getDatas(CSV_TASK);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<SignZhuan> vecr3 = pRedisGet->getSignZhuan();
	if (vecr3.empty()){
		std::map<int, Object *> vec = getDatas(CSV_ZHUANPAN);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<SignAward> vecr4 = pRedisGet->getSignAward();
	if (vecr4.empty()){
		std::map<int, Object *> vec = getDatas(CSV_SIGN);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<ExAward> vecr5 = pRedisGet->getExAward();
	if (vecr5.empty()){
		std::map<int, Object *> vec = getDatas(CSV_EXREWARD);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Active> vecr6 = pRedisGet->getActive(1);
	if (vecr6.empty()){
		std::map<int, Object *> vec = getDatas(CSV_ACTIVE);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

	vector<Task> vecr8 = pRedisGet->getFree();
	if (vecr8.empty()){
		std::map<int, Object *> vec = getDatas(CSV_FREE);
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
			printf("%s\n", rd.DebugString().c_str());
		}
	}

    return true;
}


CSVDataInfo* CSVDataInfo::getIns(){
	if (!m_ins){
		m_ins = new CSVDataInfo();
		m_ins->init();
	}
	return m_ins;
}

void CSVDataInfo::openCSVFile(string file, CSVSTRUCT filekey){
	CSVDataHelper *p = new CSVDataHelper();
	p->openAndResolveFile(file.c_str());
	if (m_CSVDataHelpers.find(filekey) != m_CSVDataHelpers.end()){
		m_CSVDataHelpers.at(filekey) = p;
	}
	else{
		m_CSVDataHelpers.insert(make_pair(filekey, p));
	}
	
	if (m_Objects.find(filekey) == m_Objects.end()){
		std::map<int, Object *> vec;
		m_Objects.insert(make_pair(filekey,vec));
	}

	int rowlen = p->getRowLength();
	int collen = p->getColLength();
	std::map<int, Object *> vec = m_Objects.at(filekey);
	for (int j = 1; j < rowlen; j++){
		//string colname = p->getColName(j);
		Object *obj = NULL;
		switch (filekey)
		{
		case CSV_ROBOT:
		case CSV_LOGOCSERVER:
			obj = new RobotData();
			((RobotData *)obj)->_id = atoi(p->getData(j, 0));
			((RobotData *)obj)->_name = p->getData(j, 1);
			vec.insert(make_pair(((RobotData *)obj)->_id, ((RobotData *)obj)));
			break;
		case CSV_GATESERVER:
		case CSV_LOGOCMANAGER:
			obj = new GateData();
			((GateData *)obj)->_id = atoi(p->getData(j, 0));
			strcpy(((GateData *)obj)->_name , p->getData(j, 1));
			strcpy(((GateData *)obj)->_ip, p->getData(j, 2));
			((GateData *)obj)->_port =atoi(p->getData(j, 3));
			((GateData *)obj)->_type = atoi(p->getData(j, 4));
			vec.insert(make_pair(((GateData *)obj)->_id, ((GateData *)obj)));
			break;
		case CSV_REWARD:
			obj = new CSVReward();
			((CSVReward *)obj)->_rid = atoi(p->getData(j, 0));
			((CSVReward *)obj)->_pid = atoi(p->getData(j, 1));
			((CSVReward *)obj)->_pnumber = atoi(p->getData(j, 2));
			vec.insert(make_pair(((CSVReward *)obj)->_rid, ((CSVReward *)obj)));
			break;
		case CSV_SHOP:
			obj = new CSVShopItem();
			((CSVShopItem *)obj)->_sid = atoi(p->getData(j, 0));
			((CSVShopItem *)obj)->_rid = atoi(p->getData(j, 1));
			((CSVShopItem *)obj)->_conid = atoi(p->getData(j, 2));
			((CSVShopItem *)obj)->_giveid = atoi(p->getData(j, 3));
			((CSVShopItem *)obj)->_hot = atoi(p->getData(j, 4));
			vec.insert(make_pair(((CSVShopItem *)obj)->_sid, ((CSVShopItem *)obj)));
			break;
		case CSV_TASK:
			obj = new CSVTaskItem();
			((CSVTaskItem *)obj)->_tid = atoi(p->getData(j, 0));
			((CSVTaskItem *)obj)->_title = p->getData(j, 1);
			((CSVTaskItem *)obj)->_content = p->getData(j, 2);
			((CSVTaskItem *)obj)->_count = atoi(p->getData(j, 3));
			((CSVTaskItem *)obj)->_rewardid = getIntFromstr(p->getData(j, 4));
			((CSVTaskItem *)obj)->_type = atoi(p->getData(j, 5));
			vec.insert(make_pair(((CSVTaskItem *)obj)->_tid, ((CSVTaskItem *)obj)));
			break;
		case CSV_SIGN:
			obj = new CSVSignItem();
			((CSVSignItem *)obj)->_id = atoi(p->getData(j, 0));
			((CSVSignItem *)obj)->_day = atoi(p->getData(j, 1));
			((CSVSignItem *)obj)->_rid = atoi(p->getData(j, 2));
			vec.insert(make_pair(((CSVSignItem *)obj)->_id, ((CSVSignItem *)obj)));
			break;
		case CSV_ZHUANPAN:
			obj = new CSVSignZhuan();
			((CSVSignZhuan *)obj)->_index = atoi(p->getData(j, 0));
			((CSVSignZhuan *)obj)->_rid = atoi(p->getData(j, 1));
			vec.insert(make_pair(((CSVSignZhuan *)obj)->_index, ((CSVSignZhuan *)obj)));
			break;
		case CSV_EXREWARD:
			obj = new CSVExchangeAward();
			((CSVExchangeAward *)obj)->_eid = atoi(p->getData(j, 0));
			((CSVExchangeAward *)obj)->_title = p->getData(j, 1);
			((CSVExchangeAward *)obj)->_rid = atoi(p->getData(j, 2));
			((CSVExchangeAward *)obj)->_brid = atoi(p->getData(j, 3));
			vec.insert(make_pair(((CSVExchangeAward *)obj)->_eid, ((CSVExchangeAward *)obj)));
			break;
		case CSV_ACTIVE:
			obj = new CSVActive();
			((CSVActive *)obj)->_id = atoi(p->getData(j, 0));
			((CSVActive *)obj)->_type = atoi(p->getData(j, 1));
			((CSVActive *)obj)->_url = p->getData(j, 2);
			vec.insert(make_pair(((CSVActive *)obj)->_id, ((CSVActive *)obj)));
			break;
		case CSV_PROP:
			obj = new CSVProp();
			((CSVProp *)obj)->_pid = atoi(p->getData(j, 0));
			((CSVProp *)obj)->_name = p->getData(j, 1);
			vec.insert(make_pair(((CSVProp *)obj)->_pid, ((CSVProp *)obj)));
			break;
		case CSV_FREE:
			obj = new CSVFree();
			((CSVFree *)obj)->_tid = atoi(p->getData(j, 0));
			((CSVFree *)obj)->_title = p->getData(j, 1);
			((CSVFree *)obj)->_content = p->getData(j, 2);
			((CSVFree *)obj)->_count = atoi(p->getData(j, 3));
			((CSVFree *)obj)->_rewardid = getIntFromstr(p->getData(j, 4));
			vec.insert(make_pair(((CSVFree *)obj)->_tid, ((CSVFree *)obj)));
			break;
		default:
			break;
		}
	}
	m_Objects.at(filekey) = vec;

}

vector<int> CSVDataInfo::getIntFromstr(string str){
	vector<int> vec;
	int index = str.find("|");
	string temp=str;
	while (index != -1){
		int a = atoi(temp.substr(0, index).c_str());
		vec.push_back(a);
		temp = temp.substr(index + 1, temp.length());
		index = temp.find("|");
	}
	int a = atoi(temp.c_str());
	vec.push_back(a);
	return vec;
}

Object *CSVDataInfo::getData(int key, CSVSTRUCT csvenum){
	Object *obj = NULL;
	std::map<int, Object *> vec = m_Objects.at(csvenum);
	
	if (vec.find(key) != vec.end()){
		obj = vec.at(key);
	}
	return obj;
}

int CSVDataInfo::getDataSize(CSVSTRUCT csvenu){
	std::map<int, Object *> vec = m_Objects.at(csvenu);
	return vec.size();
}

std::map<int, Object *> CSVDataInfo::getDatas(CSVSTRUCT csvenum){
	std::map<int, Object *> vec = m_Objects.at(csvenum);
	return vec;
}