#ifndef __HttpLogic_SCENE_H__
#define __HttpLogic_SCENE_H__

#include "stdafx.h"

struct SQLInfo
{
	string _name;
	string _pass;
	string _ip;
	int _port;
	string _db;
};

class HttpLogic
{
public:
	HttpLogic();
	~HttpLogic();
    virtual bool init();
	static HttpLogic *getIns();
	void HandleLogic(YMSocketData sd, char *&buff, int &sz);
	SQLInfo *getSQLInfo(){
		return m_pSQLInfo;
	}
	void aes_encrypt(char* in, int inlen, char* out);
	void aes_decrypt(char* in, int inlen, char* out);
	
	void setGateUse(string name, bool isUse);
private:
	void SqlStart(YMSocketData sd, char *&buff, int &sz);
	void SqlClose(char *&buff, int &sz);
	void SelectDB(string dbname, char *&buff, int &sz);
	void SqlConnect(char *&buff, int &sz);
	void SqlColumns(string tname, char *&buff, int &sz);
	void SqlFind(YMSocketData sd, char *&buff, int &sz);
	void SqlExcute(YMSocketData sd, char *&buff, int &sz);
	void SqlBackup(string dbname, char *&buff, int &sz);
	
	void getGateData(int cmd,char *&buff, int &sz);
	void getLogicManagerData(int cmd,char *&buff, int &sz);
	void getDBData(YMSocketData sd, char *&buff, int &sz);
	void insertDBData(YMSocketData sd, char *&buff, int &sz);
private:
	static HttpLogic *m_Ins;
	SQLInfo *m_pSQLInfo;
	map<string, bool> m_gateUse;
public:
	
};

#endif 