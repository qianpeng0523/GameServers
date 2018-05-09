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
	GRoom *enterRoom(string uid, string rid);
	GRoom *getGRoom_(string uid);
	GRoom *getGRoom(string rid);
	void eraseRoom(string rid);

	bool BeginMJ(int type,string uid);
	bool ReadyMJ(string uid,bool ready);
	bool LeaveMJ(string uid, bool zhudong);
	bool onLine(string uid,bool online);
	bool DissolveRoom(string uid,string rid);
	bool Vote(string uid,bool agree);

	void test();
private:
	string createRoom();
private:
	static RoomControl *m_ins;
	map<string, bool>m_roomids;
	map<string, GRoom*>m_roomdatas;
};


#endif // __LYNX_GAME_SERVER_RoomControl_H__