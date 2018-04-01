
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
	void SendSLogin(SLogin cl, int fd);
	void HandlerCLoginHand(ccEvent *event);
	void SendSRegister(SRegister sd, int fd);
	void HandlerCRegister(ccEvent *event);
	::google::protobuf::Message * getDBDataFromSocketData(string tablename, CSJson::Value sd);
	void setDBDataToSocketData(string tablename, ::google::protobuf::Message* data, YMSocketData &sd);
	void setDBDataToSocketDataVo(::google::protobuf::Message* data, CSJson::Value &sd);
	::google::protobuf::Message *getDBDataFromSocketDataVo(string name,CSJson::Value sd);
	
	void Check(float dt);
private:
	static LoginInfo *m_shareLoginInfo;
	
};

#endif 
