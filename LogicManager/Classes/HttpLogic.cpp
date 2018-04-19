#include "HttpLogic.h"
#include "CSVDataInfo.h"
#include "HttpEvent.h"
#include "LibEvent.h"
#include "Common.h"
#include "aes.h"


int HttpLogic::SERVER_PORT = 0;
string HttpLogic::SERVER_CODE = "";
HttpLogic *HttpLogic::m_Ins = NULL;


HttpLogic::HttpLogic(){
	
}
HttpLogic::~HttpLogic(){
	
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

void HttpLogic::requestManagerData(){
	string url=sqlhttp;
	YMSocketData sd;
	sd["cmd"] = 0x0A;
	sd["type"] = 3;
	HttpEvent::getIns()->requestData(url,sd);
}

void HttpLogic::ManagerDataCall(YMSocketData sd){
	int err = sd["err"].asInt();
	if (err == 0){
		HttpLogic::SERVER_PORT = sd["port"].asInt();
		HttpLogic::SERVER_CODE = sd["name"].asString().c_str();
		std::cout << "socket start:" << HttpLogic::SERVER_PORT << std::endl;
		LibEvent *clib = LibEvent::getIns();
		clib->StartServer(HttpLogic::SERVER_PORT, 2, 5000, 600, 600);
		getchar();
		clib->StopServer();
	}
	else{
		printf("未获取到数据\n");
	}
}

void HttpLogic::respondleLogic(YMSocketData sd){
	int cmd = sd["cmd"].asInt();
	if (cmd == 0x0A){
		ManagerDataCall(sd);
	}
}

void HttpLogic::HandleLogic(YMSocketData sd, char *&buff, int &sz){
	printf("socketdata:%s",sd.getJsonString().c_str());
}


void HttpLogic::aes_decrypt(char* in, int inlen, char* out)
{
	if (!in || !out) return;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, DECKEY, AES_BLOCK_SIZE);

	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)DECKEY, 128, &aes) < 0)
	{
		return;
	}

	int num = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_DECRYPT);
	inlen = inlen / AES_BLOCK_SIZE*AES_BLOCK_SIZE;
	out[inlen + num] = '\0';

}

void HttpLogic::aes_encrypt(char* in, int inlen, char* out)
{
	if (!in || !out) return;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, DECKEY, AES_BLOCK_SIZE);
	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)DECKEY, 128, &aes) < 0)
	{
		return;
	}
	int num = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_ENCRYPT);
	inlen = inlen / AES_BLOCK_SIZE*AES_BLOCK_SIZE;
	out[inlen + num] = '\0';

}
