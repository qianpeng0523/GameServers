#ifndef __ccEvent_H__
#define __ccEvent_H__

#include "stdafx.h"


// #pragma comment(lib,"../Server1/Debug/libprotobuf.lib") 
// 
// #pragma comment(lib,"../Server1/Debug/libprotoc.lib") 

using namespace std;

class ccEvent :public Object{
public:
	ccEvent(int pcmd, const char* data, int size, int fd);
	~ccEvent();
	int cmd;
	::google::protobuf::Message* msg;
private:
	void parse(const char* data, int size);
public:
	static google::protobuf::Message* create_message(const std::string& type_name);
	int m_fd;
};

#endif // __ccccEvent_H__
