
#ifndef __LoginInfo__
#define __LoginInfo__

#include "stdafx.h"



class LoginInfo:public Object
{
public:
	LoginInfo();
	~LoginInfo();
	static LoginInfo* getIns();
	bool init();
public:
	void SendSLogin(YMSocketData sd,int fd);
	void HandlerCLoginHand(ccEvent *event);
	void SendSRegister(YMSocketData sd, int fd);
	void HandlerCRegister(ccEvent *event);
	::google::protobuf::Message * getDBDataFromSocketData(string tablename, CSJson::Value sd);
	void setDBDataToSocketData(string tablename, ::google::protobuf::Message* data, YMSocketData &sd);
private:
	static LoginInfo *m_shareLoginInfo;
	
};

#endif 
