// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "ClientSocket.h"
#include "HttpLogic.h"
#include "RoomControl.h"
#include "ConfigData.h"
int _tmain(int argc, _TCHAR* argv[])
{
	HttpLogic::getIns()->requestManagerData();
	redis::getIns()->initial("www.lesharecs.com",  6379, "3.1415926qp");
	ConfigData::getIns();
	getchar();
	return 0;
}

#endif