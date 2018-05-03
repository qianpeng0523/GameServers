#ifndef __ccEvent_H__
#define __ccEvent_H__

#include "stdafx.h"



enum SERVERTYPE
{
	NO_TYPE = -1,
	GATE_TYPE = 0,
	LOGIC_TYPE,
};

// #pragma comment(lib,"../Server1/Debug/libprotobuf.lib") 
// 
// #pragma comment(lib,"../Server1/Debug/libprotoc.lib") 

using namespace std;

class ccEvent :public Object{
public:
	ccEvent(int pcmd, const char* data, int size, int fd,string servername);
	int m_cmd;
	::google::protobuf::Message* msg;
private:
	void parse(const char* data, int size);
	string m_servername;
public:
	static google::protobuf::Message* create_message(const std::string& type_name);
	int m_fd;
	SERVERTYPE m_type;
	
};

#endif // __ccccEvent_H__
