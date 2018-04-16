#include "CSVDataInfo.h"

CSVDataInfo *CSVDataInfo::m_ins=NULL;

CSVDataInfo::CSVDataInfo(){
	
}

CSVDataInfo::~CSVDataInfo(){
	std::map<CSVSTRUCT, CSVDataHelper *>::iterator itr = m_CSVDataHelpers.begin();
	while (itr!=m_CSVDataHelpers.end())
	{
		delete itr->second;
		m_CSVDataHelpers.erase(itr++);
	}
}

bool CSVDataInfo::init()
{
	
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
	p->openAndResolveFile(file.c_str(),filekey);
	if (m_CSVDataHelpers.find(filekey) != m_CSVDataHelpers.end()){
		m_CSVDataHelpers.at(filekey) = p;
	}
	else{
		m_CSVDataHelpers.insert(make_pair(filekey, p));
	}

}

string CSVDataInfo::getDataHuItem(string key, CSVSTRUCT csvenum){
	string obj;
	map<string, int> vec = getDatasHuItem(csvenum);
	
	if (vec.find(key) != vec.end()){
		return key;
	}
	return obj;
}

int CSVDataInfo::getDataSize(CSVSTRUCT csvenu){
	return getDatasHuItem(csvenu).size();
}

map<string, int> CSVDataInfo::getDatasHuItem(CSVSTRUCT csvenum){
	map<string, int> vec;
	if (m_CSVDataHelpers.find(csvenum) != m_CSVDataHelpers.end()){
		CSVDataHelper *p = m_CSVDataHelpers.at(csvenum);
		vec = p->getDataHuItems();
		return vec;
	}
	return vec;
}