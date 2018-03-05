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
	bool updateDBUserInfoByUid(std::map<string, void *>updatedata,string uid);
	bool updateDBUserInfoByUid(string upname,void *upvalue, string uid);
	int updateDBUserInfoByKey(std::map<string, string >updatedata, string key, string value);

	void setDatabases(vector<string> dbs){
		m_databases = dbs;
	}
	vector<string> getDatabases(){
		return m_databases;
	}

	void setdbtables(vector<string> tables){
		m_curdbtables = tables;
	}
	vector<string> getdbtables(){
		return m_curdbtables;
	}

	std::map<string, DBUserInfo> getUserInfoDatas();
	vector<string> getTableColumnName(string tablename);

	DBUserInfo getDBUserInfo(string coname, string covalue);
	string getTablePrikey(string tablename);

	void getDBUserFromSocketData(DBUserInfo &user, YMSocketData sd, string listname="", int index=0);
	void setDBUserToSocketData(DBUserInfo user, YMSocketData &sd, string listname="", int index=0);

	static string g_dbitennames[12];
private:
	void startAI();
	
	//////////////DBUserInfo
	void getAllDBUserInfo();
	bool getDBUser(char *uid, DBUserInfo &dbuser);
	std::vector<std::string> getDBUserData(char *uid);
	
private:
	std::map<string,DBUserInfo> m_dbusers;
	vector<string> m_databases;
	vector<string> m_curdbtables;
	static DataBaseUserInfo *m_ins;
	string m_maxuid;
	map<string, vector<string>>m_tablecolumnsname;
	map<string, string>m_tableprikey;
};