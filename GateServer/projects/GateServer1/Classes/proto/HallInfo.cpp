#include "HallInfo.h"
#include "EventDispatcher.h"
#include "XXIconv.h"
#include "LoginInfo.h"
#include "YMSocketData.h"
#include "LibEvent.h"
#include "Common.h"

HallInfo *HallInfo::m_shareHallInfo=NULL;
HallInfo::HallInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	CRank sl1;
	pe->registerProto(sl1.cmd(), sl1.GetTypeName());
	pe->addListener(sl1.cmd(), this, Event_Handler(HallInfo::HandlerCRankHand));
	CShop sr2;
	pe->registerProto(sr2.cmd(), sr2.GetTypeName());
	pe->addListener(sr2.cmd(), this, Event_Handler(HallInfo::HandlerCShop));
	CMail sl3;
	pe->registerProto(sl3.cmd(), sl3.GetTypeName());
	pe->addListener(sl3.cmd(), this, Event_Handler(HallInfo::HandlerCMail));

	//EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCMailAward));

	CFriend sr4;
	pe->registerProto(sr4.cmd(), sr4.GetTypeName());
	CFindFriend sl5;
	pe->registerProto(sl5.cmd(), sl5.GetTypeName());
	CGiveFriend sr6;
	pe->registerProto(sr6.cmd(), sr6.GetTypeName());
	CAddFriend sl7;
	pe->registerProto(sl7.cmd(), sl7.GetTypeName());
	 CAddFriendList sr8;
	pe->registerProto(sr8.cmd(), sr8.GetTypeName());
	CActive sl9;
	pe->registerProto(sl9.cmd(), sl9.GetTypeName());
	CTask sl10;
	pe->registerProto(sl10.cmd(), sl10.GetTypeName());
	CReward sl11;
	pe->registerProto(sl11.cmd(), sl11.GetTypeName());
	CAgreeFriend sl12;
	pe->registerProto(sl12.cmd(), sl12.GetTypeName());
	CExchangeReward sl13;
	pe->registerProto(sl13.cmd(), sl13.GetTypeName());
	 CExchangeCode sl14;
	pe->registerProto(sl14.cmd(), sl14.GetTypeName());
	CExchangeRecord sl15;
	pe->registerProto(sl15.cmd(), sl15.GetTypeName());
	 CApplePay sl17;
	pe->registerProto(sl17.cmd(), sl17.GetTypeName());
	CWxpayOrder sl18;
	pe->registerProto(sl18.cmd(), sl18.GetTypeName());
	CWxpayQuery sl19;
	pe->registerProto(sl19.cmd(), sl19.GetTypeName());
	CFirstBuy sl20;
	pe->registerProto(sl20.cmd(), sl20.GetTypeName());
	CFeedBack sl21;
	pe->registerProto(sl21.cmd(), sl21.GetTypeName());
	CSign sl22;
	pe->registerProto(sl22.cmd(), sl22.GetTypeName());
	CSignList sl23;
	pe->registerProto(sl23.cmd(), sl23.GetTypeName());
	
	pe->addListener(sl19.cmd(), this, Event_Handler(HallInfo::HandlerCWxpayQuery));
}

HallInfo::~HallInfo(){
	
}

HallInfo* HallInfo::getIns(){
	if (!m_shareHallInfo){
		m_shareHallInfo = new HallInfo();
		m_shareHallInfo->init();
	}
	return m_shareHallInfo;
}

bool HallInfo::init()
{
	
    return true;
}


void HallInfo::SendSRank(YMSocketData sd, int fd){
	SRank cl1;
	cl1.set_type(1);
	
	LibEvent::getIns()->SendData(cl1.cmd(), &cl1,fd);

	
}

void HallInfo::HandlerCRankHand(ccEvent *event){
	CRank cl;
	cl.CopyFrom(*event->msg);
	int type = cl.type();
	int index = cl.index();

	//逻辑
	SRank cl1;
	char buff1[30];
	for (int i = 0; i < 10; i++){
		Rank rk;
		rk.set_lv(i * 10 + (i + 1));
		rk.set_number(10000 * 1 + 1000 * i);
		sprintf(buff1, "1%d%d0%02d", 1, index, i);
		rk.set_uid(buff1);
		sprintf(buff1, "qp1%d%d0%02d", 1, index, i);
		rk.set_uname(buff1);
		rk.set_type(1);
		Rank *rk1 = cl1.add_list();
		rk1->CopyFrom(rk);
	}
}

void HallInfo::SendSShop(YMSocketData sd, int fd){
	SShop cl;
	
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCShop(ccEvent *event){
	CShop cl;
	cl.CopyFrom(*event->msg);
	
}

void HallInfo::SendSMail(YMSocketData sd, int fd){
	CMail cl;
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

	
}

void HallInfo::HandlerCMail(ccEvent *event){
	CMail cl;
	cl.CopyFrom(*event->msg);
	
	SMail sl;
	char buff[100];
	for (int i = 0; i < 8; i++){
		Mail *ml = sl.add_list();
		ml->set_id(i + 1);
		sprintf(buff, XXIconv::GBK2UTF("邮件内容测试%d").c_str(), i + 1);
		ml->set_content(buff);
		sprintf(buff, XXIconv::GBK2UTF("邮件标题%d").c_str(), i + 1);
		ml->set_title(buff);
		ml->set_time(Common::getLocalTime().c_str());
		ml->set_type(1);
	}
}


void HallInfo::SendSMailAward(YMSocketData sd, int fd){
	SMailAward cl;
	
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCMailAward(ccEvent *event){
	CMailAward cl;
	cl.CopyFrom(*event->msg);
	
	
}

void HallInfo::SendSFriend(YMSocketData sd, int fd){
	SFriend cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFriend));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFriend(ccEvent *event){
	CFriend cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFriend));
	

	SFriend sf;
	for (int i = 0; i < 7; i++){
		Friend *fri = sf.add_list();
		fri->set_acttype(i % 3 + 1);
		fri->set_time(Common::getTime());
		char buff[100];
		DBUserInfo us;
		DBUserInfo *user = (DBUserInfo *)ccEvent::create_message(us.GetTypeName());
		sprintf(buff, "10000%d", i);
		user->set_username(buff);
		fri->set_allocated_userinfo(user);
	}
}


void HallInfo::SendSFindFriend(YMSocketData sd, int fd){
	SFindFriend cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFindFriend));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFindFriend(ccEvent *event){
	CFindFriend cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFindFriend));

	char buff[100];
	SFindFriend fris;
	for (int i = 0; i < 2; i++){
		Friend *fri = fris.add_list();
		fri->set_acttype(i % 3 + 1);
		fri->set_online(i % 2);
		fri->set_time(Common::getTime());
		DBUserInfo us;
		DBUserInfo *user = (DBUserInfo *)ccEvent::create_message(us.GetTypeName());
		sprintf(buff, "10000%d", i);
		user->set_username(buff);
		user->set_userid(buff);
		fri->set_allocated_userinfo(user);

	}
}


void HallInfo::SendSGiveFriend(YMSocketData sd, int fd){
	SGiveFriend cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCGiveFriend));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCGiveFriend(ccEvent *event){
	CGiveFriend cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCGiveFriend));
	
}


void HallInfo::SendSAddFriend(YMSocketData sd, int fd){
	SAddFriend cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAddFriend));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAddFriend(ccEvent *event){
	CAddFriend cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAddFriend));
	
}


void HallInfo::SendSAddFriendList(YMSocketData sd, int fd){
	SAddFriendList cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAddFriendList));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCAddFriendList(ccEvent *event){
	CAddFriendList cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAddFriendList));
	
	char buff[100];
	SAddFriendList fris;
	for (int i = 0; i < 5; i++){
		FriendNotice *fri = fris.add_list();
		fri->set_add(i % 2);
		sprintf(buff, "1000%02d", i);
		fri->set_uid(buff);
		Mail ml1;
		Mail *ml = (Mail *)ccEvent::create_message(ml1.GetTypeName());
		ml->set_id(i + 1);
		sprintf(buff, XXIconv::GBK2UTF("%s添加您为好友").c_str(), buff);
		ml->set_content(buff);
		sprintf(buff, XXIconv::GBK2UTF("邮件标题%d").c_str(), i + 1);
		ml->set_title(buff);
		ml->set_time(Common::getLocalTime().c_str());
		ml->set_type(1);
		fri->set_allocated_notice(ml);
	}
}


void HallInfo::SendSActive(YMSocketData sd, int fd){
	SActive cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCActive));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCActive(ccEvent *event){
	CActive cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCActive));
	
}

void HallInfo::SendSTask(YMSocketData sd, int fd){
	STask cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCTask));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCTask(ccEvent *event){
	STask cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCTask));
	
	char buff[50];
	STask st;
	for (int i = 0; i < 20; i++){
		Task *ts = st.add_list();
		sprintf(buff, XXIconv::GBK2UTF("登录奖励%d").c_str(), i % 4 + 1);
		ts->set_title(buff);
		ts->set_content(XXIconv::GBK2UTF("完成任务可获得大量金币"));
		ts->set_taskid(i + 1);
		int count = 3 * (i % 4) + 1;
		ts->set_count(count);
		int fcount = 2 * (i % 3) + 1;
		ts->set_fcount(fcount);
		if (count <= fcount){
			ts->set_finish(i % 2 + 1);
		}
		else{
			ts->set_finish(0);
		}
		ts->set_type(i / 4 + 1);
		Prop *prop = ts->add_award();
		prop->set_id(1);
		prop->set_name(XXIconv::GBK2UTF("金币"));
		prop->set_number((i + 1)*(i / 4 + 1) * 1500);
	}
}

void HallInfo::SendSReward(YMSocketData sd, int fd){
	SReward cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCReward));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCReward(ccEvent *event){
	CReward cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCReward));
	
}

void HallInfo::SendSAgreeFriend(YMSocketData sd, int fd){
	SAgreeFriend cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAgreeFriend));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAgreeFriend(ccEvent *event){
	CAgreeFriend cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCAgreeFriend));
	
}

void HallInfo::SendSExchangeReward(YMSocketData sd, int fd){
	SExchangeReward cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeReward));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCExchangeReward(ccEvent *event){
	CExchangeReward cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeReward));
	

	char buff[100];
	int gold = /*LoginInfo::getIns()->getMyDBUserInfo().gold()*/0;
	SExchangeReward se;
	for (int i = 0; i < 8; i++){
		ExAward *ea = se.add_list();
		ea->set_eid(i + 1);
		ea->set_pid(1);
		int number = 28000 * i;
		sprintf(buff, XXIconv::GBK2UTF("%d元红包").c_str(), i * 5 + 5);
		ea->set_title(buff);
		ea->set_can(gold >= number);
		Prop prop;
		prop.set_id(1);
		prop.set_name(XXIconv::GBK2UTF("金币"));
		prop.set_number(number);
		Prop *prop1 = (Prop *)ccEvent::create_message(prop.GetTypeName());
		prop1->CopyFrom(prop);

		ea->set_allocated_award(prop1);
	}
}

void HallInfo::SendSExchangeCode(YMSocketData sd, int fd){
	SExchangeCode cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeCode));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchangeCode(ccEvent *event){
	CExchangeCode cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeCode));
	
}

void HallInfo::SendSExchangeRecord(YMSocketData sd, int fd){
	SExchangeRecord cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeRecord));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCExchangeRecord(ccEvent *event){
	SExchangeRecord cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeRecord));
	
	char buff[100];
	int gold = /*LoginInfo::getIns()->getMyDBUserInfo().gold()*/0;
	SExchangeRecord se;
	for (int i = 0; i < 8; i++){
		ExRecord *ea = se.add_list();
		sprintf(buff, XXIconv::GBK2UTF("%d元红包").c_str(), i * 5 + 5);
		ea->set_title(buff);
		ea->set_id(i + 1);
		sprintf(buff, "201803201%04d", i);
		ea->set_orderid(buff);
		ea->set_status(i % 3);
		ea->set_time(Common::getLocalTime());
	}
}

void HallInfo::SendSExchange(YMSocketData sd, int fd){
	SExchange cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchange));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchange(ccEvent *event){
	CExchange cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchange));
	
}

void HallInfo::SendSApplePay(YMSocketData sd, int fd){
	SApplePay cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCApplePay));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCApplePay(ccEvent *event){
	CApplePay cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCApplePay));
	
}


void HallInfo::SendSWxpayOrder(YMSocketData sd, int fd){
	SWxpayOrder cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCWxpayOrder));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCWxpayOrder(ccEvent *event){
	CWxpayOrder cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCWxpayOrder));
	
}


void HallInfo::SendSWxpayQuery(YMSocketData sd, int fd){
	SWxpayQuery cl;
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCWxpayQuery(ccEvent *event){
	CWxpayQuery cl;
	cl.CopyFrom(*event->msg);
	
}


void HallInfo::SendSFirstBuy(YMSocketData sd, int fd){
	SFirstBuy cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFirstBuy));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCFirstBuy(ccEvent *event){
	CFirstBuy cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFirstBuy));
	
}


//反馈
void HallInfo::SendSFeedBack(YMSocketData sd, int fd){
	SFeedBack cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFeedBack));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFeedBack(ccEvent *event){
	CFeedBack cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCFeedBack));
}


//签到
void HallInfo::SendSSign(YMSocketData sd, int fd){
	SSign cl;

	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCSign));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCSign(ccEvent *event){
	SSign cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCSign));
	
}


void HallInfo::SendSSignList(YMSocketData sd, int fd){
	SSignList cl;
	EventDispatcher::getIns()->addListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCSignList));
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCSignList(ccEvent *event){
	CSignList cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCSignList));
	
	SSignList sl;
	sl.set_count(3);
	sl.set_sign(0);
	int dd[8] = { 3, 5, 7, 10, 14, 18, 22, 30 };
	for (int i = 0; i < 8; i++){
		SignAward *sa = sl.add_reward();
		sa->set_id(i + 1);
		sa->set_day(dd[i]);
		Prop p1;
		Prop *p = (Prop *)ccEvent::create_message(p1.GetTypeName());
		int pid = i % 2 + 1;
		p->set_id(pid);
		p->set_number(pid == 1 ? 500 * dd[i] : i / 2);
		sa->set_allocated_reward(p);
	}
}
