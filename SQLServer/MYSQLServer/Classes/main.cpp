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
// 线程A 方法
DWORD WINAPI ThreadHttp(LPVOID lPVOID){
	HttpEvent::getIns()->init();
	return GetCurrentThreadId();
}





int main()
{
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