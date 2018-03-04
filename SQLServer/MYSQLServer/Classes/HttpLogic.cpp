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
		SqlExcute(sd, buff, sz);
	}
	else if (cmd == 0x04){
		SelectDB(sd["dbname"].asString(), buff, sz);
	}
	else if (cmd == 0x05){
		SelectTableData(sd["tname"].asString(), buff, sz);
	}
	else if (cmd == 0x06){
		SqlConnect( buff, sz);
	}
	else if (cmd == 0x07){
		SqlColumns(sd["tname"].asString(),buff, sz);
	}
	else if (cmd == 0x08){
		SqlFind(sd, buff, sz);
	}
}

void HttpLogic::SqlConnect(char *&buff, int &sz){
	YMSocketData sd1;
	sd1["err"] = SqlControl::getIns()->isConnect();
	sd1.serializer(buff, &sz);
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
	YMSocketData sd;
	sd["err"] = 0;
	sd["tname"] = tname;
	
	if (tname.compare("userinfo") == 0){
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
	}
	else if(tname.compare("records")==0){
		
	}
	sd.serializer(buff, &sz);
}

void HttpLogic::SqlColumns(string tname, char *&buff, int &sz){
	YMSocketData sd;
	sd["err"] = 0;
	sd["tname"] = tname;
	
	
	vector<string>vec = DataBaseUserInfo::getIns()->getTableColumnName(tname);
	for (int i = 0; i < vec.size(); i++){
		sd["columns"][i] = vec.at(i);
	}
	sd.serializer(buff, &sz);
}

void HttpLogic::SqlFind(YMSocketData sd, char *&buff, int &sz){
	string tname = sd["tname"].asString();
	string coname = sd["coname"].asString();
	string covalue = sd["covalue"].asString();
	YMSocketData sd1;
	sd1["tname"] = tname;
	sd1["prikey"] = DataBaseUserInfo::getIns()->getTablePrikey(tname);
	if (tname.compare("userinfo") == 0){
		string prikey;
		DBUserInfo user= DataBaseUserInfo::getIns()->getDBUserInfo(coname, covalue);
		int sz = user.ByteSize();
		char* sm = new char[sz+1];
		user.SerializeToArray(sm,sz);
		sd1["data"] = sm; 
		printf("sqlfind:%s",sd1["data"].asString().c_str());
		delete sm;
	}
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlExcute(YMSocketData sd, char *&buff, int &sz){
	string tname = sd["tname"].asString();
	string key = sd["key"].asString();
	string keyvalue = sd["keyvalue"].asString();
	printf("%s",sd.getJsonString().c_str());
	int err = 0;
	if (tname.compare("userinfo") == 0){
		map<string, string>maps;
		for (int i = 0; i < 12; i++){
			string coname = DataBaseUserInfo::g_dbitennames[i];
			if (sd.isMember(coname)){
				string covalue= sd[coname].asString();
				maps.insert(make_pair(coname,covalue));
			}
		}
		err=DataBaseUserInfo::getIns()->updateDBUserInfoByKey(maps, key, keyvalue);
	}
	YMSocketData sd1;
	sd1["err"] = err;
	sd1["tname"] = tname;
	sd1.serializer(buff, &sz);
}