#ifndef __CSVDataInfo_SCENE_H__
#define __CSVDataInfo_SCENE_H__

#include "CSVDataHelper.h"
#include "CSVBaseData.h"

typedef std::map<CSVSTRUCT, CSVDataHelper *> CSVDATAS;



using namespace CSV;
class CSVDataInfo : public Object
{
public:
	CSVDataInfo();
	~CSVDataInfo();
    virtual bool init();
	static CSVDataInfo* getIns();
	
	void openCSVFile(string file, CSVSTRUCT filekey);//打开文件并读取文件
	string getDataHuItem(string key, CSVSTRUCT csvenum);
	int getDataSize(CSVSTRUCT csvenu);
	map<string, int> getDatasHuItem(CSVSTRUCT csvenum);
	vector<int> getIntFromstr(string str);
	vector<string> getStrFromstr(string str);
private:
	static CSVDataInfo *m_ins;
public:
	CSVDATAS m_CSVDataHelpers;
};

#endif 
