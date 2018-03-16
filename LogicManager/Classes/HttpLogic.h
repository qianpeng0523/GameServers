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
	void requestManagerData();
	void ManagerDataCall(YMSocketData sd);
	void HandleLogic(YMSocketData sd, char *&buff, int &sz);
	void respondleLogic(YMSocketData sd);

	void aes_encrypt(char* in, int inlen, char* out);
	void aes_decrypt(char* in, int inlen, char* out);
private:
	static HttpLogic *m_Ins;
	
public:
	static int SERVER_PORT;
	static string SERVER_CODE;
};

#endif 