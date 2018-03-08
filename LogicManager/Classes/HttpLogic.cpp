#include "HttpLogic.h"
#include "CSVDataInfo.h"
#include "HttpEvent.h"
#include "LibEvent.h"

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
	
	//url += buff;
	HttpEvent::getIns()->requestData(url);
}

void HttpLogic::ManagerDataCall(YMSocketData sd){
	int err = sd["err"].asInt();
	SERVER_PORT = sd["serverport"].asInt();
	std::cout << "socket start!" << std::endl;
	LibEvent *clib = LibEvent::getIns();
	clib->StartServer(SERVER_PORT, 2, 5000, 600, 600);
	getchar();
	clib->StopServer();

}

void HttpLogic::HandleLogic(YMSocketData sd, char *&buff, int &sz){
	printf("socketdata:%s",sd.getJsonString().c_str());
}