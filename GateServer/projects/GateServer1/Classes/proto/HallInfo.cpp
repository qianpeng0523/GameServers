#include "HallInfo.h"
#include "XXIconv.h"
#include "LoginInfo.h"
#include "YMSocketData.h"
#include "LibEvent.h"
#include "Common.h"

HallInfo *HallInfo::m_shareHallInfo=NULL;
HallInfo::HallInfo()
{
	CRank sl1;
	regist(sl1.cmd(), sl1.GetTypeName(), Event_Handler(HallInfo::HandlerCRankHand));
	CShop sr2;
	regist(sr2.cmd(), sr2.GetTypeName(), Event_Handler(HallInfo::HandlerCShop));
	CMail sl3;
	regist(sl3.cmd(), sl3.GetTypeName(), Event_Handler(HallInfo::HandlerCMail));
	CMailAward s24;
	regist(s24.cmd(), s24.GetTypeName(), Event_Handler(HallInfo::HandlerCMailAward));
	CFriend sr4;
	regist(sr4.cmd(), sr4.GetTypeName(), Event_Handler(HallInfo::HandlerCFriend));
	CFindFriend sl5;
	regist(sl5.cmd(), sl5.GetTypeName(), Event_Handler(HallInfo::HandlerCFindFriend));
	CGiveFriend sr6;
	regist(sr6.cmd(), sr6.GetTypeName(), Event_Handler(HallInfo::HandlerCGiveFriend));
	CAddFriend sl7;
	regist(sl7.cmd(), sl7.GetTypeName(), Event_Handler(HallInfo::HandlerCAddFriend));
	CAddFriendList sr8;
	regist(sr8.cmd(), sr8.GetTypeName(), Event_Handler(HallInfo::HandlerCAddFriendList));
	CActive sl9;
	regist(sl9.cmd(), sl9.GetTypeName(), Event_Handler(HallInfo::HandlerCActive));
	CTask sl10;
	regist(sl10.cmd(), sl10.GetTypeName(), Event_Handler(HallInfo::HandlerCTask));
	CReward sl11;
	regist(sl11.cmd(), sl11.GetTypeName(), Event_Handler(HallInfo::HandlerCReward));
	CAgreeFriend sl12;
	regist(sl12.cmd(), sl12.GetTypeName(), Event_Handler(HallInfo::HandlerCAgreeFriend));
	CExchangeReward sl13;
	regist(sl13.cmd(), sl13.GetTypeName(), Event_Handler(HallInfo::HandlerCExchangeReward));
	CExchangeCode sl14;
	regist(sl14.cmd(), sl14.GetTypeName(), Event_Handler(HallInfo::HandlerCExchangeCode));
	CExchangeRecord sl15;
	regist(sl15.cmd(), sl15.GetTypeName(), Event_Handler(HallInfo::HandlerCExchangeRecord));
	CApplePay sl17;
	regist(sl17.cmd(), sl17.GetTypeName(), Event_Handler(HallInfo::HandlerCApplePay));
	CWxpayOrder sl18;
	regist(sl18.cmd(), sl18.GetTypeName(), Event_Handler(HallInfo::HandlerCWxpayOrder));
	CWxpayQuery sl19;
	regist(sl19.cmd(), sl19.GetTypeName(), Event_Handler(HallInfo::HandlerCWxpayQuery));
	CFirstBuy sl20;
	regist(sl20.cmd(), sl20.GetTypeName(), Event_Handler(HallInfo::HandlerCFirstBuy));
	CFeedBack sl21;
	regist(sl21.cmd(), sl21.GetTypeName(), Event_Handler(HallInfo::HandlerCFeedBack));
	CSign sl22;
	regist(sl22.cmd(), sl22.GetTypeName(), Event_Handler(HallInfo::HandlerCSign));
	CSignList sl23;
	regist(sl23.cmd(), sl23.GetTypeName(), Event_Handler(HallInfo::HandlerCSignList));
	CExchange sl24;
	regist(sl24.cmd(), sl24.GetTypeName(), Event_Handler(HallInfo::HandlerCExchange));
}

HallInfo::~HallInfo(){
	
}

void HallInfo::regist(int cmd, string name, EventHandler handler){
	EventDispatcher *pe = EventDispatcher::getIns();
	pe->registerProto(cmd,name);
	pe->addListener(cmd, this, handler);
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
	SRank cl;
	SRank *cl1 = (SRank *)LoginInfo::getIns()->getDBDataFromSocketDataVo(cl.GetTypeName(),sd);
	
	LibEvent::getIns()->SendData(cl1->cmd(), cl1,fd);

	
}

void HallInfo::HandlerCRankHand(ccEvent *event){
	CRank cl;
	cl.CopyFrom(*event->msg);
	int type = cl.type();
	int index = cl.index();

	//逻辑
	SRank cl1;
	cl1.set_type(type);
	char buff1[30];
	for (int i = 0; i < 10; i++){
		Rank rk;
		rk.set_lv(i + 1);
		rk.set_number(10000 * 1 + 1000 * i);
		sprintf(buff1, "1%d%d0%02d", 1, index, i);
		rk.set_uid(buff1);
		sprintf(buff1, "qp1%d%d0%02d", 1, index, i);
		rk.set_uname(buff1);
		rk.set_type(1);
		Rank *rk1 = cl1.add_list();
		rk1->CopyFrom(rk);
	}
	YMSocketData sd;
	LoginInfo::getIns()->setDBDataToSocketDataVo(&cl1, sd);
	SendSRank(sd,event->m_fd);
}

void HallInfo::SendSShop(SShop cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCShop(ccEvent *event){
	CShop cl;
	cl.CopyFrom(*event->msg);
	int type = cl.type();

	//逻辑
	SShop sl;
	for (int i = 0; i < 10; i++){
		ShopItem *rk = sl.add_list();
		rk->set_type(i + 1);
		rk->set_number(6 * (i + 1));
		rk->set_givenum(1000 * (i + 1));
		rk->set_hot(i % 2 == 1);
		Prop ppp;
		Prop *pp = (Prop *)ccEvent::create_message(ppp.GetTypeName());
		pp->set_id(i + 1);
		int number = rk->number();
		pp->set_number((i + 1) % 2 == 1 ? 10000 * number : 2 * number);
		rk->set_allocated_prop(pp);
	}
	SendSShop(sl, event->m_fd);
}

void HallInfo::SendSMail(SMail sl, int fd){
	LibEvent::getIns()->SendData(sl.cmd(), &sl,fd);
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
	SendSMail(sl,event->m_fd);
}


void HallInfo::SendSMailAward(SMailAward cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCMailAward(ccEvent *event){
	CMailAward cl;
	cl.CopyFrom(*event->msg);
	int id = cl.id();
	//逻辑
	SMailAward sma;
	sma.set_err(0);
	sma.set_id(id);
	SendSMailAward(sma, event->m_fd);
}

void HallInfo::SendSFriend(SFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCFriend(ccEvent *event){
	CFriend cl;
	cl.CopyFrom(*event->msg);
	

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
	SendSFriend(sf,event->m_fd);
}


void HallInfo::SendSFindFriend(SFindFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFindFriend(ccEvent *event){
	CFindFriend cl;
	cl.CopyFrom(*event->msg);
	
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
	SendSFindFriend(fris, event->m_fd);
}


void HallInfo::SendSGiveFriend(SGiveFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCGiveFriend(ccEvent *event){
	CGiveFriend cl;
	cl.CopyFrom(*event->msg);
	
	SGiveFriend sl;
	SendSGiveFriend(sl, event->m_fd);
}


void HallInfo::SendSAddFriend(SAddFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAddFriend(ccEvent *event){
	CAddFriend cl;
	cl.CopyFrom(*event->msg);
	
	SAddFriend sl;
	SendSAddFriend(sl, event->m_fd);
}


void HallInfo::SendSAddFriendList(SAddFriendList cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAddFriendList(ccEvent *event){
	CAddFriendList cl;
	cl.CopyFrom(*event->msg);
	
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
	SendSAddFriendList(fris,event->m_fd);
}


void HallInfo::SendSActive(SActive cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCActive(ccEvent *event){
	CActive cl;
	cl.CopyFrom(*event->msg);
	
	SActive sl;
	SendSActive(sl, event->m_fd);
}

void HallInfo::SendSTask(STask cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCTask(ccEvent *event){
	CTask cl;
	cl.CopyFrom(*event->msg);
	
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
	SendSTask(st, event->m_fd);
}

void HallInfo::SendSReward(SReward cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCReward(ccEvent *event){
	CReward cl;
	cl.CopyFrom(*event->msg);

	SReward sl;
	SendSReward(sl,event->m_fd);
}

void HallInfo::SendSAgreeFriend(SAgreeFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAgreeFriend(ccEvent *event){
	CAgreeFriend cl;
	cl.CopyFrom(*event->msg);
	
	SAgreeFriend sl;
	SendSAgreeFriend(sl, event->m_fd);
}

void HallInfo::SendSExchangeReward(SExchangeReward cl, int fd){
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
	SendSExchangeReward(se, event->m_fd);
}

void HallInfo::SendSExchangeCode(SExchangeCode cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchangeCode(ccEvent *event){
	CExchangeCode cl;
	cl.CopyFrom(*event->msg);
	
	SExchangeCode sl;
	SendSExchangeCode(sl, event->m_fd);
}

void HallInfo::SendSExchangeRecord(SExchangeRecord cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCExchangeRecord(ccEvent *event){
	CExchangeRecord cl;
	cl.CopyFrom(*event->msg);
	
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
	SendSExchangeRecord(se, event->m_fd);
}

void HallInfo::SendSExchange(SExchange cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchange(ccEvent *event){
	CExchange cl;
	cl.CopyFrom(*event->msg);
	
	SExchange sl;
	SendSExchange(sl, event->m_fd);
}

void HallInfo::SendSApplePay(SApplePay cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCApplePay(ccEvent *event){
	CApplePay cl;
	cl.CopyFrom(*event->msg);
	
	SApplePay sl;
	SendSApplePay(sl, event->m_fd);
}


void HallInfo::SendSWxpayOrder(SWxpayOrder cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCWxpayOrder(ccEvent *event){
	CWxpayOrder cl;
	cl.CopyFrom(*event->msg);
	
	SWxpayOrder sl;
	SendSWxpayOrder(sl, event->m_fd);
}


void HallInfo::SendSWxpayQuery(SWxpayQuery cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCWxpayQuery(ccEvent *event){
	CWxpayQuery cl;
	cl.CopyFrom(*event->msg);
	SWxpayQuery sl;
	SendSWxpayQuery(sl, event->m_fd);
}


void HallInfo::SendSFirstBuy(SFirstBuy cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCFirstBuy(ccEvent *event){
	CFirstBuy cl;
	cl.CopyFrom(*event->msg);
	
	SFirstBuy sl;
	SendSFirstBuy(sl, event->m_fd);
}


//反馈
void HallInfo::SendSFeedBack(SFeedBack cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFeedBack(ccEvent *event){
	CFeedBack cl;
	cl.CopyFrom(*event->msg);
	

	SFeedBack sl;
	SendSFeedBack(sl, event->m_fd);
}


//签到
void HallInfo::SendSSign(SSign cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCSign(ccEvent *event){
	CSign cl;
	cl.CopyFrom(*event->msg);
	
	SSign sl;
	SendSSign(sl, event->m_fd);
	
}


void HallInfo::SendSSignList(SSignList cl, int fd){
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
	SendSSignList(sl, event->m_fd);
}
