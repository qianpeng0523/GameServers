﻿#include "SqlControl.h"
#include "DataBaseUserinfo.h"
#include "HttpLogic.h"
#include "HttpEvent.h"
SqlControl *SqlControl::m_ins = NULL;

static string g_othersql[] = {"information_schema","mysql","test","performance_schema"};

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
	
}

bool SqlControl::isConnect(){
	if (m_mysql&&m_mysql->info){
		return true;
	}
	return false;
}

bool SqlControl::start(){
	SQLInfo *p = HttpLogic::getIns()->getSQLInfo();
	if (!m_mysql){
		char *query = NULL;
		int rt;
		m_mysql = mysql_init((MYSQL*)0);
		if (m_mysql != NULL && mysql_real_connect(m_mysql, p->_ip.c_str(), p->_name.c_str(), p->_pass.c_str(), p->_db.c_str(), p->_port, NULL, 0)) {
			if (SelectDB(p->_db)) {
				printf("选择数据库成功!\n");
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
				getAllDatabases();
				getAllTables();
				return true;
			}
			return false;
		}
		else {
			
			printf("连接数据库失败，请检查配置!\n");
		}
	}
	else{
		SelectDB(p->_db);
		getAllDatabases();
		getAllTables();
		return true;
	}
	return false;
}

bool SqlControl::SelectDB(string dbname){
	return !mysql_select_db(m_mysql, dbname.c_str());
}

vector<string> SqlControl::getAllDatabases(){
	string sqlstr = "show databases";
	vector<string> vecs;
	int err= ExcuteQuery((char *)sqlstr.c_str(), vecs,showdatatses_sql);
	if (err == 0){
		for (int i = 0; i < 4; i++){
			string oname = g_othersql[i];
			vector<string>::iterator itr = vecs.begin();
			for (itr; itr != vecs.end(); itr++){
				if (oname.compare(*itr) == 0){
					vecs.erase(itr);
					break;
				}
			}
		}
		DataBaseUserInfo::getIns()->setDatabases(vecs);
	}
	return vecs;
}

vector<string> SqlControl::getAllTables(){
	string sqlstr = "show tables";
	vector<string> vecs;
	int err = ExcuteQuery((char *)sqlstr.c_str(),vecs, showdatatses_sql);
	if (err == 0){
		DataBaseUserInfo::getIns()->setdbtables(vecs);
	}
	return vecs;
}

bool SqlControl::close(){
	if (m_mysql){
		mysql_close(m_mysql);
		m_mysql = NULL;
		printf("关闭数据库成功\n");
		return true;
	}
	else{
		return false;
	}
}

int SqlControl::ExcuteQuery(char* sqlstr, std::vector<std::string> &vecs, sqloptype type){
	printf("%s\n",sqlstr);
	int rt;
	rt = mysql_real_query(m_mysql, sqlstr, strlen(sqlstr));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(m_mysql));
		return 1;
	}
	else
	{
		printf("%s executed!!!\n", sqlstr);
	}
	MYSQL_RES *res;
	res = mysql_store_result(m_mysql);//将结果保存在res结构体中
	int t = 0;
	int count = 0;
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
			return 2;
		}
		
	}
	return 0;
}

int SqlControl::ExcuteQueryAll(char* sqlstr, vector<vector<string>> &allvecs){
	printf("%s\n", sqlstr);
	int rt;
	rt = mysql_real_query(m_mysql, sqlstr, strlen(sqlstr));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(m_mysql));
		return 1;
	}
	else
	{
		printf("%s executed!!!\n", sqlstr);
	}
	MYSQL_RES *res;
	res = mysql_store_result(m_mysql);//将结果保存在res结构体中
	int t = 0;
	int count = 0;
	
	if (res){
		MYSQL_ROW row;
		while (row = mysql_fetch_row(res)) {
			std::vector<std::string> vecs;
			for (t = 0; t < mysql_num_fields(res); t++) {
				std::string s = row[t];
				vecs.push_back(s);
				
			}
			//printf("\n");
			allvecs.push_back(vecs);
			count++;
		}
		//printf("number of rows %d\n", count);
		mysql_free_result(res);
	}
	else{
		printf("未能查找到数据\n");
		return 2;
	}
	return 0;
}