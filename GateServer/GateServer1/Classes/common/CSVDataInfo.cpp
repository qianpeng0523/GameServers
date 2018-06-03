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
	for (int j = 1; j < rowlen; j++){
		Object *obj = NULL;
		switch (filekey)
		{
		
		default:
			break;
		}
	}
	m_Objects.at(filekey) = vec;

}

void CSVDataInfo::openCSVFile(string file, string &mps){
	CSVDataHelper *p = new CSVDataHelper();
	p->openAndResolveFile(file.c_str(),mps);
	delete p;
}

void CSVDataInfo::openCSVFile(string file,std::map<int, int> &mps){
	CSVDataHelper *p = new CSVDataHelper();
	p->openAndResolveFile(file.c_str());
	int rowlen = p->getRowLength();
	int collen = p->getColLength();
	for (int j = 0; j < rowlen; j++){
		int index = atoi(p->getData(j,0));
		mps.insert(make_pair(index,j));
	}
	delete p;
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


vector<int> CSVDataInfo::getIntFromstr(string str, string fenge){
	vector<int> vec;
	int index = str.find(fenge);
	string temp = str;
	while (index != -1){
		int a = atoi(temp.substr(0, index).c_str());
		vec.push_back(a);
		temp = temp.substr(index + 1, temp.length());
		index = temp.find(fenge);
	}
	int a = atoi(temp.c_str());
	vec.push_back(a);
	return vec;
}


vector<string> CSVDataInfo::getStrFromstr(string str, string fenge){
	vector<string> vec;
	int index = str.find(fenge);
	string temp = str;
	while (index != -1){
		string a = temp.substr(0, index);
		vec.push_back(a);
		temp = temp.substr(index + 1, temp.length());
		index = temp.find(fenge);
	}
	vec.push_back(temp);
	return vec;
}
