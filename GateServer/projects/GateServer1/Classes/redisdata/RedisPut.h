#ifndef __LYNX_GAME_SERVER_RedisPut_H__
#define __LYNX_GAME_SERVER_RedisPut_H__
#include "stdafx.h"

using namespace std;


class RedisPut 
{
public:
	RedisPut();
	virtual ~RedisPut();
	
	static RedisPut *getIns();
	void init();

	bool PushPass(string uid,string pass);
	bool PushUserBase(UserBase ub);
	bool PushRank(Rank rk);

	bool PushShop(ShopItem item);
private:
	static RedisPut *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisPut_H__