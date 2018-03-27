
#ifndef __HallInfo__
#define __HallInfo__

#include "stdafx.h"
#include "ccEvent.h"


class HallInfo:public Object
{
public:
	HallInfo();
	~HallInfo();
	static HallInfo* getIns();
	bool init();
public:
	//排行榜
	void SendSRank(YMSocketData sd, int fd);
	void HandlerCRankHand(ccEvent *event);

	//商城
	void SendSShop(YMSocketData sd, int fd);
	void HandlerCShop(ccEvent *event);

	//邮件
	void SendSMail(YMSocketData sd, int fd);
	void HandlerCMail(ccEvent *event);

	void SendSMailAward(YMSocketData sd, int fd);
	void HandlerCMailAward(ccEvent *event);

	//好友
	void SendSFriend(YMSocketData sd, int fd);
	void HandlerCFriend(ccEvent *event);

	void SendSFindFriend(YMSocketData sd, int fd);
	void HandlerCFindFriend(ccEvent *event);

	void SendSGiveFriend(YMSocketData sd, int fd);
	void HandlerCGiveFriend(ccEvent *event);

	void SendSAddFriend(YMSocketData sd, int fd);
	void HandlerCAddFriend(ccEvent *event);

	void SendSAddFriendList(YMSocketData sd, int fd);
	void HandlerCAddFriendList(ccEvent *event);

	void SendSAgreeFriend(YMSocketData sd, int fd);
	void HandlerCAgreeFriend(ccEvent *event);

	//活动
	void SendSActive(YMSocketData sd, int fd);
	void HandlerCActive(ccEvent *event);

	//任务
	void SendSTask(YMSocketData sd, int fd);
	void HandlerCTask(ccEvent *event);

	///////////兑换
	void SendSReward(YMSocketData sd, int fd);
	void HandlerCReward(ccEvent *event);

	void SendSExchangeReward(YMSocketData sd, int fd);
	void HandlerCExchangeReward(ccEvent *event);

	void SendSExchangeCode(YMSocketData sd, int fd);
	void HandlerCExchangeCode(ccEvent *event);

	void SendSExchangeRecord(YMSocketData sd, int fd);
	void HandlerCExchangeRecord(ccEvent *event);

	void SendSExchange(YMSocketData sd, int fd);
	void HandlerCExchange(ccEvent *event);

	//////////支付
	void SendSApplePay(YMSocketData sd, int fd);
	void HandlerCApplePay(ccEvent *event);

	void SendSWxpayOrder(YMSocketData sd, int fd);
	void HandlerCWxpayOrder(ccEvent *event);

	void SendSWxpayQuery(YMSocketData sd, int fd);
	void HandlerCWxpayQuery(ccEvent *event);

	void SendSFirstBuy(YMSocketData sd, int fd);
	void HandlerCFirstBuy(ccEvent *event);

	//反馈
	void SendSFeedBack(YMSocketData sd, int fd);
	void HandlerCFeedBack(ccEvent *event);

	//签到
	void SendSSign(YMSocketData sd, int fd);
	void HandlerCSign(ccEvent *event);

	void SendSSignList(YMSocketData sd, int fd);
	void HandlerCSignList(ccEvent *event);

private:
	static HallInfo *m_shareHallInfo;
	
};

#endif 
