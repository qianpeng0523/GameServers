// testlibeventSrv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "LibEvent.h"
#include "Poco/Thread.h"
using namespace Poco;



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