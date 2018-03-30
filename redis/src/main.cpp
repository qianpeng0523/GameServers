// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "redis.h"
// 线程A 方法


int main()
{
	
	
	redis rd;
	rd.initial("127.0.0.1", 6379, "qp");

	getchar();
	
	return 0;
}

#endif