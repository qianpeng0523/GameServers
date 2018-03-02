#pragma once
#include "stdafx.h"
#include "mysql.h"

#define MYHTTPD_SIGNATURE   "myhttpd v 0.0.1"
#pragma comment(lib,"libmysql.lib") 

enum sqloptype{
	select_sql=1,
	update_sql,
	insert_sql,
};

class SqlControl{
public:
	SqlControl();
	~SqlControl();

	static SqlControl* getIns();
	void init();
	bool start();
	void close();
	std::vector<std::string> ExcuteQuery(char* sqlstr, sqloptype type=select_sql);
private:
	static SqlControl *m_ins;
	MYSQL * m_mysql;
};