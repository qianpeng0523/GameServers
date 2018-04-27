
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
	void SendSDice(SDice sd);
	void HandCDice(ccEvent *event);

	void SendSSelectZhuang(SSelectZhuang ssz);

	void SendSZhuangCard(SZhuangCard sd);

	void SendSCard(SCard sd);

	void HandCDiscard(ccEvent *event);
	void SendSDiscard(SDiscard sd);

	void SendSDraw(SDraw sd);

	void SendSOtherDraw(SOtherDraw sd);

	void HandChi(ccEvent *event);
	void SendSChi(SChi sd);

	void HandCPeng(ccEvent *event);
	void SendSPeng(SPeng sd);

	void HandCMingGang(ccEvent *event);
	void SendSMingGang(SMingGang sd);

	void HandCAnGang(ccEvent *event);
	void SendSAnGang(SAnGang sd);

	void SendSFa(SFa sd);

	void HandCHu(ccEvent *event);
	void SendSHu(SHu sd);

	void SendSGameover(SGameover sd);

	void SendSRoundOver(SRoundOver sd);

	void SendSDissolveTip(SDissolveTip sd);
private:
	static RoomLogicInfo *m_shareRoomLogicInfo;
	
};

#endif 
