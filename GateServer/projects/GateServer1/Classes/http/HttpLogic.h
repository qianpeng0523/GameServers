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
	void respondleLogic(YMSocketData sd);
	void HandleLogic(YMSocketData sd, char *&buff, int &sz);
	void requestGateData();
	void GateDataCall(YMSocketData sd);

	void requestManagerData();
	void ManagerDataCall(YMSocketData sd);

	void ChangeZero(char *&data, int sz);

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