
#ifndef __RoomLogicInfo__
#define __RoomLogicInfo__

#include "stdafx.h"
#include "ccEvent.h"

class RoomLogicInfo:public Object
{
public:
	RoomLogicInfo();
	~RoomLogicInfo();
	static RoomLogicInfo* getIns();
	bool init();
public:
	void HandSDice(ccEvent *event);
	void HandCDice(ccEvent *event);

	void HandSSelectZhuang(ccEvent *event);

	void HandSZhuangCard(ccEvent *event);

	void HandSCard(ccEvent *event);

	void HandCDiscard(ccEvent *event);
	void HandSDiscard(ccEvent *event);

	void HandSDraw(ccEvent *event);

	void SendSOtherDraw(SOtherDraw sd);

	void HandChi(ccEvent *event);
	void HandSChi(ccEvent *event);

	void HandCPeng(ccEvent *event);
	void HandSPeng(ccEvent *event);

	void HandCMingGang(ccEvent *event);
	void HandSMingGang(ccEvent *event);

	void HandCAnGang(ccEvent *event);
	void HandSAnGang(ccEvent *event);

	void HandSFa(ccEvent *event);

	void HandCHu(ccEvent *event);
	void HandSHu(ccEvent *event);

	void HandSGameover(ccEvent *event);

	void HandSRoundOver(ccEvent *event);

	void HandSDissolveTip(ccEvent *event);
private:
	static RoomLogicInfo *m_shareRoomLogicInfo;
	
};

#endif 
