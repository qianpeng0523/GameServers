#ifndef __REDIS_H__
#define __REDIS_H__

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "hiredis.h"
#include "YMSocketData.h"
#include "Hall.h"
#include "Login.h"
#include "LogicServer.h"
#include "DBProto.h"

using namespace ::google::protobuf;
using namespace protocol;
class redis{
public:
	redis();
	~redis();
	static redis *getIns();
	bool initial(std::string redisIp, int redisPort, std::string mypass);
	void release();
	void set(std::string key, std::string value);
	void Hash(std::string key, Message *msg);
	Message *getHash(std::string key, std::string name = "");
	std::string get(std::string key);
private:
	vector<string> PushDataToRedis(std::string key, Message *msg);
	Message *PopDataFromRedis(std::string key, map<string, string>maps);
private:
	redisContext* m_pConnect;
	redisReply* m_pReply;
	static redis *m_ins;
};

#endif