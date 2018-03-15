// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "HttpLogic.h"
#include "thread"
#include "LibEvent.h"
#include "Poco/Util/Timer.h"



int _tmain(int argc, _TCHAR* argv[])
{
	
	HttpLogic::getIns()->requestManagerData();
	HttpLogic::getIns()->requestGateData();
	
	getchar();
	return 0;
}

#endif