// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include <thread>
#include "HttpEvent.h"
#include "CSVDataInfo.h"
#include "HttpLogic.h"
#include "redis.h"
#include "RedisGet.h"
#include "MD55.h"

// 线程A 方法
DWORD WINAPI ThreadHttp(LPVOID lPVOID){
	HttpEvent::getIns()->init();
	return GetCurrentThreadId();
}





int main()
{
// 	YMSocketData sd1;
// 	sd1["nn"] = "1234";
// 	sd1["type"] = 222;
// 	char buff[1024];
// 	int sz = 0;
// 	sd1.serializer(buff, &sz);
// 	buff[1] = '\0';
// 	YMSocketData sd = HttpEvent::getIns()->getSocketDataByStr(buff, sz);
// 	printf("%d\n",sd.getJsonString().c_str());

	
	CSVDataInfo::getIns();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	redis::getIns()->initial("47.104.165.65", 6379, "3.1415926qp");
	RedisGet::getIns()->init();
	HttpLogic::getIns();
	CreateThread(NULL, 0, ThreadHttp, NULL, 0, NULL);
	
	
	
	//SqlControl::getIns();
	
	getchar();
	
	return 0;
}

#endif