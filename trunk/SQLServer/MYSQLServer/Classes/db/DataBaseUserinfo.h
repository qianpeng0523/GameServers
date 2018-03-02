#pragma once
#include "stdafx.h"



class DataBaseUserInfo{
public:
	DataBaseUserInfo();
	~DataBaseUserInfo();
	static DataBaseUserInfo *getIns();

	DBUserInfo setDBUserData(std::vector<std::string> vecs);//������������
	void setDBUserData(DBUserInfo dbuser);//������������
	DBUserInfo getDBUserInfo(char *uid);
	
	bool insertDBUserInfo(DBUserInfo dbuser);
private:
	bool getDBUser(char *uid, DBUserInfo &dbuser);
	void startAI();
	std::vector<std::string> getDBUserData(char *uid);
private:
	std::map<string,DBUserInfo> m_dbusers;
	static DataBaseUserInfo *m_ins;
	int m_maxid;
	string m_maxuid;
};