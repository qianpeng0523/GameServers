#ifndef __LYNX_GAME_SERVER_RedisModify_H__
#define __LYNX_GAME_SERVER_RedisModify_H__
#include "stdafx.h"

using namespace std;


class RedisModify 
{
public:
	RedisModify();
	virtual ~RedisModify();
	
	static RedisModify *getIns();
	void init();


private:
	static RedisModify *m_ins;
};


#endif // __LYNX_GAME_SERVER_RedisModify_H__