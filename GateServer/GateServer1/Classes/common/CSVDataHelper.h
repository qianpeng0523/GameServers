#ifndef __Cell__CSVDataHelper__
#define __Cell__CSVDataHelper__

#include "stdafx.h"

using namespace std;

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

	bool openAndResolveFile(const char *fileName);
	bool openAndResolveFile(const char *fileName, map<string, char> &mps);

	const char *getData(unsigned int rowIndex, unsigned int colIndex);

	inline int getColLength() { return m_colLength; }
	inline int getRowLength() { return data.size(); }

	string getColName(int col);//获取列的名称
	FILEDATA *getFileByName(string filename);
private:
	const std::string m_seperator;
	std::vector<std::vector<std::string> > data;
	std::vector<std::string> m_row;
	//cols length
	int m_colLength;

	void rowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator);
	void fieldSplit(std::vector<std::string> &fields, std::string line);

	//获取带引号的字段
	int getFieldWithQuoted(const std::string &line, std::string& field, int index);

	//获取无引号的字段
	int getFieldNoQuoted(const std::string &line, std::string &field, int index);
};

#endif /* defined(__Cell__CSVDataHelper__) */