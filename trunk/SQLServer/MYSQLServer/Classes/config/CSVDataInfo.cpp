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
		case CSV_HUTEST:
			obj = new CSVHuTest();
			((CSVHuTest *)obj)->_name = p->getData(j, 0);
			((CSVHuTest *)obj)->_content = p->getData(j, 1);
			((CSVHuTest *)obj)->_peng = p->getData(j, 2);
			((CSVHuTest *)obj)->_minggang = p->getData(j, 3);
			((CSVHuTest *)obj)->_angang = p->getData(j, 4);
			((CSVHuTest *)obj)->_bugang = p->getData(j, 5);
			((CSVHuTest *)obj)->_chi = p->getData(j, 6);


			((CSVHuTest *)obj)->_copyData();
			vec.insert(make_pair(j - 1, ((CSVHuTest *)obj)));
			break;
		case CSV_CONFIG:
			obj = new CSVConfig();
			((CSVConfig *)obj)->_server_ip = p->getData(j, 0);
			((CSVConfig *)obj)->_server_port = atoi(p->getData(j, 1));
			((CSVConfig *)obj)->_sql_ip = p->getData(j, 2);
			((CSVConfig *)obj)->_sql_port = atoi(p->getData(j, 3));
			((CSVConfig *)obj)->_log = atoi(p->getData(j, 4));
			((CSVConfig *)obj)->_robot = atoi(p->getData(j, 5));
			((CSVConfig *)obj)->_roomcount = atoi(p->getData(j, 6));
			vec.insert(make_pair(j - 1, ((CSVConfig *)obj)));
			break;
		case CSV_HUFAN:
			obj = new CSVHuFan();
			((CSVHuFan *)obj)->_id = atoi(p->getData(j, 0));
			((CSVHuFan *)obj)->_name = p->getData(j, 1);
			((CSVHuFan *)obj)->_number = atoi(p->getData(j, 2));

			vec.insert(make_pair(((CSVHuFan *)obj)->_id, ((CSVHuFan *)obj)));
			break;
		case CSV_RULE_TYPE_FAN:
		case CSV_OTHER_TYPE_FAN:
		case CSV_GANG_TYPE_FAN:
		case CSV_GANGHU_TYPE_FAN:
			obj = new FanItem();
			((FanItem *)obj)->_id = atoi(p->getData(j, 0));
			((FanItem *)obj)->_name = p->getData(j, 1);
			((FanItem *)obj)->_number = atoi(p->getData(j, 2));
			((FanItem *)obj)->_add = atoi(p->getData(j, 3));
			((FanItem *)obj)->_mutil = atoi(p->getData(j, 4));

			vec.insert(make_pair(((FanItem *)obj)->_id, ((FanItem *)obj)));
			break;
		case  CSV_OVERDATA_TYPE_FAN:
			obj = new OverFanItem();
			((OverFanItem *)obj)->_id = atoi(p->getData(j, 0));
			((OverFanItem *)obj)->_kind = atoi(p->getData(j, 1));
			((OverFanItem *)obj)->_oid = atoi(p->getData(j, 2));
			vec.insert(make_pair(((OverFanItem *)obj)->_id, ((OverFanItem *)obj)));
			break;
		case CSV_GAMEOVERDATA_TYPE_FAN:
			obj = new GameOverData();
			((GameOverData *)obj)->_id = atoi(p->getData(j, 0));
			((GameOverData *)obj)->_name = p->getData(j, 1);
			((GameOverData *)obj)->_count = atoi(p->getData(j, 2));
			((GameOverData *)obj)->_type = p->getData(j, 3);
			vec.insert(make_pair(((GameOverData *)obj)->_id, ((GameOverData *)obj)));
			break;
		case CSV_ROBOT:
			obj = new RobotData();
			((RobotData *)obj)->_id = atoi(p->getData(j, 0));
			((RobotData *)obj)->_name = p->getData(j, 1);
			vec.insert(make_pair(((RobotData *)obj)->_id, ((RobotData *)obj)));
			break;
		default:
			break;
		}
	}
	m_Objects.at(filekey) = vec;

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