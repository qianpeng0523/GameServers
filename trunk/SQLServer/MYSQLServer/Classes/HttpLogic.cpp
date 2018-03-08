#include "HttpLogic.h"
#include "SqlControl.h"
#include "DataBaseUserInfo.h"
#include "aes.h"
#include "CSVDataInfo.h"

#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"



HttpLogic *HttpLogic::m_Ins = NULL;


int HttpLogic::aes_decrypt(char* in, int inlen,char* key, char* out)
{
	if (!in || !key || !out) return 0;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, key, AES_BLOCK_SIZE);

	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
	{
		return 0;
	}
	
	int num = 0,en_len=0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_DECRYPT);
	
	return num;

}

int HttpLogic::aes_encrypt(char* in, int inlen, char* key, char* out)
{
	if (!in || !key || !out) return 0;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, key, AES_BLOCK_SIZE);
	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
	{
		return 0;
	}
	
	int num = 0,en_len=0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_ENCRYPT);
	
	return num;

}



HttpLogic::HttpLogic(){
	m_pSQLInfo = new SQLInfo();

	
}
HttpLogic::~HttpLogic(){
	delete m_pSQLInfo;
	m_pSQLInfo = NULL;
}

bool HttpLogic::init()
{
	std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
	std::map<int, Object *>::iterator itr = maps.begin();
	for (itr; itr != maps.end();itr++){
		GateData *data = (GateData *)itr->second;
		string name = data->_name;
		setGateUse(name, false);
	}
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
		getLogicManagerData( buff, sz);
	}
	else if (cmd == 0x0B){
		getGateData(buff, sz);
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

string HttpLogic::encryptStringFromString(string in,int sz){
	char *out = new char[4096];
	int num = aes_encrypt((char *)in.c_str(), sz, DECKEY, out);
	out[sz+num] = '\0';
	string ss = out;
	int len = ss.length();
	delete out;
	return ss;
}

string HttpLogic::decryptStringFromString(string in,int sz){
	char *out = new char[4096];
	int nn = aes_decrypt((char *)in.c_str(), sz, DECKEY, out);
	out[sz + nn] = '\0';
	string ss = out;
	int len = ss.length();
	delete out;
	return ss;
}

void HttpLogic::SqlBackup(string dbname, char *&buff, int &sz){
	string sqlstr = "backup database " + dbname + " to disk='d://sqlbackup/" + dbname + ".bak'";
	vector<string> vecs;
	SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(), vecs, backup_sql);
	YMSocketData sd1;
	sd1["err"] = 0;
	sd1.serializer(buff, &sz);
}

void HttpLogic::setGateUse(string name, bool isUse){
	if (m_gateUse.find(name) != m_gateUse.end()){
		m_gateUse.at(name) = isUse;
	}
	else{
		m_gateUse.insert(make_pair(name,isUse));
	}
}

void HttpLogic::getGateData(char *&buff, int &sz){
	string name;
	map<string, bool>::iterator itr = m_gateUse.begin();
	for (itr; itr != m_gateUse.end();itr++){
		if (!itr->second){
			name = itr->first;
			break;
		}
	}
	YMSocketData sd;
	int err = 0;
	if (!name.empty()){
		std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
		std::map<int, Object *>::iterator itr1 = maps.begin();
		GateData *data = NULL;
		for (itr1; itr1 != maps.end(); itr1++){
			GateData *data1 = (GateData *)itr1->second;
			string name1 = data1->_name;
			if (name.compare(name1) == 0){
				data = data1;
			}
		}
		if (data){
			sd["servername"] = data->_name;
			sd["serverip"] = data->_ip;
			sd["serverport"] = data->_port;
		}
		else{
			err = 2;
		}
	}
	else{
		err = 1;
	}
	sd["err"] = err;
	sd.serializer(buff, &sz);
}

void HttpLogic::getLogicManagerData(char *&buff, int &sz){
	YMSocketData sd;
	int err = 0;
	
	std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
	std::map<int, Object *>::iterator itr1 = maps.begin();
	GateData *data = NULL;
	for (itr1; itr1 != maps.end(); itr1++){
		data = (GateData *)itr1->second;
		sd["servername"] = data->_name;
		sd["serverip"] = data->_ip;
		sd["serverport"] = data->_port;
	}
	if (!data){
		err = 1;
	}
	
	sd["err"] = err;
	sd.serializer(buff, &sz);
}
