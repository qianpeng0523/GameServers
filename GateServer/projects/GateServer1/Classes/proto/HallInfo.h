
#ifndef __HallInfo__
#define __HallInfo__

#include "stdafx.h"
#include "ccEvent.h"
#include "EventDispatcher.h"

class HallInfo:public Object
{
public:
	HallInfo();
	~HallInfo();
	static HallInfo* getIns();
	bool init();
public:
	//排行榜
	void SendSRank(SRank cl, int fd);
	void HandlerCRankHand(ccEvent *event);

	//商城
	void SendSShop(SShop cl, int fd);
	void HandlerCShop(ccEvent *event);

	//邮件
	void SendSMail(SMail sl, int fd);
	void HandlerCMail(ccEvent *event);

	void SendSMailAward(SMailAward cl, int fd);
	void HandlerCMailAward(ccEvent *event);

	//好友
	void SendSFriend(SFriend cl, int fd);
	void HandlerCFriend(ccEvent *event);

	void SendSFindFriend(SFindFriend cl, int fd);
	void HandlerCFindFriend(ccEvent *event);

	void SendSGiveFriend(SGiveFriend cl, int fd);
	void HandlerCGiveFriend(ccEvent *event);

	void SendSAddFriend(SAddFriend cl, int fd);
	void HandlerCAddFriend(ccEvent *event);

	void SendSAddFriendList(SAddFriendList sd, int fd);
	void HandlerCAddFriendList(ccEvent *event);

	void SendSAgreeFriend(SAgreeFriend cl, int fd);
	void HandlerCAgreeFriend(ccEvent *event);

	//活动
	void SendSActive(SActive cl, int fd);
	void HandlerCActive(ccEvent *event);

	//任务
	void SendSTask(STask sd, int fd);
	void HandlerCTask(ccEvent *event);

	///////////兑换
	void SendSReward(SReward cl, int fd);
	void HandlerCReward(ccEvent *event);

	void SendSExchangeReward(SExchangeReward cl, int fd);
	void HandlerCExchangeReward(ccEvent *event);

	void SendSExchangeCode(SExchangeCode cl, int fd);
	void HandlerCExchangeCode(ccEvent *event);

	void SendSExchangeRecord(SExchangeRecord cl, int fd);
	void HandlerCExchangeRecord(ccEvent *event);

	void SendSExchange(SExchange cl, int fd);
	void HandlerCExchange(ccEvent *event);

	//////////支付
	void SendSApplePay(SApplePay cl, int fd);
	void HandlerCApplePay(ccEvent *event);

	void SendSWxpayOrder(SWxpayOrder cl, int fd);
	void HandlerCWxpayOrder(ccEvent *event);

	void SendSWxpayQuery(SWxpayQuery cl, int fd);
	void HandlerCWxpayQuery(ccEvent *event);

	void SendSFirstBuy(SFirstBuy cl, int fd);
	void HandlerCFirstBuy(ccEvent *event);

	//反馈
	void SendSFeedBack(SFeedBack cl, int fd);
	void HandlerCFeedBack(ccEvent *event);

	//签到
	void SendSSign(SSign cl, int fd);
	void HandlerCSign(ccEvent *event);

	void SendSSignList(SSignList cl, int fd);
	void HandlerCSignList(ccEvent *event);
private:
	void regist(int cmd, string name, EventHandler handler);
private:
	static HallInfo *m_shareHallInfo;
	
};

#endif 
