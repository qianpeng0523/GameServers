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
	backup_sql,
};

class SqlControl{
public:
	SqlControl();
	~SqlControl();

	static SqlControl* getIns();
	void init();
	bool start();
	bool close();
	int ExcuteQuery(char* sqlstr, vector<map<string, string>> &vecs, sqloptype type = select_sql);
	int ExcuteQuery(char* sqlstr, map<string,string> &vecs, sqloptype type = select_sql);
	
	int ExcuteQuery1(char* sqlstr, vector< string> &vecs, sqloptype type = select_sql);

	bool SelectDB(string dbname);

	bool isConnect();
private:
	static SqlControl *m_ins;
	MYSQL * m_mysql;
};