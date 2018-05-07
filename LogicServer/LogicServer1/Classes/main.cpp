// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "ClientSocket.h"
#include "HttpLogic.h"
#include "redis.h"
#include "ConfigData.h"
#include "RoomInfo.h"
#include "RoomLogicInfo.h"
#include "PingInfo.h"
int _tmain(int argc, _TCHAR* argv[])
{
	PingInfo::getIns();
	RoomInfo::getIns();
	RoomLogicInfo::getIns();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	redis::getIns()->initial("www.lesharecs.com", 6379, "3.1415926qp");
	PingInfo::getIns()->openCheckUpdate(true);
	HttpLogic::getIns()->requestManagerData();

	//ConfigData::getIns();
	getchar();
	return 0;
}

#endif