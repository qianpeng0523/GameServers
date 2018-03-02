#include "SqlControl.h"
#include "DataBaseUserinfo.h"

SqlControl *SqlControl::m_ins = NULL;

SqlControl::SqlControl()
{	
	m_mysql = NULL;

}

SqlControl::~SqlControl()
{
	if (m_mysql){
		close();
	}
}

SqlControl* SqlControl::getIns(){
	if (m_ins == NULL){
		m_ins = new SqlControl();
		m_ins->init();
	}
	return m_ins;
}

void SqlControl::init(){
	start();
}

bool SqlControl::start(){
	char *query = NULL;
	int rt;
	m_mysql = mysql_init((MYSQL*)0);
	if (m_mysql != NULL && mysql_real_connect(m_mysql, DBIP, DBUSER, DBPASSWD, DBNAME, DBPORT, NULL, 0)) {
		if (!mysql_select_db(m_mysql, DBNAME)) {
			printf("Select successfully the database!\n");
			m_mysql->reconnect = 1;
			query = "set names \'GBK\'";
			rt = mysql_real_query(m_mysql, query, strlen(query));
			if (rt) {
				printf("Error making query: %s !!!\n", mysql_error(m_mysql));
			}
			else {
				printf("query %s succeed!\n", query);
			}
			DataBaseUserInfo::getIns();
			return true;
		}
		return false;
	}
	else {
		printf("Unable to connect the database,check your configuration!");
	}
	return false;
}

void SqlControl::close(){
	mysql_close(m_mysql);
}

std::vector<std::string> SqlControl::ExcuteQuery(char* sqlstr, sqloptype type){
	printf("%s\n",sqlstr);
	int rt;
	rt = mysql_real_query(m_mysql, sqlstr, strlen(sqlstr));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(m_mysql));
	}
	else
	{
		printf("%s executed!!!\n", sqlstr);
	}
	MYSQL_RES *res;
	res = mysql_store_result(m_mysql);//将结果保存在res结构体中
	int t = 0;
	int count = 0;
	std::vector<std::string> vecs;
	if (res){
		MYSQL_ROW row;
		while (row = mysql_fetch_row(res)) {
			for (t = 0; t < mysql_num_fields(res); t++) {
				std::string s = row[t];
				//printf("%s ",s.c_str());
				vecs.push_back(s);
			}
			//printf("\n");
			count++;
		}
		//printf("number of rows %d\n", count);
		mysql_free_result(res);
	}
	else{
		if (type == select_sql){
			printf("未能查找到数据\n");
		}
	}
	return vecs;
}