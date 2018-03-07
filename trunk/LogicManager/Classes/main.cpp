// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "LibEvent.h"


int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "socket start!" << std::endl;
	LibEvent *clib = LibEvent::getIns();
	clib->StartServer(SERVER_PORT, 2, 5000, 600, 600);
	getchar();
	clib->StopServer();
	return 0;
}

#endif