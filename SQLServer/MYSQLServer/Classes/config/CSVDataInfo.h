#ifndef __CSVDataInfo_SCENE_H__
#define __CSVDataInfo_SCENE_H__

#include "CSVDataHelper.h"
#include "CSVBaseData.h"

enum CSVSTRUCT{
	CSV_ROBOT,
	CSV_LOGOCSERVER,
	CSV_GATESERVER,
	CSV_LOGOCMANAGER,

	CSV_REWARD,
	CSV_SHOP,
	CSV_TASK,
	CSV_SIGN,
	CSV_ZHUANPAN,
	CSV_EXREWARD,
	CSV_ACTIVE,
	CSV_PROP,
	CSV_FREE,
	CSV_FIRSTBUY,
	CSV_EXCHANGECODE,
	CSV_REDISDB
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
	
	void openCSVFile(string file, CSVSTRUCT filekey);//打开文件并读取文件
	Object *getData(int key, CSVSTRUCT csvenum);
	int getDataSize(CSVSTRUCT csvenu);
	std::map<int, Object *> getDatas(CSVSTRUCT csvenum);
	vector<int> getIntFromstr(string str, string fenge);
	vector<string> getStrFromstr(string str, string fenge);

	vector<int> getRewardID(vector<int> pids);

	std::map<string, REDISDBName *> getDBNames();
private:
	static CSVDataInfo *m_ins;
	CSVDATAS m_pCSVDataInfos;
	
public:
	std::map<CSVSTRUCT, std::map<int, Object *>> m_Objects;//_widgetid 
	std::map<CSVSTRUCT, CSVDataHelper *>m_CSVDataHelpers;
};

#endif 
