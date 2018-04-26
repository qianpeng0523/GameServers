#ifndef __LYNX_GAME_SERVER_RoomControl_H__
#define __LYNX_GAME_SERVER_RoomControl_H__
#include "stdafx.h"
#include "redis.h"
#include "GRoom.h"

using namespace std;

class RoomControl 
{
public:
	RoomControl();
	virtual ~RoomControl();
	
	static RoomControl *getIns();
	void init();
	
	GRoom *createRoom(string uid, int type, int ante, int round, int bao, int bang);
	bool enterRoom(string uid,string rid);

	void eraseRoom(string rid);

	void test();
private:
	string createRoom();
private:
	static RoomControl *m_ins;
	map<string, bool>m_roomids;
	map<string, GRoom*>m_roomdatas;
};


#endif // __LYNX_GAME_SERVER_RoomControl_H__