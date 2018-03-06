#pragma once
#include "stdafx.h"

enum ColumnType
{
	String_Type = 0,
	Int_Type,
	Float_Type,
	Double_Type,
	Bool_Type,
};

class DataBaseUserInfo{
public:
	DataBaseUserInfo();
	~DataBaseUserInfo();
	static DataBaseUserInfo *getIns();

	//////////////DBUserInfo
	YMSocketData setDBData(map<string, string> vecs, string tablename);//设置整个数据
	void setDBData(YMSocketData sd, string tablename);//设置整个数据
	//DBUserInfo getDBUserInfo(char *uid);

	bool insertDBData(YMSocketData sd, string tablename);
	
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

	map<string, YMSocketData> getDBDatas(string tablename);
	vector<string> getTableColumnName(string tablename);
	vector<string> getTableColumnType(string tablename);

	std::vector<YMSocketData> getDBData(string tablename, string coname, string covalue);
	string getTablePrikey(string tablename);

	
	vector<string> getAllDatabases();
	vector<string> getAllTables();
// 
// 	static string g_dbitennames[12];
// 	static string g_dbrecordsnames[4];
// 	static string g_dbdetailrecordsnames[5];
private:
	void startAI();
	
	//////////////DBUserInfo
	void getAllDBData(string tablename);
	bool getDBUser(char *uid, YMSocketData &dbuser);
	//std::vector<std::string> getDBUserData(char *uid);
	ColumnType getColumnType(string tablename, string name);
	
private:
	map<string, map<string, YMSocketData>>m_dbdatas;
	vector<string> m_databases;
	vector<string> m_curdbtables;
	static DataBaseUserInfo *m_ins;
	string m_maxuid;
	map<string, vector<string>>m_tablecolumnsname;
	map<string, vector<string>>m_tablecolumnstype;
	map<string, string>m_tableprikey;
};