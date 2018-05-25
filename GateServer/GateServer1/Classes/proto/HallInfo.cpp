#include "HallInfo.h"
#include "XXIconv.h"
#include "LoginInfo.h"
#include "YMSocketData.h"
#include "LibEvent.h"
#include "Common.h"
#include "HttpPay.h"
#include "XmlConfig.h"
#include "HttpAliPay.h"
#include "ConfigInfo.h"
#include "LoginInfo.h"

HallInfo *HallInfo::m_shareHallInfo=NULL;
HallInfo::HallInfo()
{
	m_pRedisGet = RedisGet::getIns();
	m_pRedisPut = RedisPut::getIns();

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

	CAliPayOrder sl25;
	regist(sl25.cmd(), sl25.GetTypeName(), Event_Handler(HallInfo::HandlerCAliPayOrder));

	CAliPayResult sl26;
	regist(sl26.cmd(), sl26.GetTypeName(), Event_Handler(HallInfo::HandlerCAliPayResult));

	CFirsyBuyData sl27;
	regist(sl27.cmd(), sl27.GetTypeName(), Event_Handler(HallInfo::HandCFirsyBuyData));

	m_lastday = Common::getLocalTimeDay();
	StatTimer::getIns()->scheduleSelector(this, schedule_selector(HallInfo::update), 1.0);
}

HallInfo::~HallInfo(){
	StatTimer::getIns()->unscheduleSelector(this, schedule_selector(HallInfo::update));
}

void HallInfo::regist(int cmd, string name, EventHandler handler){
	EventDispatcher *pe = EventDispatcher::getIns();
	pe->registerProto(cmd, name, GAME_TYPE);
	pe->addListener(cmd, this, handler, GAME_TYPE);
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


void HallInfo::SendSRank(SRank cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCRankHand(ccEvent *event){
	CRank cl;
	cl.CopyFrom(*event->msg);
	int type = cl.type();
	int index = cl.index();

	//逻辑
	SRank sl;
	sl.set_type(type);
	vector<Rank *> vec = m_pRedisGet->getRank(type);
	int ii = index * 20;
	int im = (index+1)*20;
	int sz = vec.size();
	int endindex = (im < sz) ? im : sz;
	for (int i = ii; i <endindex; i++){
		Rank *rk = vec.at(i);
		Rank *k = sl.add_list();

		UserBase *user = rk->mutable_info();
		string uid = rk->uid();
		UserBase *ub = m_pRedisGet->getUserBase(uid);
		if (ub){
			user->CopyFrom(*ub);
		}
		
		k->CopyFrom(*rk);
	}
	SendSRank(sl,event->m_fd);
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
	sl.set_type(type);
	auto vec = m_pRedisGet->getShop();
	auto itr = vec.begin();
	for (itr; itr!=vec.end(); itr++){
		ShopItem *p = itr->second;
		int pid = p->prop().prop().id();
		if (pid == type){
			ShopItem *pp = sl.add_list();
			pp->CopyFrom(*p);
		}
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
	sl.set_cmd(sl.cmd());
	string uid = LibEvent::getIns()->getUID(event->m_fd);
	map<int,Mail*> vec = m_pRedisGet->getMail(uid);
	map<int, Mail*>::iterator itr = vec.begin();
	for (itr; itr != vec.end();itr++){
		Mail *m = itr->second;
		Mail *m1= sl.add_list();
		m1->CopyFrom(*m);
	}
	SendSMail(sl,event->m_fd);
}


void HallInfo::SendSMailAward(SMailAward cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCMailAward(ccEvent *event){
	CMailAward cl;
	cl.CopyFrom(*event->msg);
	int mid = cl.id();
	//逻辑
	SMailAward sma;
	string uid = LibEvent::getIns()->getUID(event->m_fd);
	Mail *mail= m_pRedisGet->getMail(uid,mid);
	vector<Reward>rds;
	if (mail->eid() > 0&&mail->status()==1){
		sma.set_err(0);
		int status = mail->status();
		//status 0表示无奖励 1表示有奖励未领取 2表示有奖励已经领取
		if (status == 1){
			m_pRedisPut->PopMail(uid,mail);
			//给玩家奖励//推送金币那些协议
			int sz = mail->rewardlist_size();
			for (int i = 0; i < sz;i++){
				Reward rd = mail->rewardlist(i);
				rds.push_back(rd);
			}
		}
		else{
			sma.set_err(1);
		}
	}
	else{
		sma.set_err(1);
	}
	
	sma.set_id(mid);
	SendSMailAward(sma, event->m_fd);
	if (sma.err() == 0){
		for (int i = 0; i < rds.size(); i++){
			Reward rd = rds.at(i);
			HttpPay::getIns()->NoticePushCurrency(rd, uid);
		}

		auto vec = m_pRedisGet->getMail(uid);
		SConfig *sc = m_pRedisGet->getSConfig(uid);
		if (vec.empty() && sc&&sc->mail()){
			sc->set_mail(false);
			m_pRedisPut->setConfig(uid, POINT_MAIL, false);
			ConfigInfo::getIns()->SendSConfig(*sc, event->m_fd);
		}
	}
}

void HallInfo::resetUserData(Reward rd, string uid){
	int number = rd.number();
	Prop p = rd.prop();
	int pid = p.id();
	if (pid == 1){
		//金币
		m_pRedisPut->addUserBase(uid,"gold",number);
	}
	else if (pid == 3){
		//房卡
		m_pRedisPut->addUserBase(uid, "card", number);
	}
	else if (pid == 2){
		//钻石
		m_pRedisPut->addUserBase(uid, "diamond", number);
	}
}

void HallInfo::SendSFriend(SFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCFriend(ccEvent *event){
	CFriend cl;
	cl.CopyFrom(*event->msg);
	

	SFriend sf;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		auto users = m_pRedisGet->getFriend(uid);
		auto uitr = users.begin();
		for (uitr; uitr != users.end();uitr++){
			Friend *user = uitr->second;
			Friend *fri = sf.add_list();
			fri->CopyFrom(*user);
		}
		SendSFriend(sf, event->m_fd);
	}
}


void HallInfo::SendSFindFriend(SFindFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFindFriend(ccEvent *event){
	CFindFriend cl;
	cl.CopyFrom(*event->msg);
	SFindFriend fris;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		int type = cl.type();
		char buff[100];
		
		fris.set_type(type);
		fris.set_err(0);
		if (type == 1){
			UserBase *fir = m_pRedisGet->getUserBase(uid);
			if (fir){
				Friend *fri = fris.add_list();
				fri->set_acttype(1);
				fri->set_online(LibEvent::getIns()->isHave(uid));
				fri->set_time(Common::getTime() - RedisGet::getIns()->getUserLoginTime(uid));
				UserBase *user = fri->mutable_info();
				user->CopyFrom(*fir);
			}
		}
		else if (type == 2){
			LoginInfo *pLoginInfo = LoginInfo::getIns();
			auto vec = pLoginInfo->getOnLineUser(true);
			if (vec.find(uid) != vec.end()){
				vec.erase(vec.find(uid));
			}
			int num1 = vec.size();
			auto vec1 = pLoginInfo->getOnLineUser(false);
			int num2 = vec1.size();
			int offnum = 4 - num1;
			map<int, int> m1 = getRandNum(4, num1);
			map<int, int> m2 = getRandNum(num2, offnum);

			auto itr = m1.begin();

			for (itr; itr != m1.end(); itr++){
				int index = itr->first;
				auto itr1 = vec.begin();
				int i = 0;
				while (i < index&&itr1 != vec.end()){
					itr1++;
					i++;
				}
				string puid = itr1->first;
				UserBase *p = itr1->second;

				Friend *fri = fris.add_list();
				fri->set_acttype(1);
				fri->set_online(true);
				fri->set_time(Common::getTime() - RedisGet::getIns()->getUserLoginTime(uid));
				UserBase *user = fri->mutable_info();
				user->CopyFrom(*p);
			}
			itr = m2.begin();
			for (itr; itr != m2.end(); itr++){
				int index = itr->first;
				auto itr1 = vec1.begin();
				int i = 0;
				while (i < index&&itr1 != vec1.end()){
					itr1++;
					i++;
				}
				string puid = itr1->first;
				UserBase *p = itr1->second;

				Friend *fri = fris.add_list();
				fri->set_acttype(1);
				fri->set_online(false);
				fri->set_time(Common::getTime() - RedisGet::getIns()->getUserLoginTime(uid));
				UserBase *user = fri->mutable_info();
				user->CopyFrom(*p);
			}
		}
	}
	else{
		fris.set_err(1);
	}
	SendSFindFriend(fris, event->m_fd);
}

map<int, int> HallInfo::getRandNum(int maxnum, int num){
	map<int, int> maps;
	if (maxnum <= num){
		for (int i = 0; i < maxnum; i++){
			maps.insert(make_pair(i, i));
		}
	}
	else{
		srand(time(NULL));
		for (int i = 0; i <  num; i++){
			int rd = rand() % num;
			while (maps.find(rd) != maps.end()){
				rd = rand() % num;
			}
			maps.insert(make_pair(rd, rd));
		}
	}
	return maps;
}

void HallInfo::SendSGiveFriend(SGiveFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCGiveFriend(ccEvent *event){
	CGiveFriend cl;
	cl.CopyFrom(*event->msg);
	string fuid = cl.uid();
	SGiveFriend sl;
	sl.set_uid(fuid);
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		Friend *fri = m_pRedisGet->getFriend(uid,fuid);
		if (fri){
			bool give = fri->give();
			if (give){
				sl.set_err(1);
			}
			else{
				Reward *rd = m_pRedisGet->getReward(69);
				UserBase *user = m_pRedisGet->getUserBase(fuid);
				string uname = user->username();
				
				Mail ml1;
				Mail *ml = (Mail *)ccEvent::create_message(ml1.GetTypeName());
				Reward *rew = ml->add_rewardlist();
				rew->CopyFrom(*rd);

				int eid = m_pRedisGet->getMailID();
				ml->set_eid(eid);
				char buff[300];
				sprintf(buff, "%s%s%s%d%s", XXIconv::GBK2UTF("好友【").c_str(), uname.c_str(), XXIconv::GBK2UTF("】赠送给您").c_str(), rew->number(), rew->prop().name().c_str());
				ml->set_content(buff);
				ml->set_status(1);
				ml->set_time(Common::getLocalTime());
				ml->set_title(XXIconv::GBK2UTF("好友【")+uname + XXIconv::GBK2UTF("】赠送"));

				m_pRedisPut->setMailID(eid);
				m_pRedisPut->PushMail(uid, ml);
				m_pRedisPut->setConfig(uid, POINT_MAIL, true);
				
				ClientData *data1 = LibEvent::getIns()->getClientData(fuid);
				if (data1){
					SConfig *sc = m_pRedisGet->getSConfig(uid);
					if (sc&&!sc->mail()){
						sc->set_mail(true);
						ConfigInfo::getIns()->SendSConfig(*sc, data1->_fd);
					}
				}
			}
		}
		
	}
	SendSGiveFriend(sl, event->m_fd);
}


void HallInfo::SendSAddFriend(SAddFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAddFriend(ccEvent *event){
	CAddFriend cl;
	cl.CopyFrom(*event->msg);
	string puid = cl.uid();
	SAddFriend sl;
	sl.set_uid(puid);
	LibEvent *pLibEvent = LibEvent::getIns();
	ClientData *data = pLibEvent->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		FriendNotice *p = m_pRedisGet->getFriendNotice(uid,puid);
		Friend *fri = m_pRedisGet->getFriend(uid,puid);
		if (!fri){
			if (p){
				sl.set_err(2);
			}
			else{
				m_pRedisPut->setConfig(uid, POINT_HAOYOU, true);
				FriendNotice temp;
				string time = Common::getLocalTime();
				FriendNotice *sp = (FriendNotice *)ccEvent::create_message(temp.GetTypeName());
				sp->set_nid(m_pRedisGet->getFriendNoticeID());
				sp->set_uid(puid);
				sp->set_time(time);
				sp->set_status(0);
				sp->set_content(XXIconv::GBK2UTF("您请求") + puid + XXIconv::GBK2UTF("添加为好友"));
				m_pRedisPut->PushFriendNotice(uid, sp);
				
				m_pRedisPut->setConfig(puid, POINT_HAOYOU, true);
				FriendNotice *sp1 = (FriendNotice *)ccEvent::create_message(temp.GetTypeName());
				sp1->set_nid(m_pRedisGet->getFriendNoticeID());
				sp1->set_uid(uid);
				sp1->set_time(time);
				sp1->set_status(1);
				sp1->set_content(uid+XXIconv::GBK2UTF("请求添加您为好友"));
				m_pRedisPut->PushFriendNotice(puid, sp1);
				
				ClientData *ddata = pLibEvent->getClientData(puid);
				if (ddata){
					SendSAddFriend(sl, ddata->_fd);
				}
			}
		}
		else{
			sl.set_err(3);
		}
	}
	else{
		sl.set_err(1);
	}
	SendSAddFriend(sl, event->m_fd);
}


void HallInfo::SendSAddFriendList(SAddFriendList cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCAddFriendList(ccEvent *event){
	CAddFriendList cl;
	cl.CopyFrom(*event->msg);
	
	SAddFriendList fris;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		auto vec = m_pRedisGet->getFriendNotice(uid);
		auto itr = vec.begin();
		for (itr; itr != vec.end();itr++){
			FriendNotice *p = itr->second;
			FriendNotice *p1 = fris.add_list();
			p1->CopyFrom(*p);
		}
	}
	else{
		fris.set_err(1);
	}
	SendSAddFriendList(fris,event->m_fd);
}


void HallInfo::SendSAgreeFriend(SAgreeFriend cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void HallInfo::HandlerCAgreeFriend(ccEvent *event){
	CAgreeFriend cl;
	cl.CopyFrom(*event->msg);
	int nid = cl.nid();
	bool agree = cl.agree();

	SAgreeFriend sl;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		FriendNotice *fn = m_pRedisGet->getFriendNotice(uid, nid);
		if (fn){
			string puid = fn->uid();
			//接受方
			sl.set_userid(puid);
			sl.set_agree(agree);
			sl.set_nid(nid);
			fn->set_status(agree ? 2 : 3);
			
			Friend ff;
			m_pRedisPut->PushFriendNotice(uid, fn);
			Friend *fri = m_pRedisGet->getFriend(uid,puid);
			if (!fri){
				fri = (Friend *)ccEvent::create_message(ff.GetTypeName());
				UserBase *ub = fri->mutable_info();
				UserBase *ub1 = m_pRedisGet->getUserBase(puid);
				if (ub1){
					ub->CopyFrom(*ub1);
				}
				fri->set_give(false);
				m_pRedisPut->PushFriend(uid, fri);

				//申请方
				SAgreeFriend sl1;
				FriendNotice *fn1 = m_pRedisGet->getFriendNotice(puid, uid);
				if (fn1){
					int pnid = fn1->nid();
					sl1.set_userid(uid);
					sl1.set_agree(agree);
					sl1.set_nid(pnid);
					fn1->set_status(agree ? 2 : 3);
					m_pRedisPut->PushFriendNotice(puid, fn);
					Friend *fri1 = m_pRedisGet->getFriend(puid, uid);
					if (!fri1){
						fri1 = (Friend *)ccEvent::create_message(ff.GetTypeName());
						UserBase *ubb = fri1->mutable_info();
						UserBase *ubb1 = m_pRedisGet->getUserBase(uid);
						if (ubb1){
							ubb->CopyFrom(*ubb1);
						}
						fri1->set_give(false);

						m_pRedisPut->PushFriend(puid, fri1);
						ClientData *data1 = LibEvent::getIns()->getClientData(puid);
						if (data1){
							SendSAgreeFriend(sl1, data1->_fd);
						}
					}
					
				}
			}
			else{
				sl.set_err(1);
			}
		}
		else{
			sl.set_err(1);
		}
	}
	
	SendSAgreeFriend(sl, event->m_fd);
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
		Status *st = ts->mutable_status();

		int count = 3 * (i % 4) + 1;
		st->set_count(count);
		int fcount = 2 * (i % 3) + 1;
		st->set_fcount(fcount);
		if (count <= fcount){
			st->set_finished(i % 2 + 1);
		}
		else{
			st->set_finished(0);
		}
		ts->set_type(i / 4 + 1);
		Reward *reward = ts->add_rewardlist();
		reward->set_rid(1);
		Prop *prop = reward->mutable_prop();
		prop->set_id(1);
		prop->set_name("gold");
		reward->set_number((i + 1)*(i / 4 + 1) * 1500);
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

void HallInfo::SendSExchangeReward(SExchangeReward cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchangeReward(ccEvent *event){
	CExchangeReward cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCExchangeReward),GAME_TYPE);
	
	SExchangeReward se;
	auto vec = m_pRedisGet->getExAward();
	auto itr = vec.begin();
	for (itr; itr != vec.end();itr++){
		ExAward *ex = itr->second;
		ExAward *ex1 = se.add_list();
		ex1->CopyFrom(*ex);
	}
	se.set_err(0);
	SendSExchangeReward(se, event->m_fd);
}

void HallInfo::SendSExchangeCode(SExchangeCode cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchangeCode(ccEvent *event){
	CExchangeCode cl;
	cl.CopyFrom(*event->msg);
	
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		SExchangeCode sl;
		string code = cl.excode();
		PExchangeCode *p = m_pRedisGet->getPExchangeCode(code);
		if (p){
			sl.set_success(true);
			PExchangeCode *p = m_pRedisGet->getPExchangeCode(code);
			if (p){
				auto vec = p->_rewardid;
				for (int i = 0; i < vec.size(); i++){
					int rid = vec.at(i);
					Reward *rd = m_pRedisGet->getReward(rid);
					Reward *rd1 = sl.add_rd();
					rd1->CopyFrom(*rd);
					HttpPay::getIns()->NoticePushCurrency(*rd, uid);
				}

			}
			m_pRedisPut->erasePExchangeCode(p);
			string code = m_pRedisGet->getExchangeCode();
			char buff[10];
			sprintf(buff, "%d", atoi(code.c_str()) + 1);
			m_pRedisPut->setExchangeCode(buff);

			ExRecord exr;
			int eid = m_pRedisGet->getExchangeRecordId(uid);
			m_pRedisPut->setExchangeRecordId(uid, eid + 1);
			exr.set_eid(eid);
			exr.set_title(XXIconv::GBK2UTF("兑换码兑换"));
			exr.set_orderid(code);
			exr.set_time(Common::getLocalTime());
			exr.set_status(1);
			m_pRedisPut->PushExRecord(uid, &exr);

			CExchangeRecord ce;
			ce.set_cmd(ce.cmd());
			int sz = ce.ByteSize();
			char *buffer = new char[sz];
			ce.SerializePartialToArray(buffer, sz);
			ccEvent *ev = new ccEvent(ce.cmd(), buffer, sz, event->m_fd, event->m_type);
			HandlerCExchangeRecord(ev);
			delete ev;
			ev = NULL;
			
		}
		else{
			sl.set_err(1);
		}

		SendSExchangeCode(sl, event->m_fd);
	}
}

void HallInfo::SendSExchangeRecord(SExchangeRecord cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCExchangeRecord(ccEvent *event){
	CExchangeRecord cl;
	cl.CopyFrom(*event->msg);
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		char buff[100];
		SExchangeRecord se;
		se.set_cmd(se.cmd());
		auto vec = m_pRedisGet->getExRecord(data->_uid);
		auto itr = vec.begin();
		for (itr; itr != vec.end();itr++){
			ExRecord *ea = se.add_list();
			ExRecord *ea1 = itr->second;
			ea->CopyFrom(*ea1);
		}
		SendSExchangeRecord(se, event->m_fd);
	}
}

void HallInfo::SendSExchange(SExchange cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCExchange(ccEvent *event){
	CExchange cl;
	cl.CopyFrom(*event->msg);
	
	int id = cl.id();
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		ExAward *ea = m_pRedisGet->getExAward(id);
		SExchange sl;

		UserBase *user = LoginInfo::getIns()->getUserBase(uid);
		Reward buyrewad = ea->buy();
		int number = buyrewad.number();
		int pid = buyrewad.prop().id();
		int mynumber = 0;
		if (pid == 1){
			mynumber = user->gold();
		}
		else if (pid == 2){
			mynumber = user->diamond();
		}
		else if (pid == 3){
			mynumber = user->card();
		}
		if (number > mynumber){
			sl.set_err(1);
			SendSExchange(sl, event->m_fd);
			return;
		}
		
		string code = m_pRedisGet->getExchangeCode();
		char buff[10];
		sprintf(buff, "%d", atoi(code.c_str()) + 1);
		m_pRedisPut->setExchangeCode(buff);
		sl.set_code(code);
		sl.set_id(id);

		ExRecord exr;
		int eid = m_pRedisGet->getExchangeRecordId(uid);
		m_pRedisPut->setExchangeRecordId(uid, eid + 1);
		exr.set_eid(eid);
		exr.set_title(ea->title());
		exr.set_orderid(code);
		exr.set_time(Common::getLocalTime());
		exr.set_status(0);
		m_pRedisPut->PushExRecord(uid,&exr);

		SendSExchange(sl, event->m_fd);

		
		HttpPay::getIns()->NoticePushCurrency(buyrewad, uid,false);
	}
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
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		int id = cl.id();
		string body = cl.body();
		string uid = data->_uid;
		char buff[50];
		sprintf(buff,"%d",id);
		ShopItem *sp = m_pRedisGet->getShop(id);
		int price= sp->consume().number();
		
		sl = HttpPay::getIns()->requestOrder(uid, buff, price, body, data->_ip);
	}
	
	SendSWxpayOrder(sl, event->m_fd);
}


void HallInfo::SendSWxpayQuery(SWxpayQuery cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
}

void HallInfo::HandlerCWxpayQuery(ccEvent *event){
	CWxpayQuery cl;
	cl.CopyFrom(*event->msg);

	string id = cl.transid();

	HttpPay::getIns()->requestCheckKH(id,true);

	SWxpayQuery sl;
	sl.set_transid(id);
	SendSWxpayQuery(sl, event->m_fd);
}

void HallInfo::SendSFirsyBuyData(SFirsyBuyData cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void HallInfo::HandCFirsyBuyData(ccEvent *event){
	SFirsyBuyData sl;
	int fd = event->m_fd;
	FirstBuyItem *fbi = m_pRedisGet->getFirstBuy();
	if (fbi){
		sl.set_id(fbi->_sid);
		for (int i = 0; i < fbi->_rid.size(); i++){
			int rid = fbi->_rid.at(i);
			Reward *rd = sl.add_reward();
			Reward *rd1 = m_pRedisGet->getReward(rid);
			rd->CopyFrom(*rd1);
		}
		Reward *conreward = sl.mutable_consume();
		Reward *rd1 = m_pRedisGet->getReward(fbi->_conid);
		conreward->CopyFrom(*rd1);
		for (int i = 0; i < fbi->_giveid.size(); i++){
			int rid = fbi->_giveid.at(i);
			Reward *rd = sl.add_give();
			Reward *rd1 = m_pRedisGet->getReward(rid);
			rd->CopyFrom(*rd1);
		}
	}
	else{
		sl.set_err(1);
	}
	SendSFirsyBuyData(sl, fd);
}

void HallInfo::SendSFirstBuy(SFirstBuy cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);
	if (cl.err() == 0){
		ClientData *data = LibEvent::getIns()->getClientData(fd);
		if (data){
			string uid = data->_uid;
			SConfig *sc = m_pRedisGet->getSConfig(data->_uid);
			sc->set_firstbuy(false);
			m_pRedisPut->setConfig(uid,POINT_SHOUCHONG,false);
			ConfigInfo::getIns()->SendSConfig(*sc, fd);
		}
	}
}

void HallInfo::HandlerCFirstBuy(ccEvent *event){
	CFirstBuy cl;
	cl.CopyFrom(*event->msg);

	SFirstBuy swo;

	int type = cl.type();
	swo.set_type(type);
	if (type == 1){
		//苹果支付
	}
	else if (type==2){
		//微信支付
		SWxpayOrder sl;
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			FirstBuyItem *p = m_pRedisGet->getFirstBuy();
			int id = p->_sid;
			string body = XXIconv::GBK2UTF("首充10元");
			string uid = data->_uid;
			char buff[50];
			sprintf(buff, "%d", id);
			Reward *conrew = m_pRedisGet->getReward(p->_conid);
			int price = conrew->number();
			sl = HttpPay::getIns()->requestOrder(uid, buff, price, body, data->_ip);
			swo.set_noncestr(sl.noncestr());
			swo.set_payreq(sl.payreq());
			swo.set_timestamp(sl.timestamp());
			swo.set_sign(sl.sign());
			swo.set_err(sl.err());
		}
		else{
			swo.set_err(1);
		}
		
	}
	else if (type==3){
		//支付宝支付
		SAliPayOrder sl;
		ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
		if (data){
			FirstBuyItem *p = m_pRedisGet->getFirstBuy();
			int id = p->_sid;
			string body = XXIconv::GBK2UTF("首充10元");
			string uid = data->_uid;
			char buff[50];
			sprintf(buff, "%d", id);
			Reward *conrew = m_pRedisGet->getReward(p->_conid);
			int price = conrew->number();

			sl = HttpAliPay::getIns()->requestOrder(uid, buff, price, body, data->_ip, type);

			swo.set_noncestr(sl.orderinfo());
			swo.set_payreq(sl.appid());
			swo.set_timestamp(sl.timestamp());
			swo.set_sign(sl.privatekey());
			swo.set_err(sl.err());
		}
	}
	SendSFirstBuy(swo, event->m_fd);
}

void HallInfo::SendSAliPayOrder(SAliPayOrder cpo, int fd){
	LibEvent::getIns()->SendData(cpo.cmd(), &cpo, fd);
}

void HallInfo::HandlerCAliPayOrder(ccEvent *event){
	CAliPayOrder cl;
	cl.CopyFrom(*event->msg);
	
	SAliPayOrder sl;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		int id = cl.id();
		string body = cl.body();
		string uid = data->_uid;
		int type = cl.type();
		char buff[50];
		sprintf(buff, "%d", id);
		ShopItem *sp = m_pRedisGet->getShop(id);
		int price = sp->consume().number();

		sl = HttpAliPay::getIns()->requestOrder(uid, buff, price, body, data->_ip,type);
	}

	SendSAliPayOrder(sl, event->m_fd);
}

void HallInfo::SendSAliPayResult(SAliPayResult sp, int fd){
	LibEvent::getIns()->SendData(sp.cmd(), &sp, fd);
}

void HallInfo::HandlerCAliPayResult(ccEvent *event){
	CAliPayResult cl;
	cl.CopyFrom(*event->msg);

	SAliPayResult sl;
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string content = cl.content();
		HttpAliPay::getIns()->respondResult(content);
	}

	SendSAliPayResult(sl, event->m_fd);
}

//反馈
void HallInfo::SendSFeedBack(SFeedBack cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCFeedBack(ccEvent *event){
	CFeedBack cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string uname = cl.uname();
	string content = cl.content();
	m_pRedisPut->PushFeedBack(&cl);

	SFeedBack sl;
	sl.set_cmd(sl.cmd());
	SendSFeedBack(sl, event->m_fd);
}


//签到
void HallInfo::SendSSign(SSign cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCSign(ccEvent *event){
	CSign cl;
	cl.CopyFrom(*event->msg);
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid =data->_uid;
		SSign sl;
		SignStatus *ss = m_pRedisGet->getSignStatus(uid);
		if (!ss){
			ss = new SignStatus();
			ss->_uid = uid;
		}
		string time = ss->_time;
		if (m_lastday.compare(time) != 0){
			ss->_issign = false;
			ss->_left = 1;
			if (time.empty()){
				ss->_signcount = 0;
			}
			else{
				int a1 = atoi(m_lastday.substr(8, m_lastday.length()).c_str());
				int a2 = atoi(time.substr(8, time.length()).c_str());
				if (a1 - a2 > 1){
					ss->_signcount = 0;
				}
			}
			ss->_time = m_lastday;
		}
		if (!ss->_issign&&ss->_left>0){
			auto vec1 = m_pRedisGet->getSignZhuan();
			int sz =vec1.size();
			int rd = rand() % sz+1;
			sl.set_index(rd);
			sl.set_count(ss->_signcount + 1);
			ss->_issign = true;
			ss->_left -= 1;
			ss->_signcount += 1;
			ss->_time = m_lastday;
			m_pRedisPut->setSignStatus(ss);
			
			//换成邮件发送

			//SMail sm;
			
			Mail ml;
			Reward *rew = ml.add_rewardlist();
			rew->CopyFrom(vec1.at(rd)->reward());

			int eid = m_pRedisGet->getMailID();
			ml.set_eid(eid);
			char buff[300];
			sprintf(buff,"%s%d%s",XXIconv::GBK2UTF("恭喜您，抽奖获得").c_str(),rew->number(),rew->prop().name().c_str());
			ml.set_content(buff);
			ml.set_status(1);
			ml.set_time(Common::getLocalTime());
			ml.set_title(XXIconv::GBK2UTF("恭喜您，抽奖获得"));

			m_pRedisPut->setMailID(eid);
			m_pRedisPut->PushMail(uid, &ml);
			m_pRedisPut->setConfig(uid, POINT_MAIL, true);
			SConfig *sc = m_pRedisGet->getSConfig(uid);
			if (sc&&sc->yqs()){
				sc->set_yqs(false);
				sc->set_mail(true);
				m_pRedisPut->PushConfig(uid, sc);
				ConfigInfo::getIns()->SendSConfig(*sc,event->m_fd);
			}
			//SendSMail(sm,event->m_fd);
		}
		else{
			sl.set_err(1);
		}
		SendSSign(sl, event->m_fd);
	}
}


void HallInfo::SendSSignList(SSignList cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl,fd);

}

void HallInfo::HandlerCSignList(ccEvent *event){
	CSignList cl;
	cl.CopyFrom(*event->msg);
	EventDispatcher::getIns()->removeListener(cl.cmd(), this, Event_Handler(HallInfo::HandlerCSignList), GAME_TYPE);
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string uid = data->_uid;
		SSignList sl;

		SignStatus *ss = m_pRedisGet->getSignStatus(uid);
		if (!ss){
			ss = new SignStatus();
			ss->_uid = uid;
		}
		string time = ss->_time;
		if (m_lastday.compare(time) != 0){
			ss->_issign = false;
			ss->_left = 1;
			if (time.empty()){
				ss->_signcount = 0;
			}
			else{
				int a1 = atoi(m_lastday.substr(8, m_lastday.length()).c_str());
				int a2 = atoi(time.substr(8, time.length()).c_str());
				if (a1 - a2 > 1){
					ss->_signcount = 0;
				}
			}
			ss->_time = m_lastday;
			m_pRedisPut->setSignStatus(ss);
		}


		sl.set_count(ss->_signcount);
		sl.set_sign(ss->_issign);
		auto vec = m_pRedisGet->getSignAward();
		auto itr = vec.begin();
		for (itr; itr != vec.end();itr++){
			SignAward *sa1 = itr->second;
			SignAward *sa = sl.add_reward();
			sa->CopyFrom(*sa1);
		}
		auto vec1 = m_pRedisGet->getSignZhuan();
		auto itr1 = vec1.begin();
		for (itr1; itr1 != vec1.begin();itr1++){
			SignZhuan *sz1 = itr1->second;
			SignZhuan *sz = sl.add_zhuan();
			sz->CopyFrom(*sz1);
		}

		SendSSignList(sl, event->m_fd);
	}
}

void HallInfo::update(float){
	string time = Common::getLocalTimeDay();
	if (m_lastday.compare(time) != 0){
		m_lastday = time;
	}
}
