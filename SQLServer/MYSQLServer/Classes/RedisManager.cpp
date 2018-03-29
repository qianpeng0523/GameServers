#include "RedisManager.h"
#include "hiredis.h"
#include <iostream>

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

using namespace std;

std::string mypass = "zjml123456&&REDIS";
RedisManager::RedisManager(){}

RedisManager::~RedisManager()
{}

bool RedisManager::initial()
{
	//redis服务器ip
	const char*  redisIp = "192.168.1.40";
	//redis服务器端口
	int  redisPort = 6379;
	//连接redis服务器
	m_pConnect = redisConnect(redisIp, redisPort);	                
    m_pReply = NULL;
	if(!m_pConnect)
	{
		return false;
	}
	if (m_pConnect != NULL && m_pConnect->err) {
		cout << " redis connect failed!!!!" << endl;
		return false;
	}
    //根据密码登录
	m_pReply =(redisReply*) redisCommand(m_pConnect, "AUTH %s", mypass.c_str()); 
	if(!m_pReply)
	{
		cout << "redis exe failed!!"<<endl;
		return false;	
	}
	
	if (!(m_pReply->type == REDIS_REPLY_STATUS && strcasecmp(m_pReply->str, "OK") == 0))
	{ 
		cout << " redis auth failed!!!!" << endl;
		freeReplyObject(m_pReply );
		m_pReply = NULL;
		return false;
	}
	
	freeReplyObject(this->m_pReply );
	m_pReply = NULL;
	
	cout << " redis auth success!!!!" << endl;
	return true;
}


void RedisManager::release()
{
	//释放连接
	 freeReplyObject(m_pConnect);
	 m_pConnect = NULL;
	m_pReply = NULL;
}

void RedisManager::set(std::string key, std::string value)
{
	//设置key和value关系，插入redis
	redisReply* r =(redisReply*)redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value.c_str());   
	if(!r)
	{
		cout << "set redis faliled" << endl;
		return;
	}
        
	//执行失败
	if( !(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK")==0))  
	{
		cout << "set redis faliled" << endl;
		freeReplyObject(r );
		return;	
	}
	
	cout << "set redis success"<<endl;
	freeReplyObject(r );
}

std::string RedisManager::get(std::string key)
{
	//根据key获取value
	m_pReply = (redisReply*)redisCommand(this->m_pConnect, "GET %s", key.c_str());
	if(!m_pReply)
	{
		cout << "get value failed" << endl;
		return "";
	}
 	//get成功返回结果为 REDIS_REPLY_STRING 
	if( m_pReply->type != REDIS_REPLY_STRING )  
	{
		cout << "get redis faliled" << endl;
		freeReplyObject(m_pReply );
		m_pReply = NULL;
		return "";	
	}
	cout << "get redis success"<<endl;
	std::string valuestr = m_pReply->str;
	freeReplyObject(m_pReply );
	m_pReply = NULL;
	 
	return valuestr ;
}




