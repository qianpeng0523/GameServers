// testlibeventSrv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "ClientSocket.h"
#include "HttpLogic.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HttpLogic::getIns()->requestManagerData();
	
	getchar();
	return 0;
}

#endif