#ifndef __XXHttpRequest_H__
#define __XXHttpRequest_H__

#include "cocos2d.h"
#include "spine/extension.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "ExtensionMacros.h"
#include "YMSocketData.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
using namespace network;

class YMSocketData;

struct HeroItem{
	string name;
	string pass;
	string sex;
	string token;
	string headid;
	string dearname;
	string id;
};

struct ServerItem:public Object{
	//"name":"FX²âÊÔ·þ1","host":null,"iP":"10.0.0.2","port":2050,"destID":0
	string name;
	//string host;
	string ip;
	int port;
	int destID;
	int serverStats;
	int serverNew;
};

class XXHttpRequest : public Object
{
public:
	XXHttpRequest();
	~XXHttpRequest();
	bool init();
	static XXHttpRequest* getIns();
	void postServerDataFromUrl(string url, SEL_HttpResponse respond);
	void getServerDataFromUrl(string url, SEL_HttpResponse respond);
	string getdata(HttpResponse* response,int &psize);
	
	void StringReplace(string &strBase, string strSrc, string strDes);
	string getRequstData(Dictionary *dict);
	
	int getDataSize(HttpResponse* response);
	YMSocketData getSocketDataByStr(string str,int sz);

	
private:
	static XXHttpRequest* m_ins;
	static bool m_init;

	
};
#endif