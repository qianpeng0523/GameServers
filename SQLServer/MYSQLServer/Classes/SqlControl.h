#pragma once
#include "stdafx.h"
#include "mysql.h"

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"
#pragma comment(lib,"libmysql.lib") 

enum sqloptype{
	select_sql = 1,
	update_sql,
	insert_sql,
	showdatatses_sql,
	column_sql,
};

class SqlControl{
public:
	SqlControl();
	~SqlControl();

	static SqlControl* getIns();
	void init();
	bool start();
	bool close();
	int ExcuteQuery(char* sqlstr, std::vector<std::string> &vecs, sqloptype type = select_sql);
	int ExcuteQueryAll(char* sqlstr, vector<vector<string>> &allvecs);

	bool SelectDB(string dbname);
	vector<string> getAllDatabases();
	vector<string> getAllTables();

	bool isConnect();
private:
	static SqlControl *m_ins;
	MYSQL * m_mysql;
};