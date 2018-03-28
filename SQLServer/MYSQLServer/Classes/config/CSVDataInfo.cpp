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
			((GateData *)obj)->_name = p->getData(j, 1);
			((GateData *)obj)->_ip = p->getData(j, 2);
			((GateData *)obj)->_port =atoi(p->getData(j, 3));
			((GateData *)obj)->_type = atoi(p->getData(j, 4));
			vec.insert(make_pair(((GateData *)obj)->_id, ((GateData *)obj)));
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