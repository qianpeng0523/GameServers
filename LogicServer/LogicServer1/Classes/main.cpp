// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __SERVER_H__
#define __SERVER_H__
#include "stdafx.h"
#include "ClientSocket.h"
#include "HttpLogic.h"
#include "redis.h"
#include "ConfigData.h"
int _tmain(int argc, _TCHAR* argv[])
{
	/*
	FILE *file = fopen("xing2-1.csv","w+");
	string content = "中奖的注数,赔率,投注金额100中奖\n";
	int max1 = 0;
	int max2 = 0;

	//1.只中一个
	float a = 27.0/100;//100个号码中选取27个号，顺序不限，号码不能相同
	float b = 2.0/100;//100个号码中选取2个号，顺序不限，号码不能相同
	//概率
	float lv = a*b;
	//倍率
	char buff[100];
	for (int i = 1; i <= 27 * 13; i++){
		int m = 2;
		while (1){
			if (i*m*lv >= 1){
				printf("[%g]:[%d]\n", m*lv, m);
				sprintf(buff,"%d",i);
				content += buff;
				content += ",";
				sprintf(buff, "%d", m);
				content += buff;
				content += ",";
				sprintf(buff, "%d\n", m*100);
				content += buff;
				break;
			}
			m++;
		}
	}
	fprintf(file,content.c_str(),content.length());
	fclose(file);
	//2.有两个相同的号
	FILE *file1 = fopen("xing2-2.csv", "w+");
	string content1 = "中奖的注数,赔率,投注金额100中奖\n";
	b = 1.0/100;
	lv = a*b;
	for (int i = 1; i <= 27 * 13; i++){
		int m = 2;
		while (1){
			if (i*m*lv >= 1){
				printf("[%g]:[%d]\n", m*lv, m);
				sprintf(buff, "%d", i);
				content1 += buff;
				content1 += ",";
				sprintf(buff, "%d", m);
				content1 += buff;
				content1 += ",";
				sprintf(buff, "%d\n", m * 100);
				content1 += buff;
				break;
			}
			m++;
		}
	}
	fprintf(file1, content1.c_str(), content1.length());
	fclose(file1);
	

	FILE *file = fopen("xing3-1.csv", "w+");
	string content = "中奖的注数,赔率,投注金额100中奖\n";
	
	//1.只中一个
	float a = 27.0 / 100;//100个号码中选取27个号，顺序不限，号码不能相同
	float b = 3.0 / 100;//100个号码中选取2个号，顺序不限，号码不能相同
	//概率
	float lv = a*b;
	//倍率
	char buff[100];
	for (int i = 1; i <= 9 * 13; i++){
		int m = 2;
		while (1){
			if (i*m*lv >= 1){
				printf("[%g]:[%d]\n", m*lv, m);
				sprintf(buff, "%d", i);
				content += buff;
				content += ",";
				sprintf(buff, "%d", m);
				content += buff;
				content += ",";
				sprintf(buff, "%d\n", m * 100);
				content += buff;
				break;
			}
			m++;
		}
	}
	fprintf(file, content.c_str(), content.length());
	fclose(file);
	//2.有两个相同的号
	FILE *file1 = fopen("xing3-2.csv", "w+");
	string content1 = "中奖的注数,赔率,投注金额100中奖\n";
	b = 2.0 / 100;
	lv = a*b;
	for (int i = 1; i <= 9 * 13; i++){
		int m = 2;
		while (1){
			if (i*m*lv >= 1){
				printf("[%g]:[%d]\n", m*lv, m);
				sprintf(buff, "%d", i);
				content1 += buff;
				content1 += ",";
				sprintf(buff, "%d", m);
				content1 += buff;
				content1 += ",";
				sprintf(buff, "%d\n", m * 100);
				content1 += buff;
				break;
			}
			m++;
		}
	}
	fprintf(file1, content1.c_str(), content1.length());
	fclose(file1);

	//3.有三个相同的号
	FILE *file2 = fopen("xing3-3.csv", "w+");
	string content2 = "中奖的注数,赔率,投注金额100中奖\n";
	b = 1.0 / 100;
	lv = a*b;
	for (int i = 1; i <= 9 * 13; i++){
		int m = 2;
		while (1){
			if (i*m*lv >= 1){
				printf("[%g]:[%d]\n", m*lv, m);
				sprintf(buff, "%d", i);
				content2 += buff;
				content2 += ",";
				sprintf(buff, "%d", m);
				content2 += buff;
				content2 += ",";
				sprintf(buff, "%d\n", m * 100);
				content2 += buff;
				break;
			}
			m++;
		}
	}
	fprintf(file2, content2.c_str(), content2.length());
	fclose(file2);
	*/
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	redis::getIns()->initial("www.lesharecs.com", 6379, "3.1415926qp");
	HttpLogic::getIns()->requestManagerData();

	//ConfigData::getIns();
	getchar();
	return 0;
}

#endif