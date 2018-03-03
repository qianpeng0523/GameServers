#include "HttpLogic.h"
#include "SqlControl.h"
#include "DataBaseUserInfo.h"

HttpLogic *HttpLogic::m_Ins = NULL;


HttpLogic::HttpLogic(){
	m_pSQLInfo = new SQLInfo();
}
HttpLogic::~HttpLogic(){
	delete m_pSQLInfo;
	m_pSQLInfo = NULL;
}

bool HttpLogic::init()
{
	
    return true;
}

HttpLogic *HttpLogic::getIns(){
	if (!m_Ins){
		m_Ins = new HttpLogic();
		m_Ins->init();
	}
	return m_Ins;
}

void HttpLogic::HandleLogic(YMSocketData sd, char *&buff, int &sz){
	int cmd = sd["cmd"].asInt();
	if (cmd == 0x01){
		SqlStart(sd,buff,sz);
	}
	else if (cmd==0x02){
		SqlClose(buff,sz);
	}
	else if (cmd == 0x03){
		
	}
	else if (cmd == 0x04){
		SelectDB(sd["dbname"].asString(), buff, sz);
	}
	else if (cmd == 0x05){
		SelectTableData(sd["tname"].asString(), buff, sz);
	}
}

void HttpLogic::SqlStart(YMSocketData sd, char *&buff, int &sz){
	printf("%s\n",sd.getJsonString().c_str());
	m_pSQLInfo->_ip = sd["ip"].asString();
	m_pSQLInfo->_name = sd["name"].asString();
	m_pSQLInfo->_pass = sd["pass"].asString();
	m_pSQLInfo->_db = sd["db"].asString();
	m_pSQLInfo->_port = sd["port"].asInt();
	bool ist= SqlControl::getIns()->start();

	YMSocketData sd1;
	if (ist){
		sd1["err"] = 0;
		SQLInfo *p = HttpLogic::getIns()->getSQLInfo();
		sd1["dbname"] = p->_db;

		vector<string> dbs = DataBaseUserInfo::getIns()->getDatabases();
		for (int i = 0; i < dbs.size(); i++){
			sd1["dbs"][i] = dbs[i];
		}

		vector<string> tables = DataBaseUserInfo::getIns()->getdbtables();
		for (int i = 0; i < tables.size(); i++){
			sd1["tables"][i] = tables[i];
		}
	}
	else{
		sd1["err"] = 1;
	}
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlClose(char *&buff, int &sz){
	bool ist= SqlControl::getIns()->close();
	YMSocketData sd1;
	if (ist){
		sd1["err"] = 0;
	}
	else{
		sd1["err"] = 1;
	}
	sd1.serializer(buff, &sz);
}

void HttpLogic::SelectDB(string dbname, char *&buff, int &sz){
	bool ist= SqlControl::getIns()->SelectDB(dbname);
	YMSocketData sd;
	if (ist){
		sd["err"] = 0;
		sd["dbname"] = dbname;
		vector<string> tables = SqlControl::getIns()->getAllTables();
		for (int i = 0; i < tables.size(); i++){
			sd["tables"][i] = tables[i];
		}
	}
	else{
		sd["err"] = 1;
	}
	sd.serializer(buff, &sz);
}

void HttpLogic::SelectTableData(string tname, char *&buff, int &sz){
	if (tname.compare("userinfo") == 0){
		YMSocketData sd;
		sd["err"] = 0;
		sd["tname"] = tname;
		std::map<string, DBUserInfo> dbusers=DataBaseUserInfo::getIns()->getUserInfoDatas();
		std::map<string, DBUserInfo>::iterator itr = dbusers.begin();
		int i = 0;
		for (itr; itr != dbusers.end();itr++){
			string sm;
			DBUserInfo msg = itr->second;
			msg.SerializePartialToString(&sm);
			sd["tablelist"][i] = sm;
			i++;
		}
		sd.serializer(buff, &sz);
	}
}