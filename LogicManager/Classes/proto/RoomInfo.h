
#ifndef __RoomInfo__
#define __RoomInfo__

#include "stdafx.h"
#include "ccEvent.h"

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
	void SendSHMMJCreateRoom(SHMMJCreateRoom sd, int fd);

	void HandCHMMJEnterRoom(ccEvent *event);
	void SendSHMMJEnterRoom(SHMMJEnterRoom sd, int fd);

	void SendSComein(SComein sd, int fd);

	void HandCBegin(ccEvent *event);
	void SendSBegin(SBegin sd, int fd);

	void HandCReady(ccEvent *event);
	void SendSReady(SReady sd, int fd);

	void HandCLeave(ccEvent *event);
	void SendSLeave(SLeave sd, int fd);

	void HandCLine(ccEvent *event);
	void SendSLine(SLine sd, int fd);

	void HandCDissolveRoom(ccEvent *event);
	void SendSDissolveRoom(SDissolveRoom sd, int fd);

	void HandCVote(ccEvent *event);
	void SendSVote(SVote sd, int fd);

	void SendSVoteResult(SVoteResult sd, int fd);

	void HandCRChat(ccEvent *event);
	void SendSRChat(SRChat sd, int fd);

	void PushRoom(string rid,string uid);
private:
	static RoomInfo *m_shareRoomInfo;
	map<string, RoomCache *> m_pRooms;
};

#endif 
