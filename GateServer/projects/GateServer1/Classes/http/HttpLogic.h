#ifndef __HttpLogic_SCENE_H__
#define __HttpLogic_SCENE_H__

#include "stdafx.h"

class HttpLogic:public Object
{
public:
	HttpLogic();
	~HttpLogic();
    virtual bool init();
	static HttpLogic *getIns();
	
public:
	void requestGateData();
	void GateDataCall(YMSocketData sd);

	void requestManagerData();
	void ManagerDataCall(YMSocketData sd);

	void requestDBData(string uid,string pwd);
	void DBDataCall(YMSocketData sd);

	void requestRegister(YMSocketData &sd);
	void RegisterCall(YMSocketData sd);

	void HandleLogic(YMSocketData sd, char *&buff, int &sz);
	void respondleLogic(YMSocketData sd);

	void requestRank(int type,int index);//获取排行榜 type表示财富榜还是局数榜
	void HandleRank(YMSocketData sd, char *&buff, int &sz);

	void requestMail(string sessionid);//获取邮件
	void HandleMail(YMSocketData sd, char *&buff, int &sz);

	void requestShop(int type);//获取商城信息
	void HandleShop(YMSocketData sd, char *&buff, int &sz);

	void requestFriends(string sessionid);
	void HandleFriends(YMSocketData sd, char *&buff, int &sz);

	void requestFindFriend(string uid);
	void HandlerFindFriend(YMSocketData sd, char *&buff, int &sz);

	void requestActive();
	void HandlerActive(YMSocketData sd, char *&buff, int &sz);

	void requestTask();
	void HandlerTask(YMSocketData sd, char *&buff, int &sz);

	void requestExchangeReward();
	void HandlerExchangeReward(YMSocketData sd, char *&buff, int &sz);

	void requestExchangeRecord(string uid);
	void HandlerExchangeRecord(YMSocketData sd, char *&buff, int &sz);

	void requestSignList(string uid);
	void HandlerSignList(YMSocketData sd, char *&buff, int &sz);



	string getServerName(){
		return m_servername;
	}

	void update(float dt);

	void aes_encrypt(char* in, int inlen, char* out);
	void aes_decrypt(char* in, int inlen, char* out);

	string m_logicmnip;
	int m_logicmnport;
private:
	static HttpLogic *m_Ins;
	
	string m_servername;
public:
	static int SERVER_PORT;
	static string SERVER_CODE;
};

#endif 