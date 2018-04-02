#ifndef __LYNX_GAME_SERVER_RedisGet_H__
#define __LYNX_GAME_SERVER_RedisGet_H__
#include "stdafx.h"

using namespace std;


class RedisGet 
{
public:
	RedisGet();
	virtual ~RedisGet();
	
	static RedisGet *getIns();
	void init();

	char* getPass(string uid);
	UserBase *getUserBase(string uid);
	std::vector<Rank >getRank(int type,int index);
private:
	static RedisGet *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisGet_H__