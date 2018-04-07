#ifndef __CSVDataInfo_SCENE_H__
#define __CSVDataInfo_SCENE_H__

#include "CSVDataHelper.h"
#include "CSVBaseData.h"

enum CSVSTRUCT{
	CSV_ROBOT,
	CSV_LOGOCSERVER,
	CSV_GATESERVER,
	CSV_LOGOCMANAGER,

	CSV_HU5,
	CSV_HU52,
	CSV_HU8,
	CSV_HU82,
	CSV_HU11,
	CSV_HU112,
	CSV_HU14,
	CSV_HU142,

	CSV_BAOHU5,
	CSV_BAOHU52,
	CSV_BAOHU8,
	CSV_BAOHU82,
	CSV_BAOHU11,
	CSV_BAOHU112,
	CSV_BAOHU14,
	CSV_BAOHU142
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

	void eraseData(CSVSTRUCT csvenum);
private:
	static CSVDataInfo *m_ins;
	CSVDATAS m_pCSVDataInfos;
public:
	std::map<CSVSTRUCT, std::map<int, Object *>> m_Objects;//_widgetid 
	
	std::map<CSVSTRUCT, CSVDataHelper *>m_CSVDataHelpers;
};

#endif 
