#include "CSVDataHelper.h"
#include <algorithm>
#include "XXIconv.h"
#include "Object.h"


CSVDataHelper::CSVDataHelper()
 :m_seperator(",")
 , m_colLength(0)
{

}

CSVDataHelper::~CSVDataHelper()
{
	m_objecs.clear();
}

#pragma region reselove the content begin...

bool CSVDataHelper::openAndResolveFile(const char *fileName, CSVSTRUCT csv)
{
	m_csvtype = csv;
	std::string pathKey = fileName;
	unsigned char* pBuffer = NULL;
	int bufferSize = 0;
	FILEDATA *fdata = getFileByName(fileName);
	pBuffer = fdata->_data;
	bufferSize = fdata->_len;

	std::string tmpStr = (char*)pBuffer;
	std::string fileContent = tmpStr.substr(0, bufferSize);

	//std::vector<std::string> line;
	rowSplit(m_row, fileContent, '\n');
	for (unsigned int i = 0; i < m_row.size(); ++i) {
		std::vector<std::string> fieldVector;
		fieldSplit(fieldVector, m_row[i]);
		setDataToObejct(fieldVector,i);
		//data.push_back(fieldVector);
		//m_colLength = max(m_colLength, (int)fieldVector.size());
	}
	delete pBuffer;
	return true;
}

void CSVDataHelper::setDataToObejct(std::vector<std::string> fieldVector,int index){
	if (m_csvtype >= CSV_HU5&&m_csvtype <= CSV_BAOHU1424){
		string key;
		if (!fieldVector.empty()){
			key = fieldVector.at(0);
		}
		m_objecs.insert(make_pair(key,0));
	}
}

map<string, int> CSVDataHelper::getDataHuItems(){
	return m_objecs;
}

void CSVDataHelper::rowSplit(std::vector<std::string> &rows, const std::string &content, const char &rowSeperator)
{
	std::string::size_type lastIndex = content.find_first_not_of(rowSeperator, 0);
	std::string::size_type    currentIndex = content.find_first_of(rowSeperator, lastIndex);

	while (std::string::npos != currentIndex || std::string::npos != lastIndex) {
		rows.push_back(content.substr(lastIndex, currentIndex - lastIndex));
		lastIndex = content.find_first_not_of(rowSeperator, currentIndex);
		currentIndex = content.find_first_of(rowSeperator, lastIndex);
	}
}

void CSVDataHelper::fieldSplit(std::vector<std::string> &fields, std::string line)
{
	if (line[line.length() - 1] == '\r') {
		line = line.substr(0, line.length() - 1);
	}

	std::string field=line;
	int index = field.find(",");
	while (index!=-1) {
		fields.push_back(field.substr(0, index));
		field = field.substr(index,field.length());
		index = field.find(",");
	}
	fields.push_back(field);
}

#pragma region end.

///////search data
const char *CSVDataHelper::getData(unsigned int rowIndex, unsigned int colIndex)
{
	if (rowIndex >= getRowLength() || colIndex >= getColLength()) {
		return "";
	}

	if (colIndex >= data[rowIndex].size()) {
		return "";
	}

	return data[rowIndex][colIndex].c_str();
}

string CSVDataHelper::getColName(int col){
	return m_row[col];
}

FILEDATA *CSVDataHelper::getFileByName(string filename){
	FILE *fd = fopen(filename.c_str(), "rb+");
	fseek(fd, 0, SEEK_END); //文件指针移到文件尾
	int nLen = ftell(fd);  //得到当前指针位置, 即是文件的长度
	rewind(fd);    //文件指针恢复到文件头位置

	//动态申请空间, 为保存字符串结尾标志\0, 多申请一个字符的空间
	unsigned char *pchBuf = new unsigned char[nLen + 1];
	if (!pchBuf)
	{
		CLog::log( "内存不够!\n");
		exit(0);
	}

	//读取文件内容//读取的长度和源文件长度有可能有出入，这里自动调整 nLen
	nLen = fread(pchBuf, sizeof(unsigned char), nLen, fd);

	pchBuf[nLen] = '\0'; //添加字符串结尾标志
	//log::CLog::log((char*)pchBuf);
	FILEDATA *p = new FILEDATA();
	p->_data = pchBuf;
	p->_len = nLen;
	fclose(fd);
	//delete pchBuf;
	return p;
}