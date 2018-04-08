#ifndef __Cell__CSVDataHelper__
#define __Cell__CSVDataHelper__

#include "stdafx.h"
#include "CSVBaseData.h"

using namespace CSV;
using namespace std;

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

	CSV_BAOHU51,
	CSV_BAOHU52,
	CSV_BAOHU53,
	CSV_BAOHU54,
	CSV_BAOHU521,
	CSV_BAOHU522,
	CSV_BAOHU523,
	CSV_BAOHU524,
	CSV_BAOHU81,
	CSV_BAOHU82,
	CSV_BAOHU83,
	CSV_BAOHU84,
	CSV_BAOHU821,
	CSV_BAOHU822,
	CSV_BAOHU823,
	CSV_BAOHU824,
	CSV_BAOHU111,
	CSV_BAOHU112,
	CSV_BAOHU113,
	CSV_BAOHU114,
	CSV_BAOHU1121,
	CSV_BAOHU1122,
	CSV_BAOHU1123,
	CSV_BAOHU1124,
	CSV_BAOHU141,
	CSV_BAOHU142,
	CSV_BAOHU143,
	CSV_BAOHU144,
	CSV_BAOHU1421,
	CSV_BAOHU1422,
	CSV_BAOHU1423,
	CSV_BAOHU1424,
};

struct FILEDATA
{
	unsigned char *_data;
	long _len;
};

class CSVDataHelper
{
public:
	CSVDataHelper();
	~CSVDataHelper();

	bool openAndResolveFile(const char *fileName, CSVSTRUCT csv);

	const char *getData(unsigned int rowIndex, unsigned int colIndex);

	inline int getColLength() { return m_colLength; }
	inline int getRowLength() { return data.size(); }

	string getColName(int col);//获取列的名称
	FILEDATA *getFileByName(string filename);

	map<string, int> getDataHuItems();
private:
	
	void rowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator);
	void fieldSplit(std::vector<std::string> &fields, std::string line);

	//获取带引号的字段
	int getFieldWithQuoted(const std::string &line, std::string& field, int index);

	//获取无引号的字段
	int getFieldNoQuoted(const std::string &line, std::string &field, int index);

	void setDataToObejct(std::vector<std::string> fieldVector, int index);
private:
	map<string,int> m_objecs;
	CSVSTRUCT m_csvtype;

	const std::string m_seperator;
	std::vector<std::vector<std::string> > data;
	std::vector<std::string> m_row;
	//cols length
	int m_colLength;

};

#endif /* defined(__Cell__CSVDataHelper__) */