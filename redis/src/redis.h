#ifndef __REDIS_H__
#define __REDIS_H__

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "hiredis.h"
#include "YMSocketData.h"
#include "Vo.h"
#include "Hall.h"
#include "Login.h"

#include "LogicServer.h"

#define ZERO_STR '&'

using namespace ::google::protobuf;
using namespace protocol;

class redis{
public:
	redis();
	~redis();
	static redis *getIns();
	bool initial(std::string redisIp, int redisPort, std::string mypass);
	bool reconnect();
	void release();
	bool set(std::string key, char* value,int len);
	bool set(string key,int value);
	char* get(std::string key,int &len);
	bool delKey(std::string key);

	bool Hash(std::string key,Message *msg);//比如配置那些（单独） 用户信息
	bool Hash(std::string key,std::string name,std::string value);
	bool addHash(std::string key,std::string name,int value);
	Message *getHash(std::string key,string msgname);
	map<string, string> getHash(std::string key);

	bool List(std::string key, Message *msg);//typename+id(唯一):添加元素列表 比如战绩那些（id是房间号）
	bool List(std::string key, char* value);
	bool List(string key,map<string,int>vec);
	int eraseList(string key, string value);//删除与value值相同的项

	map<uint64, int> getList(string key);
	vector<std::string> getListStr(string key);
	bool getList(string key, map<string, int> &vec);
	std::vector<Message *> getList(std::string key, string mesname);
	std::vector<Message *> getList(std::string key, string mesname,int beginindex,int endindex);
	vector<char *> getList(string key, vector<int> &lens,int beginindex=0, int endindex=-1);
	bool setList(std::string key,string keyname,string value,Message *msg);
	bool setList(std::string key, int index, string value);

	bool isConnect();
	void releaseMessages(vector<Message *>vecs);
	void ZeroChange(char *&data, int sz);
	void ChangeToZero(char *&data, int sz);

	char *getLastList(string key);
	Message *PopDataFromRedis(std::string key, map<string, string>maps);
private:
	vector<string> PushDataToRedis(std::string key, Message *msg);
	

	
private:
	redisContext* m_pConnect;
	redisReply* m_pReply;
	static redis *m_ins;
	string m_ip;
	int m_port;
	string m_pass;
};

#endif