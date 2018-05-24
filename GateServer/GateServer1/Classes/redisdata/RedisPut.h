#ifndef __LYNX_GAME_SERVER_RedisPut_H__
#define __LYNX_GAME_SERVER_RedisPut_H__
#include "stdafx.h"
#include "RedisGet.h"
using namespace std;


class RedisPut 
{
public:
	RedisPut();
	virtual ~RedisPut();
	
	static RedisPut *getIns();
	void init();

	bool PushConfig(string uid, SConfig *sc);
	bool setConfig(string uid, POINTTIP type, bool ist);

	bool PushUserBase(UserBase *ub);
	bool setUserBase(string uid, string key, string value);
	bool addUserBase(string uid, string key, int value);

	bool PushUserIndex(int index);

	bool PushUserLoginTime(string uid);

	bool PushPass(string uid, string pass);
	bool PushOpenid(string openid, string uid);

	bool PushRank(Rank *rk);

	bool PushMail(string uid,Mail* mail);
	void PopMail(string uid, Mail *ml);

	bool PushFriend(string uid, Friend *fd);
	bool eraseFriend(string uid,string fuid);
	bool PushFriendNotice(string uid, FriendNotice *fn);
	bool eraseFriendNotice(string uid, FriendNotice *fn);
	bool setFriendNoticeID(int id);

	bool PushTaskStatus(string uid, int taskid, Status *status);

	bool PushFreeStatus(string uid, int fid, Status *status);

	bool PushExRecord(string uid, ExRecord *er);
	bool setExchangeCode(string code);
	bool setExchangeRecordId(string uid, int id);

	bool setEXCode(string code, bool isdui);
	
	bool setSignStatus(SignStatus *ss);
	
	bool PushFeedBack(CFeedBack *cfb);

	bool setMailID(int mid);

	bool setNoTime(string time);
	bool setAliOuttradeNo(string num);
	bool pushAliPayOuttradeno(string uid, string out_trade_no, time_t time, int shopid, map<string, string> requestPairs);
	bool eraseAliPayOuttradeno(string out_trade_no);

	bool setWXNonceid(int nonceid);
	bool PushWXPayNoData(WXPayNoData *p);
	bool eraseWXPayNoData(WXPayNoData *p);

	bool PushPayRecord(PayRecord pr);
	
	void ZeroChange(char *&data, int sz);
private:
	static RedisPut *m_ins;
	redis *m_redis;
};


#endif // __LYNX_GAME_SERVER_RedisPut_H__