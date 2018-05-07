
#ifndef __RoomInfo__
#define __RoomInfo__

#include "stdafx.h"
#include "ccEvent.h"
#include "EventDispatcher.h"

struct RoomCache
{
	string _rid;
	string _fzuid;
	vector<string> _uids;
};

class RoomInfo:public Object
{
public:
	RoomInfo();
	~RoomInfo();
	static RoomInfo* getIns();
	bool init();
public:
	void HandCHMMJCreateRoom(ccEvent *event);
	void HandSHMMJCreateRoom(ccEvent *event);

	void HandCHMMJEnterRoom(ccEvent *event);
	void HandSHMMJEnterRoom(ccEvent *event);

	void HandSComein(ccEvent *event);

	void HandCBegin(ccEvent *event);
	void HandSBegin(ccEvent *event);

	void HandCReady(ccEvent *event);
	void HandSReady(ccEvent *event);

	void HandCLeave(ccEvent *event);
	void HandSLeave(ccEvent *event);

	void HandCLine(ccEvent *event);
	void HandSLine(ccEvent *event);

	void HandCDissolveRoom(ccEvent *event);
	void HandSDissolveRoom(ccEvent *event);

	void HandCVote(ccEvent *event);
	void HandSVote(ccEvent *event);

	void HandSVoteResult(ccEvent *event);

	void HandCRChat(ccEvent *event);
	void SendSRChat(SRChat sd);

	void PushRoom(string rid,string uid);
	string getRoomId(string uid);
	void PopRoom(string rid);
	void PopUserFromRoom(string uid);
	vector<string> getRoomUsers(string rid);
private:
	static RoomInfo *m_shareRoomInfo;
	map<string, RoomCache *> m_pRooms;
	map<string, string> m_roomids;
};

#endif 
