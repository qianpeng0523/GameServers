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

	void HandleLogic(YMSocketData sd, char *&buff, int &sz);
	void respondleLogic(YMSocketData sd);

	string getServerName(){
		return m_servername;
	}
private:
	static HttpLogic *m_Ins;
	string m_logicmnip;
	int m_logicmnport;
	string m_servername;
public:
	
};

#endif 