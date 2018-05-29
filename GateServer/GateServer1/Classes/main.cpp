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
#include "ClientSocket.h"
#include "LoginInfo.h"
#include "StatTimer.h"
#include "RedisGet.h"
#include "ConfigData.h"
#include "Common.h"
#include "HttpPay.h"
#include "HttpWXLogin.h"
#include "HttpAliPay.h"
#include "RoomInfo.h"
#include "RoomLogicInfo.h"
#include "PingInfo.h"
#include "CLog.h"
#include "ConfigInfo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//1.生成所有的刻子
	vector<string> ke;
	for (int i = 0; i < 9; i++){
		string vc;
		for (int j = 0; j < 3; j++){
			int v = i + 1;
			vc += (char)(v +'0');
		}
		ke.push_back(vc);
	}
	//2.生成所有的连子
	vector<string> lian;
	for (int i = 0; i < 7; i++){
		int v1 = i + 1;
		int v2 = i + 2;
		int v3 = i + 3;
		string vc;
		vc += (char)(v1 + '0');
		vc += (char)(v2 + '0');
		vc += (char)(v3 + '0');
		lian.push_back(vc);
	}
	//组合所有的刻子和连子
	map<string, string>mps;
	for (int i = 0; i < lian.size(); i++){
		string t1 = lian.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps.find(con) == mps.end()){
			mps.insert(make_pair(con, ""));
		}
		for (int j = i; j < lian.size(); j++){
			string t2 = lian.at(j);
			con = t1 + t2;
			sort(con.begin(), con.end());
			if (mps.find(con) == mps.end()){
				mps.insert(make_pair(con, ""));
			}
			for (int k = j; k < lian.size(); k++){
				string t3 = lian.at(k);
				con = t1 + t2 + t3;
				sort(con.begin(), con.end());
				if (mps.find(con) == mps.end()){
					mps.insert(make_pair(con, ""));
				}
				for (int m = k; m < lian.size(); m++){
					string t4 = lian.at(m);
					con = t1+t2+t3+t4;
					sort(con.begin(),con.end());
					if (mps.find(con) == mps.end()){
						mps.insert(make_pair(con, ""));
					}
				}
			}
		}
	}
	mps.insert(make_pair("", ""));
	map<string, string>mps1;
	for (int i = 0; i < ke.size(); i++){
		string t1 = ke.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps1.find(con) == mps1.end()){
			mps1.insert(make_pair(con, ""));
		}
		for (int j = i+1; j < ke.size(); j++){
			string t2 = ke.at(j);
			string con = t1+t2;
			sort(con.begin(), con.end());
			if (mps1.find(con) == mps1.end()){
				mps1.insert(make_pair(con, ""));
			}
			for (int m = j+1; m < ke.size(); m++){
				string t3 = ke.at(m);
				string con = t1 + t2+t3;
				sort(con.begin(), con.end());
				if (mps1.find(con) == mps1.end()){
					mps1.insert(make_pair(con, ""));
				}
				for (int n = m+1; n < ke.size(); n++){
					string t4 = ke.at(n);
					string con = t1 + t2 + t3+t4;
					sort(con.begin(), con.end());
					if (mps1.find(con) == mps1.end()){
						mps1.insert(make_pair(con, ""));
					}
				}

			}

		}
		
	}
	mps1.insert(make_pair("", ""));
	//组合
	map<string, string>allmps;
	auto itr = mps.begin();
	for (itr; itr != mps.end();itr++){
		string con1 = itr->first;
		auto itr1 = mps1.begin();
		for (itr1; itr1 != mps1.end();itr1++){
			string con2 = itr1->first;
			string con = con1+con2;
			int len = strlen(con.c_str());
			if (len <= 12){
				for (int i = 0; i < 9;i++){
					int co = count(con.begin(),con.end(),i+'0');
					if (co == 4){
						continue;
					}
				}
				sort(con.begin(), con.end());
				if (allmps.find(con) == allmps.end()){
					allmps.insert(make_pair(con, ""));
				}
			}
		}
	}

	//终极所有
	for (int i = 0; i < allmps.size();i++){

	}

	printf("\n");

	/*
	map<int, vector<int>>mps;
	for (int i = 0; i < 100; i++){
		int co = i/10+i%10;
		vector<int >vec;
		if (mps.find(co) != mps.end()){
			vec = mps.at(co);
			vec.push_back(i);
			mps.at(co) = vec;
		}
		else{
			vec.push_back(i);
			mps.insert(make_pair(co,vec));
		}
	}
	map<int, float> mplv;
	float lvlv = 0;
	for (auto itr = mps.begin(); itr != mps.end();itr++){
		int co = itr->first;
		vector<int> vec=itr->second;
		int sz = vec.size();
		float lv = sz*1.0/100;
		mplv.insert(make_pair(co,lv));
	}
	float ying = 0.0;
	map<int, int> zhongda;
	map<int, int> zhongxiao;
	for (auto itr = mplv.begin(); itr != mplv.end();itr++){
		int co = itr->first;
		float lv = itr->second;
		int dabei = 0;
		int xiaobei = 0;
		int count = mps.at(co).size();
		for (int i = 1;; i++){
			if (i*count>100*(1-ying)){
				dabei = i - 1;
				break;
			}
			else if (xiaobei==0&& i*count > 1){
				xiaobei = i;
			}
		}
		zhongda.insert(make_pair(co, dabei));
		zhongxiao.insert(make_pair(co, xiaobei));
	}
	string content="中奖号码,随机值,中奖概率,赔率(最大赔率),投注金额100中奖(最大值)\n";
	char buff[100];
	for (auto itr = mps.begin(); itr != mps.end(); itr++){
		int key = itr->first;
		sprintf(buff, "%02d",key);
		content += buff;
		content += ",";
		vector<int>number = itr->second;
		int sz = number.size();
		content += "{";
		for (int i = 0; i < sz; i++){
			if (i < sz - 1){
				sprintf(buff, "%02d ", number.at(i));
			}
			else{
				sprintf(buff, "%02d}", number.at(i));
			}
			content += buff;
		}
		content += ",";
		float zhonglv = mplv.at(key);
		sprintf(buff, "%g", zhonglv);
		content += buff;
		content += ",";
		int dapeilv = zhongda.at(key);
		int xiaopeilv = zhongxiao.at(key);
		sprintf(buff, "%d",dapeilv);
		content += buff;
		content += ",";
		sprintf(buff, "%d", dapeilv*100);
		content += buff;
		content += "\n";
	}
	FILE *fp = fopen("./res/byyx.csv","w+");
	fseek(fp,0,SEEK_END);
	fprintf(fp,content.c_str());
	fclose(fp);
	*/
	
	ConfigInfo::getIns();
	PingInfo::getIns();
	RoomInfo::getIns();
	RoomLogicInfo::getIns();
	StatTimer::getIns();
	CSVDataInfo::getIns();
	LoginInfo::getIns();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
	redis::getIns()->initial("www.lesharecs.com", 6379, "3.1415926qp");
	RedisGet::getIns()->init();
	//HttpAliPay::getIns()->requestOrder("100001", "1", 1, "test 1", "", 2);
	//ConfigData::getIns();
	
	LoginInfo::getIns()->openCheckUpdate(true);
	HallInfo::getIns();
	HttpLogic::getIns()->requestGateData();
	HttpLogic::getIns()->requestManagerData();
	
	HttpWXLogin::getIns();
	HttpAliPay::getIns();
	HttpPay::getIns()/*->test()*/;

	getchar();
	return 0;
}

#endif