// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "HttpLogic.h"
#include "thread"


DWORD WINAPI threadClient(LPVOID lPVOID){
	HttpLogic::getIns()->requestManagerData();
	return GetCurrentThreadId();
}

int _tmain(int argc, _TCHAR* argv[])
{
	CreateThread(NULL, 0, &threadClient, NULL, 0, NULL);
	thread t1(&threadClient, NULL);
	t1.detach();
	HttpLogic::getIns()->requestGateData();
	
	getchar();
	return 0;
}

#endif