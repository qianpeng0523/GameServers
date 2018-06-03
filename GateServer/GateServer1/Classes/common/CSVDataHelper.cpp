#include "CSVDataHelper.h"
#include <algorithm>
#include "XXIconv.h"


CSVDataHelper::CSVDataHelper()
:m_seperator(",")
, m_colLength(0)
{

}

CSVDataHelper::~CSVDataHelper()
{

}

#pragma region reselove the content begin...

bool CSVDataHelper::openAndResolveFile(const char *fileName)
{
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
		data.push_back(fieldVector);
		m_colLength = max(m_colLength, (int)fieldVector.size());
	}
	delete fdata;
	return true;
}

bool CSVDataHelper::openAndResolveFile(const char *fileName, string &mps)
{
	std::string pathKey = fileName;
	unsigned char* pBuffer = NULL;
	int bufferSize = 0;
	FILEDATA *fdata = getFileByName(fileName);
	pBuffer = fdata->_data;
	bufferSize = fdata->_len;

	std::string tmpStr = (char*)pBuffer;
	std::string fileContent = tmpStr.substr(0, bufferSize);
	tmpStr.clear();
	tmpStr = "";
	std::string::size_type lastIndex = fileContent.find_first_not_of("\n", 0);
	std::string::size_type    currentIndex = fileContent.find_first_of("\n", lastIndex);
	mps = (char*)pBuffer;
	/*
	while (std::string::npos != currentIndex || std::string::npos != lastIndex) {
		string cc = fileContent.substr(lastIndex, currentIndex - lastIndex);
		if (cc[cc.length() - 1] == '\r') {
			cc = cc.substr(0, cc.length() - 1);
		}
		if (mps.find((char *)cc.c_str()) == mps.end()){
			int len = cc.length() + 1;
			char *bb = new char[len];
			bb[len - 1] = '\0';
			memcpy(bb,cc.c_str(),len);
			mps.insert(bb);
		}
		lastIndex = fileContent.find_first_not_of("\n", currentIndex);
		currentIndex = fileContent.find_first_of("\n", lastIndex);
	}*/
	delete fdata;
	fileContent.clear();
	fileContent = "";
	return true;
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

	std::string field;
	unsigned int i = 0, j = 0;
	while (j < line.length()) {
		if (line[i] == '"') {
			//有引号
			j = getFieldWithQuoted(line, field, i);
		}
		else {
			j = getFieldNoQuoted(line, field, i);
		}

		fields.push_back(field);
		i = j + 1; //解析下一个field， ＋1为了跳过当前的分隔符
	}
}

int CSVDataHelper::getFieldWithQuoted(const std::string &line, std::string &field, int i)
{
	unsigned int j = 0;
	field = std::string();
	if (line[i] != '"') {
		//不是引号起始，有问题
		CLog::log( "start char is not quote when call");
		return -1;
	}

	for (j = i + 1; j < line.length() - 1; ++j) {
		if (line[j] != '"') {
			//当前char不为引号，则是field内容(包括逗号)
			field += line[j];
		}
		else {
			//遇到field结束时的引号，可以返回
			return j;
			break;
		}
	}

	if (j == line.length()) {
		//没有找到成对的结束引号
		
		CLog::log("resoleve the line error: no pair quote, line:%s, field:%s, start index:%d", line.c_str(), field.c_str(), i);
	}

	return j;
}

int CSVDataHelper::getFieldNoQuoted(const std::string &line, std::string &field, int index)
{
	unsigned int j = 0;
	//找到下一个分隔符位置
	j = line.find_first_of(m_seperator, index);
	if (j > line.length()) {
		j = line.length();
	}

	field = std::string(line, index, j - index);

	return j;
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
	//CLog::log((char*)pchBuf);
	FILEDATA *p = new FILEDATA();
	p->_data = pchBuf;
	p->_len = nLen;

	//delete pchBuf;
	return p;
}