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
	
	SqlControl::getIns();
	
	getchar();
	
	return 0;
}

#endif