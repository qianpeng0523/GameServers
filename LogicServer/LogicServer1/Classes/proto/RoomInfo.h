
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
	void SendSHMMJCreateRoom(SHMMJCreateRoom sd);

	void HandCHMMJEnterRoom(ccEvent *event);
	void SendSHMMJEnterRoom(SHMMJEnterRoom sd);

	void HandCBegin(ccEvent *event);
	void SendSBegin(SBegin sd);

	void HandCReady(ccEvent *event);
	void SendSReady(SReady sd);

	void HandCLeave(ccEvent *event);
	void SendSLeave(SLeave sd);

	void HandCLine(ccEvent *event);
	void SendSLine(SLine sd);

	void HandCDissolveRoom(ccEvent *event);
	void SendSDissolveRoom(SDissolveRoom sd);

	void HandCVote(ccEvent *event);
	void SendSVote(SVote sd);

	void SendSVoteResult(SVoteResult sd);

private:
	static RoomInfo *m_shareRoomInfo;
	
};

#endif 
