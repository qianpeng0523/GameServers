#include "HttpLogic.h"
#include "redis.h"
#include "aes.h"
#include "CSVDataInfo.h"
#include "StatTimer.h"
#include "RedisGet.h"



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

void HttpLogic::update(float dt){
	if (!redis::getIns()->isConnect()){
		redis::getIns()->reconnect();
	}
}

HttpLogic::HttpLogic(){
	m_pSQLInfo = new SQLInfo();
	StatTimer::getIns()->scheduleSelector(this, schedule_selector(HttpLogic::update), 8.0);
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
	CLog::log("sd:%s\n",sd.getJsonString().c_str());
	CSJson::ValueType ise = sd.type();
	if (ise == CSJson::ValueType::objectValue&&sd.isMember("cmd")){
		int cmd = sd["cmd"].asInt();
		if (cmd == 0x01){
			SqlStart(sd, buff, sz);
		}
		else if (cmd == 0x02){
			SqlClose(buff, sz);
		}
		else if (cmd == 0x0A){
			getLogicManagerData(sd, buff, sz);
		}
		else if (cmd == 0x0B){
			getGateData(sd, buff, sz);
		}
	}
}

void HttpLogic::SqlConnect(char *&buff, int &sz){
	YMSocketData sd1;
	sd1["err"] = !redis::getIns()->isConnect();
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlStart(YMSocketData sd, char *&buff, int &sz){
	CLog::log("%s\n", sd.getJsonString().c_str());
	if (sd.isMember("ip") && sd.isMember("name") && sd.isMember("pass") && sd.isMember("db") && sd.isMember("port")){
		m_pSQLInfo->_ip = sd["ip"].asString();
		m_pSQLInfo->_name = sd["name"].asString();
		m_pSQLInfo->_pass = sd["pass"].asString();
		m_pSQLInfo->_db = sd["db"].asString();
		m_pSQLInfo->_port = sd["port"].asInt();
		bool isc = redis::getIns()->isConnect();
		bool ist = true;
		if (!isc){
			ist = redis::getIns()->initial(m_pSQLInfo->_ip, m_pSQLInfo->_port, m_pSQLInfo->_pass);
		}
		YMSocketData sd1;
		if (ist){
			sd1["err"] = 0;
			SQLInfo *p = HttpLogic::getIns()->getSQLInfo();
			sd1["dbname"] = p->_db;
		}
		else{
			sd1["err"] = 1;
		}
		sd1.serializer(buff, &sz);
	}
}

void HttpLogic::SqlClose(char *&buff, int &sz){
	redis::getIns()->release();
	YMSocketData sd1;
	sd1["err"] = 0;
	sd1.serializer(buff, &sz);
}

void HttpLogic::getGateData(YMSocketData sd1, char *&buff, int &sz){
	if (sd1.isMember("type") && sd1.isMember("cmd")){
		int type = sd1["type"].asInt();
		int cmd = sd1["cmd"].asInt();
		YMSocketData sd;
		int err = 0;
		GateData *data = RedisGet::getIns()->getGateData(g_redisdbnames[REIDS_GATE]+"_gate",type);
		if (!data){
			err = 1;
		}
		else{
			sd["ip"] = data->_ip;
			sd["port"] = data->_port;
			sd["name"] = data->_name;
		}
		sd["cmd"] = cmd;
		sd["err"] = err;
		sd["type"] = type;
		sd.serializer(buff, &sz);
	}
}

void HttpLogic::getLogicManagerData(YMSocketData sd1, char *&buff, int &sz){
	if (sd1.isMember("type") && sd1.isMember("cmd")){
		int type = sd1["type"].asInt();
		int cmd = sd1["cmd"].asInt();
		YMSocketData sd;
		int err = 0;

		GateData *data = RedisGet::getIns()->getGateData(g_redisdbnames[REIDS_GATE] + "_logicmanager",type);
		if (!data){
			err = 1;
		}
		else{
			sd["ip"] = data->_ip;
			sd["port"] = data->_port;
			sd["name"] = data->_name;
		}
		sd["cmd"] = cmd;
		sd["err"] = err;
		sd["type"] = type;
		sd.serializer(buff, &sz);
	}
}