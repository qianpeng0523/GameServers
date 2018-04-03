#include "CSVDataInfo.h"

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