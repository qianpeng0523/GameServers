#include "RoomLogicInfo.h"
#include "ClientSocket.h"
#include "EventDispatcher.h"
#include "XXIconv.h"
#include "GRoom.h"
#include "RoomControl.h"

static RoomControl *g_pRoomControl = RoomControl::getIns();
RoomLogicInfo *RoomLogicInfo::m_shareRoomLogicInfo=NULL;
RoomLogicInfo::RoomLogicInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CDice sl;
	pe->registerProto(sl.cmd(), sl.GetTypeName());
	pe->addListener(sl.cmd(), this, Event_Handler(RoomLogicInfo::HandCDice));

	CDiscard sl4;
	pe->registerProto(sl4.cmd(), sl4.GetTypeName());
	pe->addListener(sl4.cmd(), this, Event_Handler(RoomLogicInfo::HandCDiscard));

	CChi sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName());
	pe->addListener(sl7.cmd(), this, Event_Handler(RoomLogicInfo::HandChi));

	CPeng sl8;
	pe->registerProto(sl8.cmd(), sl8.GetTypeName());
	pe->addListener(sl8.cmd(), this, Event_Handler(RoomLogicInfo::HandCPeng));

	CMingGang sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName());
	pe->addListener(sl9.cmd(), this, Event_Handler(RoomLogicInfo::HandCMingGang));

	CAnGang sl10;
	pe->registerProto(sl10.cmd(), sl10.GetTypeName());
	pe->addListener(sl10.cmd(), this, Event_Handler(RoomLogicInfo::HandCAnGang));

	SHu sl12;
	pe->registerProto(sl12.cmd(), sl12.GetTypeName());
	pe->addListener(sl12.cmd(), this, Event_Handler(RoomLogicInfo::HandCHu));

}

RoomLogicInfo::~RoomLogicInfo(){
	
}

RoomLogicInfo* RoomLogicInfo::getIns(){
	if (!m_shareRoomLogicInfo){
		m_shareRoomLogicInfo = new RoomLogicInfo();
		m_shareRoomLogicInfo->init();
	}
	return m_shareRoomLogicInfo;
}

bool RoomLogicInfo::init()
{
	
    return true;
}

void RoomLogicInfo::SendSDice(SDice sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(),&sd);
}

void RoomLogicInfo::HandCDice(ccEvent *event){
	CDice cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	if (gr){
		gr->SendDice(uid);
	}
}

void RoomLogicInfo::SendSSelectZhuang(SSelectZhuang ssz){
	ClientSocket::getIns()->sendMsg(ssz.cmd(),&ssz);
}

void RoomLogicInfo::SendSZhuangCard(SZhuangCard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSCard(SCard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCDiscard(ccEvent *event){
	CDiscard cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	int card = cl.card();
	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	if (gr){
		int pos = gr->getPosition(uid);
		gr->SendDiscard(pos,card);
	}
}

void RoomLogicInfo::SendSDiscard(SDiscard sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSDraw(SDraw sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSOtherDraw(SOtherDraw sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandChi(ccEvent *event){
	CChi cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string cards = cl.card();
	int sz = cards.size();
	int cds[3] = { 0 };
	for (int i = 0; i < sz;i++){
		cds[i] = cards[i];
	}
	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	if (gr){
		int pos = gr->getPosition(uid);
		gr->SendChi(pos, cds);
	}
}

void RoomLogicInfo::SendSChi(SChi sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCPeng(ccEvent *event){
	CPeng cl;
	cl.CopyFrom(*event->msg);
	
	string uid = cl.uid();
	int card = cl.card();
	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	if (gr){
		int pos = gr->getPosition(uid);
		gr->SendPeng(pos, card);
	}
}

void RoomLogicInfo::SendSPeng(SPeng sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCMingGang(ccEvent *event){
	CMingGang cl;
	cl.CopyFrom(*event->msg);
	int card = cl.card();
	int type = cl.type();
	string uid = cl.uid();

	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	int pos = 0;
	if (gr){
		pos = gr->getPosition(uid);
		if (type == 3){
			gr->SendMingGang(pos, card);
			return;
		}
		else if (type == 5){
			gr->SendPeng(pos, card);
			return;
		}
	}
	SMingGang smg;
	smg.set_err(1);
	SendSMingGang(smg);
}

void RoomLogicInfo::SendSMingGang(SMingGang sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCAnGang(ccEvent *event){
	CAnGang cl;
	cl.CopyFrom(*event->msg);
	
	int card = cl.card();
	int type = cl.type();
	string uid = cl.uid();

	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	int pos = 0;
	if (gr){
		pos = gr->getPosition(uid);
		if (type == 4){
			gr->SendAnGang(pos, card);
			return;
		}
		else if (type == 6){
			gr->SendPeng(pos, card);
			return;
		}
	}
	SAnGang smg;
	smg.set_err(1);
	SendSAnGang(smg);
}

void RoomLogicInfo::SendSAnGang(SAnGang sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSFa(SFa sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::HandCHu(ccEvent *event){
	CHu cl;
	cl.CopyFrom(*event->msg);
	
	string uid = cl.uid();
	GRoom *gr = g_pRoomControl->getGRoom_(uid);
	int pos = 0;
	if (gr){
		pos = gr->getPosition(uid);
		gr->SendHu(pos);
		return;
	}
	SHu smg;
	smg.set_err(1);
	SendSHu(smg);
}

void RoomLogicInfo::SendSHu(SHu sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSGameover(SGameover sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSRoundOver(SRoundOver sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}

void RoomLogicInfo::SendSDissolveTip(SDissolveTip sd){
	ClientSocket::getIns()->sendMsg(sd.cmd(), &sd);
}