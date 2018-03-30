#include "HttpLogic.h"
#include "SqlControl.h"
#include "DataBaseUserInfo.h"
#include "aes.h"
#include "CSVDataInfo.h"





HttpLogic *HttpLogic::m_Ins = NULL;


void HttpLogic::aes_decrypt(char* in, int inlen, char* out)
{
	if (!in || !out) return;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, DECKEY, AES_BLOCK_SIZE);

	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)DECKEY, 128, &aes) < 0)
	{
		return ;
	}
	
	int num = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_DECRYPT);
	inlen = inlen / AES_BLOCK_SIZE*AES_BLOCK_SIZE;
	out[inlen+num] = '\0';
	
}

void HttpLogic::aes_encrypt(char* in, int inlen, char* out)
{
	if (!in || !out) return ;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, DECKEY, AES_BLOCK_SIZE);
	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)DECKEY, 128, &aes) < 0)
	{
		return ;
	}
	int num = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_ENCRYPT);
	inlen = inlen / AES_BLOCK_SIZE*AES_BLOCK_SIZE;
	out[inlen+num] = '\0';
	
}

HttpLogic::HttpLogic(){
	m_pSQLInfo = new SQLInfo();
	CSVDataInfo::getIns()->openCSVFile("res/gateserver.csv", CSV_GATESERVER);
	CSVDataInfo::getIns()->openCSVFile("res/logicmanager.csv", CSV_LOGOCMANAGER);
	
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
	printf("sd:%s\n",sd.getJsonString().c_str());
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
	else if (cmd == 0x06){
		SqlConnect( buff, sz);
	}
	else if (cmd == 0x07){
		SqlColumns(sd["tname"].asString(),buff, sz);
	}
	else if (cmd == 0x08){
		SqlFind(sd, buff, sz);
	}
	else if (cmd == 0x09){
		SqlBackup(sd["dbname"].asString(), buff, sz);
	}
	else if (cmd == 0x0A){
		getLogicManagerData(sd, buff, sz);
	}
	else if (cmd == 0x0B){
		getGateData(sd, buff, sz);
	}
	else if (cmd == 0x0C){
		getDBData(sd, buff, sz);
	}
	else if (cmd == 0x10){
		insertDBData(sd, buff, sz);
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
		vector<string> tables = DataBaseUserInfo::getIns()->getAllTables();
		for (int i = 0; i < tables.size(); i++){
			sd["tables"][i] = tables[i];
		}
	}
	else{
		sd["err"] = 1;
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
	DataBaseUserInfo *p = DataBaseUserInfo::getIns();
	sd1["prikey"] = p->getTablePrikey(tname);
	
	vector<YMSocketData> vec = p->getDBData(tname, coname, covalue);
	if (!vec.empty()){
		sd1["err"] = 0;
		for (int i = 0; i < vec.size(); i++){
			sd1["datas"][i] = vec.at(i);
		}
	}
	else{
		sd1["err"] = 1;
	}
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlExcute(YMSocketData sd, char *&buff, int &sz){
	string tname = sd["tname"].asString();
	string key = sd["key"].asString();
	string keyvalue = sd["keyvalue"].asString();
	printf("%s",sd.getJsonString().c_str());

	vector<string>columnnames = DataBaseUserInfo::getIns()->getTableColumnName(tname);
	int sz1 = columnnames.size();
	int err = 0;
	
	map<string, string>maps;
	for (int i = 0; i < sz1; i++){
		string coname = columnnames.at(i);
		if (sd.isMember(coname)){
			string covalue = sd[coname].asString();
			maps.insert(make_pair(coname, covalue));
		}
	}
	err = DataBaseUserInfo::getIns()->updateDBDataByKey(tname, maps, key, keyvalue);
	
	
	YMSocketData sd1;
	sd1["err"] = err;
	sd1["tname"] = tname;
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlBackup(string dbname, char *&buff, int &sz){
	string sqlstr = "backup database " + dbname + " to disk='d://sqlbackup/" + dbname + ".bak'";
	vector<string> vecs;
	SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(), vecs, backup_sql);
	YMSocketData sd1;
	sd1["err"] = 0;
	sd1.serializer(buff, &sz);
}

void HttpLogic::getGateData(YMSocketData sd1, char *&buff, int &sz){
	int type = sd1["type"].asInt();
	int cmd = sd1["cmd"].asInt();
	YMSocketData sd;
	int err = 0;
	
	std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
	std::map<int, Object *>::iterator itr1 = maps.begin();
	GateData *data = NULL;
	for (itr1; itr1 != maps.end(); itr1++){
		GateData *data1 = (GateData *)itr1->second;
		if (data1->_type == type&&!data1->_use){
			sd["servername"] = data1->_name;
			sd["serverip"] = data1->_ip;
			sd["serverport"] = data1->_port;
			data = data1;
			break;
		}
	}
	if (!data){
		err = 1;
	}
	sd["cmd"] = cmd;
	sd["err"] = err;
	sd.serializer(buff, &sz);
}

void HttpLogic::getLogicManagerData(YMSocketData sd1, char *&buff, int &sz){
	int type = sd1["type"].asInt();
	int cmd = sd1["cmd"].asInt();
	YMSocketData sd;
	int err = 0;
	std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_LOGOCMANAGER);
	std::map<int, Object *>::iterator itr1 = maps.begin();
	GateData *data = NULL;
	for (itr1; itr1 != maps.end(); itr1++){
		GateData *data1 = (GateData *)itr1->second;
		if (data1->_type == type&&!data1->_use){
			sd["servername"] = data1->_name;
			sd["serverip"] = data1->_ip;
			sd["serverport"] = data1->_port;
			data = data1;
			break;
		}
	}
	if (!data){
		err = 1;
	}
	sd["cmd"] = cmd;
	sd["err"] = err;
	sd.serializer(buff, &sz);
}

void HttpLogic::getDBData(YMSocketData sd, char *&buff, int &sz){
	string uid=sd["userid"].asString();
	string pass=sd["pwd"].asString();
	int cmd = sd["cmd"].asInt();
	vector<YMSocketData > vec = DataBaseUserInfo::getIns()->getDBData("userinfo","userid",uid);
	for (int i = 0; i < vec.size(); i++){
		YMSocketData sd1 = vec.at(i);
		if (sd1.isMember("pwd")){
			string spwd = sd1["pwd"].asString();
			if (spwd.compare(pass) == 0){
				YMSocketData sd2;
				sd2["err"] = 0;
				sd2["cmd"] = cmd;
				sd2["data"] = sd1;
				sd2.serializer(buff, &sz);
				return;
			}
		}
	}
	YMSocketData sd2;
	sd2["err"] = 1;
	sd2["cmd"] = cmd;
	sd2["data"]["userid"] = uid;
	sd2["data"]["pwd"] = pass;
	sd2.serializer(buff, &sz);
}

void HttpLogic::insertDBData(YMSocketData sd, char *&buff, int &sz){
	string uid = sd["data"]["userid"].asString();
	string pass = sd["data"]["pwd"].asString();
	int cmd = sd["cmd"].asInt();

	YMSocketData sd2;
	sd2["cmd"] = cmd;
	sd2["data"]["userid"] = uid;
	sd2["data"]["pwd"] = pass;
	std::vector<YMSocketData> vec = DataBaseUserInfo::getIns()->getDBData("userinfo", "userid", uid);
	if (vec.empty()){
		bool ist = DataBaseUserInfo::getIns()->insertDBData(sd, "userinfo");
		if (ist){
			std::vector<YMSocketData> vec = DataBaseUserInfo::getIns()->getDBData("userinfo", "userid", uid);
			for (int i = 0; i < vec.size(); i++){
				YMSocketData sd1 = vec.at(i);
				if (sd1.isMember("pwd")){
					string spwd = sd1["pwd"].asString();
					if (spwd.compare(pass) == 0){
						YMSocketData sd2;
						sd2["err"] = 0;
						sd2["cmd"] = cmd;
						sd2["data"] = sd1;
						sd2.serializer(buff, &sz);
						return;
					}
				}
			}
		}
		sd2["err"] = 1;//×¢²áÊ§°Ü
	}
	else{
		sd2["err"] = 2;//ÓÃ»§´æÔÚ
	}
	
	sd2.serializer(buff, &sz);
}