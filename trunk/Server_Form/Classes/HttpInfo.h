#ifndef __HttpInfo_SCENE_H__
#define __HttpInfo_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppMacros.h"
#include "XXHttpRequest.h"
#include "YMSocketData.h"

using namespace cocos2d;
using namespace ui;
using namespace std;


class HttpInfo:public Ref
{
public:
	HttpInfo();
	~HttpInfo();
    virtual bool init();
	static HttpInfo *getIns();
	
	void requestSQLStart(SQLInfo &info);
	void SQLStartCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLClose();
	void SQLCloseCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLExcute(string tablename, map<string, string> vec, string key, string value);
	void SQLExcuteCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLDB(string dbname);
	void SQLDBCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLConnect();
	void SQLConnectCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLColumns(string tablename);
	void SQLLColumnsCallBack(HttpClient* client, HttpResponse* response);

	void requestSQLFindConnect(string tablename,string coname,string covalue);
	void SQLLFindCallBack(HttpClient* client, HttpResponse* response);

	int aes_encrypt(char* in, int inlen, char* key, char* out);
	int aes_decrypt(char* in, int inlen, char* key, char* out);
	
	vector<string> getDatabases(){
		return m_dbs;
	}
	vector<string> getTables(){
		return m_tables;
	}

	void setDBName(string dbname){
		m_curdbname = dbname;
	}
	string getDBName(){
		return m_curdbname;
	}

	void setTableName(string tname){
		m_curtablename = tname;
	}
	string getTableName(){
		return m_curtablename;
	}


	void update(float dt);
	void openUpdate();
	void closeUpdate();
private:
	map<string, string> setDBDataToVec(::google::protobuf::Message* msg,string tname);
	YMSocketData getSocketDataByStr(string str, int sz);
	string getDBDataString(::google::protobuf::Message *data,string tname);
	void HttpSend(YMSocketData sd, SEL_HttpResponse respond);

	::google::protobuf::Message * getDBDataFromSocketData(string tablename, CSJson::Value sd);
	void setDBDataToSocketData(string tablename, ::google::protobuf::Message *user, YMSocketData &sd, string listname = "", int index = 0);

	string encryptStringFromString(string in, int sz);
	string decryptStringFromString(string in, int sz);
public:
	static HttpInfo *m_Ins;
	bool m_isConnetServer;
	vector<string> m_dbs;
	vector<string> m_tables;
	SQLInfo *m_pSQLInfo;
	string m_curdbname;
	string m_curtablename;

	bool m_isopen;
};

#endif 