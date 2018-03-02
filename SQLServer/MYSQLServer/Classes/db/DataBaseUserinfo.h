#pragma once
#include "stdafx.h"



class DataBaseUserInfo{
public:
	DataBaseUserInfo();
	~DataBaseUserInfo();
	static DataBaseUserInfo *getIns();

	//////////////DBUserInfo
	DBUserInfo setDBUserData(std::vector<std::string> vecs);//设置整个数据
	void setDBUserData(DBUserInfo dbuser);//设置整个数据
	DBUserInfo getDBUserInfo(char *uid);
	
	bool insertDBUserInfo(DBUserInfo dbuser);
	bool updateDBUserInfo(DBUserInfo dbuser,std::map<string,void *>maps);

private:
	void startAI();

	//////////////DBUserInfo
	bool getDBUser(char *uid, DBUserInfo &dbuser);
	std::vector<std::string> getDBUserData(char *uid);
private:
	std::map<string,DBUserInfo> m_dbusers;
	static DataBaseUserInfo *m_ins;
	int m_maxid;
	string m_maxuid;
};