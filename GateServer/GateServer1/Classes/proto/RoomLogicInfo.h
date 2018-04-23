
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
	void SendSDice(SDice sd,int fd);
	void HandCDice(ccEvent *event);

	void SendSSelectZhuang(SSelectZhuang ssz, int fd);

	void SendSZhuangCard(SZhuangCard sd, int fd);

	void SendSCard(SCard sd, int fd);

	void HandCDiscard(ccEvent *event);
	void SendSDiscard(SDiscard sd, int fd);

	void SendSDraw(SDraw sd, int fd);

	void SendSOtherDraw(SOtherDraw sd, int fd);

	void HandChi(ccEvent *event);
	void SendSChi(SChi sd, int fd);

	void HandCPeng(ccEvent *event);
	void SendSPeng(SPeng sd, int fd);

	void HandCMingGang(ccEvent *event);
	void SendSMingGang(SMingGang sd, int fd);

	void HandCAnGang(ccEvent *event);
	void SendSAnGang(SAnGang sd, int fd);

	void SendSFa(SFa sd, int fd);

	void HandCHu(ccEvent *event);
	void SendSHu(SHu sd, int fd);

	void SendSGameover(SGameover sd, int fd);

	void SendSRoundOver(SRoundOver sd, int fd);

	void SendSDissolveTip(SDissolveTip sd, int fd);
private:
	static RoomLogicInfo *m_shareRoomLogicInfo;
	
};

#endif 
