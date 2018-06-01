#ifndef __CSVDataInfo_SCENE_H__
#define __CSVDataInfo_SCENE_H__

#include "CSVDataHelper.h"
#include "CSVBaseData.h"

enum CSVSTRUCT{
	CSV_HUS,
};

typedef std::map<CSVSTRUCT, CSVDataHelper *> CSVDATAS;



using namespace CSV;
class CSVDataInfo : public Object
{
public:
	CSVDataInfo();
	~CSVDataInfo();
    virtual bool init();
	static CSVDataInfo* getIns();
	void openCSVFile(string file, CSVSTRUCT filekey);
	void openCSVFile(string file, map<string, char> &mps);//打开文件并读取文件
	void openCSVFile(string file, std::map<int, int> &mps);
	Object *getData(int key, CSVSTRUCT csvenum);
	int getDataSize(CSVSTRUCT csvenu);
	std::map<int, Object *> getDatas(CSVSTRUCT csvenum);

	vector<int> getIntFromstr(string str, string fenge);
	vector<string> getStrFromstr(string str, string fenge);
private:
	static CSVDataInfo *m_ins;
	CSVDATAS m_pCSVDataInfos;
public:
	std::map<CSVSTRUCT, std::map<int, Object *>> m_Objects;//_widgetid 
	
	std::map<CSVSTRUCT, CSVDataHelper *>m_CSVDataHelpers;
};

#endif 
