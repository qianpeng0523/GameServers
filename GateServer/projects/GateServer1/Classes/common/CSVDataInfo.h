#ifndef __CSVDataInfo_SCENE_H__
#define __CSVDataInfo_SCENE_H__

#include "CSVDataHelper.h"
#include "CSVBaseData.h"

enum CSVSTRUCT{
	//CSV_ROOM,
	CSV_HUTEST,
	CSV_CONFIG,
	CSV_HUFAN,
	CSV_GANG_TYPE_FAN,
	CSV_GANGHU_TYPE_FAN,
	CSV_RULE_TYPE_FAN,
	CSV_OTHER_TYPE_FAN,
	CSV_OVERDATA_TYPE_FAN,
	CSV_GAMEOVERDATA_TYPE_FAN,
	CSV_ROBOT,
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
private:
	static CSVDataInfo *m_ins;
	CSVDATAS m_pCSVDataInfos;
public:
	std::map<CSVSTRUCT, std::map<int, Object *>> m_Objects;//_widgetid 
	
	std::map<CSVSTRUCT, CSVDataHelper *>m_CSVDataHelpers;
};

#endif 
