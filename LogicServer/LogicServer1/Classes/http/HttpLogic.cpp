#include "HttpLogic.h"
#include "CSVDataInfo.h"
#include "HttpEvent.h"
#include "ClientSocket.h"
#include "Common.h"
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"



HttpLogic *HttpLogic::m_Ins = NULL;


HttpLogic::HttpLogic(){
	
}
HttpLogic::~HttpLogic(){
	
}

bool HttpLogic::init()
{
	
    return true;
}

HttpLogic *HttpLogic::getIns(){
	if (!m_Ins){
		m_Ins = new HttpLogic();
		m_Ins->init();
	}
	return m_Ins;
}

void HttpLogic::requestManagerData(){
	string url=sqlhttp;
	YMSocketData sd;
	sd["cmd"] = 0x0A;
	HttpEvent::getIns()->requestData(url,sd);
}

void HttpLogic::ManagerDataCall(YMSocketData sd){
	int err = sd["err"].asInt();
	if (err == 0){
		SERVER_PORT = sd["serverport"].asInt();
		SERVER_IP = sd["serverip"].asString();
		SERVER_CODE = sd["servername"].asString();
		ClientSocket::getIns()->connect(SERVER_IP.c_str(), SERVER_PORT);
	}
	else{
		printf("未获取到数据\n");
	}
}

void HttpLogic::respondleLogic(YMSocketData sd){
	int cmd = sd["cmd"].asInt();
	if (cmd == 0x0A){
		ManagerDataCall(sd);
	}
}

void HttpLogic::HandleLogic(YMSocketData sd, char *&buff, int &sz){
	printf("socketdata:%s",sd.getJsonString().c_str());
}