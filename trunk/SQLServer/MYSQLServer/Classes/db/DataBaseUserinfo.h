#pragma once
#include "stdafx.h"



class DataBaseUserInfo{
public:
	DataBaseUserInfo();
	~DataBaseUserInfo();
	static DataBaseUserInfo *getIns();

	//////////////DBUserInfo
	::google::protobuf::Message* setDBData(std::vector<std::string> vecs, string tablename);//设置整个数据
	void setDBData(::google::protobuf::Message* msg,string tablename);//设置整个数据
	//DBUserInfo getDBUserInfo(char *uid);

	bool insertDBData(::google::protobuf::Message *msg, string tablename);
	
	int updateDBDataByKey(string tablename, std::map<string, string >updatedata, string key, string value);

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

	std::map<string, ::google::protobuf::Message*> getDBDatas(string tablename);
	vector<string> getTableColumnName(string tablename);

	std::vector<::google::protobuf::Message*> getDBData(string tablename, string coname, string covalue);
	string getTablePrikey(string tablename);

	void getDBUserFromSocketData(DBUserInfo &user, YMSocketData sd, string listname="", int index=0);
	void setDBUserToSocketData(DBUserInfo user, YMSocketData &sd, string listname="", int index=0);

	static string g_dbitennames[12];
	static string g_dbrecordsnames[4];
	static string g_dbdetailrecordsnames[5];
private:
	void startAI();
	
	//////////////DBUserInfo
	void getAllDBData(string tablename);
	bool getDBUser(char *uid, DBUserInfo &dbuser);
	//std::vector<std::string> getDBUserData(char *uid);
	bool isStringType(string tablename, string name);
private:
	std::map<string,DBUserInfo> m_dbusers;
	std::map<int, DBRecords> m_dbrecordss;
	std::map<int, DBDetailRecords> m_dbdetailrecords;
	vector<string> m_databases;
	vector<string> m_curdbtables;
	static DataBaseUserInfo *m_ins;
	string m_maxuid;
	map<string, vector<string>>m_tablecolumnsname;
	map<string, string>m_tableprikey;
};