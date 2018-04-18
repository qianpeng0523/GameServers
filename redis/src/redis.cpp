#include "redis.h"
#include "fmacros.h"
#include "Common.h"
#ifndef _WIN32
#include <unistd.h>
#endif
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <assert.h>
#include <fcntl.h>
#include <limits.h>

#ifdef _WIN32
#include <fcntl.h>
#ifndef FD_SETSIZE
#define FD_SETSIZE 16000
#endif
#ifndef STDIN_FILENO
#define STDIN_FILENO (_fileno(stdin))
#endif
#include <winsock2.h>
#include <windows.h>
#include "win32fixes.h"
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoull _strtoui64
#endif


#include "zmalloc.h"
#include "help.h"
#include "anet.h"
#include "ae.h"
#include "ccEvent.h"
#include "HttpLogic.h"

redis *redis::m_ins = NULL;
redis *redis::getIns(){
	if (!m_ins){
		m_ins = new redis();
	}
	return m_ins;
}

redis::redis(){
	m_pConnect = NULL;
}

redis::~redis(){

}

bool redis::isConnect(){
	if (m_pConnect&& !m_pConnect->err) {
		redisReply* r = (redisReply*)redisCommand(this->m_pConnect, "ping");
		if (!r)
		{
			return false;
		}

		//执行失败
		if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "PONG") == 0))
		{
			freeReplyObject(r);
			return false;
		}
		freeReplyObject(r);
		return true;
	}
	return false;
}

bool redis::initial(std::string redisIp,int redisPort,std::string mypass)
{
	if (redisIp.empty() && mypass.empty()){
		printf("redisip or pass error!\n");
		return false;
	}
	//连接redis服务器
	struct timeval timeout = { 1, 500000 };
	m_pConnect = redisConnectWithTimeout(redisIp.c_str(), redisPort, timeout);
	m_pReply = NULL;
	if (!m_pConnect)
	{
		return false;
	}
	if (m_pConnect != NULL && m_pConnect->err) {
		printf("redis connect failed:err:%d,errstr:%s\n", m_pConnect->err, m_pConnect->errstr);
		return false;
	}
	//根据密码登录
	m_pReply = (redisReply*)redisCommand(m_pConnect, "AUTH %s", mypass.c_str());
	if (!m_pReply)
	{
		printf("redis exe failed\n");
		return false;
	}

	if (!(m_pReply->type == REDIS_REPLY_STATUS && strcasecmp(m_pReply->str, "OK") == 0))
	{
		printf("redis auth failed\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return false;
	}
	m_ip = redisIp;
	m_port = redisPort;
	m_pass = mypass;
	freeReplyObject(this->m_pReply);
	m_pReply = NULL;
	//printf("redis auth success\n");
	return true;
}

bool redis::reconnect(){
	bool isc=redis::getIns()->initial(m_ip, m_port, m_pass);
	return isc;
}

void redis::release()
{
	//释放连接
	if (m_pConnect){
		freeReplyObject(m_pConnect);
		m_pConnect = NULL;
		m_pReply = NULL;
	}
}

bool redis::delKey(std::string key){
	//设置key和value关系，插入redis
	if (!this->m_pConnect){
		reconnect();
		return false;
	}
	
	redisReply* r = (redisReply*)redisCommand(this->m_pConnect, "del %s", key.c_str());
	if (!r)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0))
	{
		printf("set redis faliled\n");
		freeReplyObject(r);
		return false;
	}
	//printf("set redis success\n");
	freeReplyObject(r);
	return true;
}

bool redis::set(std::string key, char* value, int len)
{
	//设置key和value关系，插入redis
	if (!this->m_pConnect){
		reconnect();
		return false;
	}
	ZeroChange(value, len);
	printf("*************************************%s\n",value);
	redisReply* r = (redisReply*)redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value);
	if (!r)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0))
	{
		printf("set redis faliled\n");
		freeReplyObject(r);
		return false;
	}
	//printf("set redis success\n");
	freeReplyObject(r);
	return true;
}

bool redis::set(string key, int value){
	if (!this->m_pConnect){
		reconnect();
		return false;
	}
	char buff[30];
	sprintf(buff,"%d",value);
	redisReply* r = (redisReply*)redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), buff);
	if (!r)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0))
	{
		printf("set redis faliled\n");
		freeReplyObject(r);
		return false;
	}
	//printf("set redis success\n");
	freeReplyObject(r);
	return true;
}

char* redis::get(std::string key, int &len)
{
	//根据key获取value
	len = 0;
	if (!this->m_pConnect){
		reconnect();
		return NULL;
	}
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "GET %s", key.c_str());
	if (!m_pReply)
	{
		printf("get value failed\n");
		return NULL;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type != REDIS_REPLY_STRING)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return NULL;
	}
	//printf("get redis success\n");
	len = m_pReply->len;
	char* valuestr=new char[len+1];
	strcpy(valuestr,m_pReply->str);
	ChangeToZero(valuestr, len);
	freeReplyObject(m_pReply);
	m_pReply = NULL;

	return valuestr;
}

bool redis::Hash(std::string key, Message *msg){
	if (!this->m_pConnect){
		reconnect();
		return false;
	}
	std::vector<std::string>vec = PushDataToRedis(key,msg);
	for (int i = 0; i < vec.size();i++){
		string content = vec.at(i);
		redisReply* r = (redisReply*)redisCommand(this->m_pConnect, content.c_str());
		if (!r)
		{
			printf("set redis faliled\n");
			return false;
		}

		//执行失败
		if (r->type == REDIS_REPLY_ERROR || r->str)
		{
			printf("set redis faliled\n");
			freeReplyObject(r);
			return false;
		}
		//printf("set redis success\n");
		freeReplyObject(r);

	}
	return true;
}

bool redis::Hash(std::string key, std::string name, std::string value){
	Common::replace_all(value, " ", KONGGE);
	redisReply* r = (redisReply*)redisCommand(this->m_pConnect, ("hset "+key+" "+name+" "+value).c_str());
	if (!r)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (r->type == REDIS_REPLY_ERROR || r->str)
	{
		printf("set redis faliled\n");
		freeReplyObject(r);
		return false;
	}
	//printf("set redis success\n");
	freeReplyObject(r);
	return true;
}

bool redis::addHash(std::string key, std::string name, int value){
	char buff[300];
	sprintf(buff,"hincrby %s %s %d",key,name,value);
	redisReply* r = (redisReply*)redisCommand(this->m_pConnect, buff);
	if (!r)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (r->type == REDIS_REPLY_ERROR || r->str)
	{
		printf("set redis faliled\n");
		freeReplyObject(r);
		return false;
	}
	//printf("set redis success\n");
	freeReplyObject(r);
	return true;
}

Message *redis::getHash(std::string key, string msgname){
	Message *msg;
	map<string, string>datas;
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "hgetall %s", key.c_str());
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return NULL;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return NULL;
	}
	int sz = m_pReply->elements;
	for (int i = 0; i < sz;i+=2){
		redisReply *rpname = m_pReply->element[i];
		redisReply *rpvalues = m_pReply->element[i+1];
		string name = rpname->str;
		char* value = rpvalues->str;
		int len = rpvalues->len;
		if (strcmp(value,"default") == 0){
			strcpy(value , "");
		}
		datas.insert(make_pair(name, value));
	}
	if (datas.size()>0){
		msg = PopDataFromRedis(msgname, datas);
	}
	else{
		printf("get redis faild\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return NULL;
	}
	printf("get redis success\n");
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return msg;
}

map<string, string> redis::getHash(std::string key){
	map<string, string>datas;
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "hgetall %s", key.c_str());
	if (!m_pReply)
	{
		reconnect();
		printf("datas value failed\n");
		return datas;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return datas;
	}
	int sz = m_pReply->elements;
	for (int i = 0; i < sz; i += 2){
		redisReply *rpname = m_pReply->element[i];
		redisReply *rpvalues = m_pReply->element[i + 1];
		string name = rpname->str;
		char* value = rpvalues->str;
		Common::replace_all(value, KONGGE, " ");
		int len = rpvalues->len;
		if (strcmp(value, "default") == 0){
			strcpy(value, "");
		}
		datas.insert(make_pair(name, value));
	}
	printf("get redis success\n");
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return datas;
}

bool redis::List(std::string key, char* value){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "rpush %s %s", key.c_str(), value);
	if (!m_pReply)
	{
		printf("set redis faliled\n");
		return false;
	}

	//执行失败
	if (m_pReply->type == REDIS_REPLY_ERROR || m_pReply->str)
	{
		printf("set redis faliled\n");
		freeReplyObject(m_pReply);
		return false;
	}
	printf("set redis success\n");
	freeReplyObject(m_pReply);

	return true;
}

bool redis::List(std::string key, Message *msg){
	int sz = msg->ByteSize();
	char *buffer = new char[sz];
	msg->SerializePartialToArray(buffer, sz);
	ZeroChange(buffer,sz);
	return List(key, buffer);
}

bool redis::List(string key, map<string, int>vec){
	string tt ;
	map<string, int>::iterator itr = vec.begin();
	for (itr; itr != vec.end();itr++){
		tt += itr->first+" ";
	}
	return List(key,(char *)tt.c_str());
}
/*

Lrem key-name count value——根据count值从列表中删除与value相等的项，
(1)count>0，从左到右删除最多count个与value相等的项
(2)count<0，从右到左删除最多count个与value相等的项
(3)count=0，删除所有与value相等的项

**/
int redis::eraseList(string key, string value){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, " lrem  %s %d %s", key.c_str(), 0,value.c_str());
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return 0;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return 0;
	}

	//printf("get redis success\n");
	int integer = m_pReply->integer;
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return integer;
}

map<uint64, int> redis::getList(string key){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "lrange %s %d %d", key.c_str(), 0, -1);
	map<uint64, int> vec;
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return vec;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return vec;
	}

	
	int sz = m_pReply->elements;
	for (int i = 0; i < sz; i+=2){
		redisReply *rpvalues = m_pReply->element[i];
		string value = rpvalues->str;
		int len = rpvalues->len;
		vec.insert(make_pair(atoll(value.c_str()),len));
		if (i + 1 < sz){
			rpvalues = m_pReply->element[i + 1];
			value = rpvalues->str;
			len = rpvalues->len;
			vec.insert(make_pair(atoll(value.c_str()), len));
		}
	}
	//printf("get redis success\n");
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return vec;
}

bool redis::setList(std::string key, string keyname, string value, Message *msg1){
	vector<Message *>vec = redis::getIns()->getList(key,msg1->GetTypeName());
	char buff[100];
	for (int i = 0; i < vec.size(); i++){
		Message *msg = vec.at(i);
		::google::protobuf::Descriptor *des = (::google::protobuf::Descriptor *)msg->GetDescriptor();
		::google::protobuf::Reflection *reflection = (::google::protobuf::Reflection *) msg->GetReflection();
		int sz = des->field_count();
		for (int j = 0; j < sz; j++){
			::google::protobuf::FieldDescriptor *fd = (::google::protobuf::FieldDescriptor *)des->field(j);
			string name = fd->name();
			google::protobuf::FieldDescriptor::Type type = fd->type();
			if (name.compare(keyname) == 0){
				switch (type)
				{
				case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
					sprintf(buff, "%g", reflection->GetDouble(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_FLOAT:
					sprintf(buff, "%g", reflection->GetFloat(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_INT64:
					sprintf(buff, "%ld", reflection->GetInt64(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT64:
					sprintf(buff, "%ld", reflection->GetUInt64(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_INT32:
					sprintf(buff, "%d", reflection->GetInt32(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_BOOL:
					sprintf(buff, "%d", reflection->GetBool(*msg, fd));
					break;
				case google::protobuf::FieldDescriptor::TYPE_STRING:
					sprintf(buff, "%s", reflection->GetString(*msg, fd).c_str());
					break;
				case google::protobuf::FieldDescriptor::TYPE_BYTES:
					sprintf(buff, "%s", reflection->GetString(*msg, fd).c_str());
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT32:
					sprintf(buff, "%ld", reflection->GetUInt32(*msg, fd));
					break;
				default:
					break;
				}
				break;
			}
			if (value.compare(buff) == 0){
				int sz = msg1->ByteSize();
				char *buffer = new char[sz];
				msg1->SerializePartialToArray(buffer, sz);
				ZeroChange(buffer, sz);
				m_pReply = (redisReply*)redisCommand(this->m_pConnect, "lset %s %d %s", (key + msg1->GetTypeName()).c_str(), i, buffer);
				bool ist = true;
				if (!m_pReply)
				{
					printf("set redis faliled\n");
					ist = false;
					releaseMessages(vec);
					return false;
				}
				if (ist && m_pReply->type == REDIS_REPLY_ERROR){
					ist = false;
					printf("set redis faliled\n");
					freeReplyObject(m_pReply);
					releaseMessages(vec);
					return false;
				}
				//printf("set redis success\n");
				freeReplyObject(m_pReply);
				releaseMessages(vec);
				return ist;
			}
		}
	}
	return false;
}

void redis::releaseMessages(vector<Message *>vecs){
	for (int i = 0; i < vecs.size(); i++){
		Message *msg = vecs.at(i);
		delete msg;
		msg = NULL;
	}
}

vector<char *> redis::getList(string key, vector<int> &lens, int beginindex, int endindex){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "lrange %s %d %d", key.c_str(), beginindex, endindex);
	vector<char *> vec;
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return vec;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return vec;
	}
	int sz = m_pReply->elements;
	for (int i = 0; i < sz; i++){
		redisReply *rpvalues = m_pReply->element[i];
		char *value = rpvalues->str;
		int len = rpvalues->len;
		char *nn = new char[len+1];
		memset(nn,0,len+1);
		memcpy(nn,value,len);
		redis::getIns()->ChangeToZero(nn, len);
		vec.push_back(nn);
		lens.push_back(len);
	}

	//printf("get redis success\n");
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return vec;
}

std::vector<Message *> redis::getList(std::string key, string mesname, int beginindex, int endindex){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "lrange %s %d %d", key.c_str(),beginindex,endindex);
	std::vector<Message *> vec;
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return vec;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return vec;
	}
	int sz = m_pReply->elements;
	for (int i = 0; i < sz; i++){
		Message *msg=ccEvent::create_message(mesname);
		redisReply *rpvalues = m_pReply->element[i];
		char *value = rpvalues->str;
		int len= rpvalues->len;
		redis::getIns()->ChangeToZero(value, len);
		msg->ParsePartialFromArray(value, len);
		vec.push_back(msg);
	}
	
	//printf("get redis success\n");
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return vec;
}

std::vector<Message *> redis::getList(std::string key, string mesname){
	return getList(key,mesname,0,-1);
}

Message *redis::PopDataFromRedis(std::string key, map<string, string>maps){
	char buff[512];
	Message *msg = ccEvent::create_message(key);
	::google::protobuf::Descriptor *des = (::google::protobuf::Descriptor *)msg->GetDescriptor();
	::google::protobuf::Reflection *reflection = (::google::protobuf::Reflection *) msg->GetReflection();
	int sz = des->field_count();
	for (int i = 0; i < sz; i++){
		::google::protobuf::FieldDescriptor *fd = (::google::protobuf::FieldDescriptor *)des->field(i);
		string name = fd->name();
		if (name.compare("cmd") == 0){
			continue;
		}
		if (maps.find(name) != maps.end()){
			string value = maps.at(name);
			::google::protobuf::FieldDescriptor::Type type = fd->type();
			bool ist = false;
			string t;
			switch (type)
			{
			case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
				reflection->SetDouble(msg, fd, atof(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_FLOAT:
				reflection->SetFloat(msg, fd, atof(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_INT64:
				reflection->SetInt64(msg, fd, atol(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_UINT64:
				reflection->SetUInt64(msg, fd, atol(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_INT32:
				reflection->SetInt32(msg, fd, atoi(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_BOOL:
				reflection->SetBool(msg, fd, atoi(value.c_str()));
				break;
			case google::protobuf::FieldDescriptor::TYPE_STRING:
				reflection->SetString(msg, fd, value);
				break;
			case google::protobuf::FieldDescriptor::TYPE_BYTES:
				reflection->SetString(msg, fd, value);
				break;
			case google::protobuf::FieldDescriptor::TYPE_UINT32:
				reflection->SetUInt32(msg, fd, atol(value.c_str()));
				break;

			default:
				ist = true;
				break;
			}
		}
	}
	return msg;
}

vector<string> redis::PushDataToRedis(std::string key, Message *msg){
	char buff[512];
	::google::protobuf::Descriptor *des = (::google::protobuf::Descriptor *)msg->GetDescriptor();
	::google::protobuf::Reflection *reflection = (::google::protobuf::Reflection *) msg->GetReflection();
	int sz = des->field_count();
	vector<string> vec;
	for (int i = 0; i < sz; i++){
		::google::protobuf::FieldDescriptor *fd = (::google::protobuf::FieldDescriptor *)des->field(i);
		string name = fd->name();
		
		if (name.compare("cmd") == 0){
			continue;
		}
		string content = "hset " + key + " ";
		content += name+" ";
		::google::protobuf::FieldDescriptor::Type type = fd->type();
		
		::google::protobuf::Message *msg1 = NULL;
		bool ist=false;
		string t;
		switch (type)
		{
		case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
			sprintf(buff, "%g", reflection->GetDouble(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_FLOAT:
			sprintf(buff, "%g", reflection->GetFloat(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT64:
			sprintf(buff, "%ld", reflection->GetInt64(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT64:
			sprintf(buff, "%ld", reflection->GetUInt64(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT32:
			sprintf(buff, "%d", reflection->GetInt32(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_BOOL:
			sprintf(buff, "%d", reflection->GetBool(*msg, fd));
			break;
		case google::protobuf::FieldDescriptor::TYPE_STRING:
			t = reflection->GetString(*msg, fd);
			if (t.empty()){
				t = "default";
			}
			sprintf(buff, "%s", t.c_str());
			break;
		case google::protobuf::FieldDescriptor::TYPE_BYTES:
			t = reflection->GetString(*msg, fd);
			if (t.empty()){
				t = "default";
			}
			sprintf(buff, "%s", t.c_str());
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT32:
			sprintf(buff, "%ld", reflection->GetUInt32(*msg, fd));
			break;
		
		default:
			ist = true;
			break;
		}
		content +=buff;
		if (!ist){
			vec.push_back(content);
		}
	}
	
	
	return vec;
}

void redis::ZeroChange(char *&data, int sz){
	for (int i = 0; i < sz; i++){
		if (data[i] == '\0'){
			data[i] = ZERO_STR;
		}
	}
}

void redis::ChangeToZero(char *&data, int sz){
	for (int i = 0; i < sz; i++){
		if (data[i] == ZERO_STR){
			data[i] = '\0';
		}
	}
}

char *redis::getLastList(string key){
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "lindex %s -1", key.c_str());
	if (!m_pReply)
	{
		reconnect();
		printf("get value failed\n");
		return NULL;
	}
	//get成功返回结果为 REDIS_REPLY_STRING 
	if (m_pReply->type == REDIS_REPLY_ERROR)
	{
		printf("get redis faliled\n");
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return NULL;
	}
	//printf("get redis success\n");
	char *value = m_pReply->str;
	if (value){
		int len = m_pReply->len;
		char* valuestr = new char[len + 1];
		memset(valuestr, 0, len + 1);
		strcpy(valuestr, value);
		freeReplyObject(m_pReply);
		m_pReply = NULL;
		return valuestr;
	}
	
	freeReplyObject(m_pReply);
	m_pReply = NULL;
	return value;
}

bool redis::getList(string key, map<string, int> &vec){
	return true;
}