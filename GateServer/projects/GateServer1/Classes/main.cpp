// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "HttpLogic.h"
#include "thread"
#include "LibEvent.h"
#include "Poco/Util/Timer.h"
#include "LoginInfo.h"
#include "HallInfo.h"

int _tmain(int argc, _TCHAR* argv[])
{
// 	SSignList cl;
// 	cl.set_sign(1);
// 	cl.set_count(8);
// 	cl.set_err(1);
// 	for (int i = 0; i < 4;i++){
// 		SignAward *sa = cl.add_reward();
// 		sa->set_id(i + 1);
// 		sa->set_day(i + 2);
// 	}
// 
// 	YMSocketData sd;
// 	LoginInfo::getIns()->setDBDataToSocketDataVo(&cl, sd);
// 	printf("debug:%s\n",sd.getJsonString().c_str());
// 	
// 	SSignList cl1;
// 	SSignList *cl2 = (SSignList *)LoginInfo::getIns()->getDBDataFromSocketDataVo(cl1.GetTypeName(), sd);
// 	printf("%s",cl2->DebugString().c_str());
	HallInfo::getIns();
	HttpLogic::getIns()->requestManagerData();
	HttpLogic::getIns()->requestGateData();
	
	getchar();
	return 0;
}

#endif