#include "HttpLogic.h"
#include "CSVDataInfo.h"
#include "HttpEvent.h"
#include "LibEvent.h"
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
	char buff[4096];
	int sz = 0;
	sd.serializer(buff, &sz);
	buff[sz] = '\0';
	
	url += buff;
	url = Common::replace_all(url, "\r", REPLACESTRR);
	url = Common::replace_all(url, "\n", REPLACESTRN);
	HttpEvent::getIns()->requestData(url);
}

void HttpLogic::ManagerDataCall(YMSocketData sd){
	int err = sd["err"].asInt();
	if (err == 0){
		SERVER_PORT = sd["serverport"].asInt();
		SERVER_CODE = sd["servername"].asString();
		std::cout << "socket start:" <<SERVER_PORT<< std::endl;
		LibEvent *clib = LibEvent::getIns();
		clib->StartServer(SERVER_PORT, 2, 5000, 600, 600);
		getchar();
		clib->StopServer();
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