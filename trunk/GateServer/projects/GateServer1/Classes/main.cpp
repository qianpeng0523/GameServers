// testlibeventSrv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "HttpLogic.h"
#include "thread"
#include "LibEvent.h"

DWORD WINAPI threadClient(int lPVOID){
	HttpLogic::getIns()->requestManagerData();
	return GetCurrentThreadId();
}

int _tmain(int argc, _TCHAR* argv[])
{
	thread t1(&threadClient, NULL);
	t1.detach();
	HttpLogic::getIns()->requestGateData();
	
	getchar();
	return 0;
}

#endif