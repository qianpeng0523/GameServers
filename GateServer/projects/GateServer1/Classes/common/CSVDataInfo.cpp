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

void CSVDataInfo::eraseData(CSVSTRUCT csvenum){
	if (m_Objects.find(csvenum) != m_Objects.end()){
		std::map<int, Object *> vecs = m_Objects.find(csvenum)->second;
		std::map<int, Object *>::iterator itr2 = vecs.begin();
		while (itr2 != vecs.end()){
			delete itr2->second;
			itr2++;
		}
		vecs.clear();
		m_Objects.erase(m_Objects.find(csvenum));
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
	p->openAndResolveFile(file.c_str());
	if (m_CSVDataHelpers.find(filekey) != m_CSVDataHelpers.end()){
		m_CSVDataHelpers.at(filekey) = p;
	}
	else{
		m_CSVDataHelpers.insert(make_pair(filekey, p));
	}

	if (m_Objects.find(filekey) == m_Objects.end()){
		std::map<int, Object *> vec;
		m_Objects.insert(make_pair(filekey, vec));
	}

	int rowlen = p->getRowLength();
	int collen = p->getColLength();
	std::map<int, Object *> vec = m_Objects.at(filekey);
	for (int j = 0; j < rowlen; j++){
		Object *obj = NULL;
		switch (filekey)
		{
		case CSV_HU5:
		case CSV_HU52:
		case CSV_HU8:
		case CSV_HU82:
		case CSV_HU11:
		case CSV_HU112:
		case CSV_HU14:
		case CSV_HU142:
		case CSV_BAOHU5:
		case CSV_BAOHU52:
		case CSV_BAOHU8:
		case CSV_BAOHU82:
		case CSV_BAOHU11:
		case CSV_BAOHU112:
		case CSV_BAOHU14:
		case CSV_BAOHU142:
			obj = new CSVHuItem();
			((CSVHuItem *)obj)->_key = p->getData(j, 0);
			((CSVHuItem *)obj)->_value = atoi(p->getData(j, 1));
			vec.insert(make_pair(j, ((CSVHuItem *)obj)));
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