// testlibeventSrv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "ClientSocket.h"



int _tmain(int argc, _TCHAR* argv[])
{
	ClientSocket::getIns()->connect("192.168.1.103", 5600);
	getchar();
	return 0;
}

#endif