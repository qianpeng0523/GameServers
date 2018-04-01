#include "HttpLogic.h"
#include "redis.h"
#include "aes.h"
#include "CSVDataInfo.h"
#include "StatTimer.h"




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
	CSVDataInfo::getIns()->openCSVFile("res/gateserver.csv", CSV_GATESERVER);
	CSVDataInfo::getIns()->openCSVFile("res/logicmanager.csv", CSV_LOGOCMANAGER);

	std::map<int, Object *> maps = CSVDataInfo::getIns()->getDatas(CSV_GATESERVER);
	std::map<int, Object *>::iterator itr = maps.begin();
	int index[2] = { 0 ,0};
	char buff[50];
	for (itr; itr != maps.end(); itr++){
		GateData *data = (GateData *)itr->second;
		char *dd = data->getData();
		int type = data->_type;
		sprintf(buff,"gate%d_%d",type,index[type-1]);
		int len = 0;
		char *v = redis::getIns()->get(buff,len);
		if (!v){
			redis::getIns()->set(buff, dd);
		}
		else{
			for (int i = 0; i < len; i++){
				if (v[i] == ZERO_STR){
					v[i] = '\0';
				}
			}
			GateData *data1 = (GateData *)v;
			data1->DebugPrintf();
			delete v;
		}
		index[type-1]++;
	}

	std::map<int, Object *> maps1 = CSVDataInfo::getIns()->getDatas(CSV_LOGOCMANAGER);
	std::map<int, Object *>::iterator itr1 = maps1.begin();
	for (itr1; itr1 != maps1.end(); itr1++){
		GateData *data = (GateData *)itr1->second;
		char *dd = data->getData();
		sprintf(buff, "logicmanager%d",data->_type);
		int len = 0;
		char *v = redis::getIns()->get(buff, len);
		if (!v){
			redis::getIns()->set(buff, dd);
		}
		else{
			for (int i = 0; i < len; i++){
				if (v[i] == ZERO_STR){
					v[i] = '\0';
				}
			}
			GateData *data1 = (GateData *)v;
			data1->DebugPrintf();
			delete v;
		}
	}
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
	printf("sd:%s\n",sd.getJsonString().c_str());
	int cmd = sd["cmd"].asInt();
	if (cmd == 0x01){
		SqlStart(sd,buff,sz);
	}
	else if (cmd==0x02){
		SqlClose(buff,sz);
	}
	else if (cmd == 0x0A){
		getLogicManagerData(sd, buff, sz);
	}
	else if (cmd == 0x0B){
		getGateData(sd, buff, sz);
	}
}

void HttpLogic::SqlConnect(char *&buff, int &sz){
	YMSocketData sd1;
	sd1["err"] = !redis::getIns()->isConnect();
	sd1.serializer(buff, &sz);
}

void HttpLogic::SqlStart(YMSocketData sd, char *&buff, int &sz){
	printf("%s\n",sd.getJsonString().c_str());
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

void HttpLogic::SqlClose(char *&buff, int &sz){
	redis::getIns()->release();
	YMSocketData sd1;
	sd1["err"] = 0;
	sd1.serializer(buff, &sz);
}

void HttpLogic::ZeroChange(char *&data, int sz){
	for (int i = 0; i < sz;i++){
		if (data[i] == ZERO_STR){
			data[i] = '\0';
		}
	}
}

void HttpLogic::getGateData(YMSocketData sd1, char *&buff, int &sz){
	int type = sd1["type"].asInt();
	int cmd = sd1["cmd"].asInt();
	YMSocketData sd;
	int err = 0;
	int len;
	char buff1[50];
	sprintf(buff1,"gatetype%d",type);
	char *gatetype = redis::getIns()->get(buff1,len);
	int gtype = -1;
	if (gatetype == NULL){
		gtype = 0;
		redis::getIns()->set(buff1,"0");
	}
	else{
		gtype = atoi(gatetype);
		delete gatetype;
	}
	sprintf(buff1,"gate%d_%d",type,gtype);

	char *dd = (char *)redis::getIns()->get(buff1,len);
	ZeroChange(dd,len);
	GateData *data = (GateData *)dd;
	if (!data){
		err = 1;
	}
	else{
		sd["ip"] = data->_ip;
		sd["port"] = data->_port;
		sd["name"] = data->_name;
		delete dd;
	}
	sd["cmd"] = cmd;
	sd["err"] = err;
	sd["type"] = type;
	sd.serializer(buff, &sz);
}

void HttpLogic::getLogicManagerData(YMSocketData sd1, char *&buff, int &sz){
	int type = sd1["type"].asInt();
	int cmd = sd1["cmd"].asInt();
	YMSocketData sd;
	int err = 0;
	
	int len;
	char buff1[50];
	sprintf(buff1, "logicmanager%d", type);
	char *dd= redis::getIns()->get(buff1, len);
	ZeroChange(dd, len);
	GateData *data = (GateData *)dd;
	if (!data){
		err = 1;
	}
	else{
		sd["ip"] = data->_ip;
		sd["port"] = data->_port;
		sd["name"] = data->_name;
		delete dd;
	}
	sd["cmd"] = cmd;
	sd["err"] = err;
	sd["type"] = type;
	sd.serializer(buff, &sz);
}