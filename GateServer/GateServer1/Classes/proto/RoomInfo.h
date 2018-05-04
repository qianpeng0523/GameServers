
#ifndef __RoomInfo__
#define __RoomInfo__

#include "stdafx.h"
#include "ccEvent.h"

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
	void HandSRChat(ccEvent *event);
private:
	static RoomInfo *m_shareRoomInfo;
	
};

#endif 
