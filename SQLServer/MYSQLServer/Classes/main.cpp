// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "DataBaseUserinfo.h"
#include <thread>
#include "HttpEvent.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"
#include "HttpLogic.h"
#include "redis.h"
// 线程A 方法
DWORD WINAPI ThreadHttp(LPVOID lPVOID){
	HttpEvent::getIns()->init();
	return GetCurrentThreadId();
}





int main()
{
	
	CSVDataInfo::getIns();
	HttpLogic::getIns();
	CreateThread(NULL, 0, ThreadHttp, NULL, 0, NULL);
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);

	redis::getIns()->initial("127.0.0.1", 6379, "3.1415926qp");
	
 	DBUserInfo user;
	user.set_card(1);
	user.set_ip("192.168.1.1");
	redis::getIns()->Hash(user.GetTypeName(),&user);
	DBUserInfo *user1= (DBUserInfo *)redis::getIns()->getHash(user.GetTypeName());
	//SqlControl::getIns();
	
	getchar();
	
	return 0;
}

#endif