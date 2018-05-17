// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __MAIN_H__
#define __MAIN_H__
#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include <thread>
#include "HttpEvent.h"
#include "CSVDataInfo.h"
#include "HttpLogic.h"
#include "redis.h"
#include "RedisGet.h"
#include "MD55.h"
#include "RedisPut.h"

// 线程A 方法
DWORD WINAPI ThreadHttp(LPVOID lPVOID){
	HttpEvent::getIns()->init();
	return GetCurrentThreadId();
}


void RandEXCode(){
	char *at = redis::getIns()->getLastList("CSVExchangeCode");
	if (!at){
		Common p;
		vector<int>vv;
		vv.push_back(1);
		vv.push_back(2);
		vv.push_back(3);
		vv.push_back(5);
		vector<int> vec = CSVDataInfo::getIns()->getRewardID(vv);
		int sz = vec.size();
		int num = 2;
		char buff[10];
		map<string, CSVExchangeCode>maps;
		for (int i = 0; i < 1000; i++){
			CSVExchangeCode p;
			p._id = i + 1;
			for (int j = 0; j < num; j++){
				int rid = vec.at(rand() % sz);
				sprintf(buff,"%d",rid);
				p._rewardid += buff;
				if (j == 0){
					p._rewardid += "|";
				}
			}
			p._code = Common::RandStr(8, true);
			while (maps.find(p._code) != maps.end()){
				p._code = Common::RandStr(8, true);
			}
			maps.insert(make_pair(p._code, p));
		}
		auto itr = maps.begin();
		int index = 1;
		for (itr; itr != maps.end();itr++){
			printf("%d.",index++);
			CSVExchangeCode p=itr->second;
			RedisPut::getIns()->PushExchangeCode(&p);
		}
	}
}


int main()
{
// 	YMSocketData sd1;
// 	sd1["nn"] = "1234";
// 	sd1["type"] = 222;
// 	char buff[1024];
// 	int sz = 0;
// 	sd1.serializer(buff, &sz);
// 	buff[1] = '\0';
// 	YMSocketData sd = HttpEvent::getIns()->getSocketDataByStr(buff, sz);
// 	printf("%d\n",sd.getJsonString().c_str());
	
	CSVDataInfo::getIns();
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	redis::getIns()->initial("47.104.165.65", 6379, "3.1415926qp");
	RedisGet::getIns()->init();

	RandEXCode();

	HttpLogic::getIns();
	CreateThread(NULL, 0, ThreadHttp, NULL, 0, NULL);
	
	
	
	//SqlControl::getIns();
	
	getchar();
	
	return 0;
}

#endif