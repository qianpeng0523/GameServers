#include "HttpInfo.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "XXIconv.h"
#include "LogoLayer.h"
#include "aes.h"
#include "crypto_wrapper.h"
#include "YMSocketDataEvent.h"
#include "ServerControl.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <ws2tcpip.h>
#else
#include<netdb.h>
#endif

USING_NS_CC;
using namespace cocos2d_xx;

#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"

HttpInfo *HttpInfo::m_Ins = NULL;


HttpInfo::HttpInfo(){
	m_pSQLInfo = new SQLInfo();
	m_isopen = false;
}
HttpInfo::~HttpInfo(){
	delete m_pSQLInfo;
	m_pSQLInfo = NULL;
}

bool HttpInfo::init()
{
	
    return true;
}

HttpInfo *HttpInfo::getIns(){
	if (!m_Ins){
		m_Ins = new HttpInfo();
		m_Ins->init();
	}
	return m_Ins;
}


int HttpInfo::aes_decrypt(char* in, int inlen, char* key, char* out)
{
	if (!in || !key || !out) return 0;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, key, AES_BLOCK_SIZE);

	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
	{
		return 0;
	}

	int num = 0, en_len = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_DECRYPT);

	return num;

}

int HttpInfo::aes_encrypt(char* in, int inlen, char* key, char* out)
{
	if (!in || !key || !out) return 0;
	unsigned char *iv = new unsigned char[AES_BLOCK_SIZE];
	memcpy(iv, key, AES_BLOCK_SIZE);
	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
	{
		return 0;
	}

	int num = 0, en_len = 0;
	AES_cfb128_encrypt((unsigned char*)in, (unsigned char*)out, inlen, &aes, iv, &num, AES_ENCRYPT);

	return num;

}


void HttpInfo::requestSQLStart(SQLInfo &info){
	HttpInfo::getIns()->m_pSQLInfo->Copy(info);
	YMSocketData sd;
	sd["cmd"] = 0x01;
	sd["ip"] = info._ip;
	sd["db"] = info._db;
	sd["name"] = info._name;
	sd["pass"] = info._pass;
	sd["port"] = info._port;

	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLStartCallBack));
}

void HttpInfo::SQLStartCallBack(HttpClient* client, HttpResponse* response){
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	log("SQLStartCallBack:%s",str.c_str());
	bool isc = true;
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器未开启");
		isc = false;
		p->ShowFindBar(false);
		p->ShowFindBg(false);
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s",sd.getJsonString().c_str());
		bool isstart= sd.isMember("err");
		if (isstart){
			int err = sd["err"].asInt();
			if (err == 0){
				LogoLayer::ShowPrintf("连接数据库成功");
				HttpInfo::getIns()->m_curdbname = sd["dbname"].asString();
				HttpInfo::getIns()->m_dbs.clear();
				bool ishave = sd.isMember("dbs");
				if (ishave){
					CSJson::Value lists = sd["dbs"];
					int sz = lists.size();
					for (int i = 0; i < sz; i++){
						string dbname = lists[i].asString();
						HttpInfo::getIns()->m_dbs.push_back(dbname);
					}
					p->ShowDatabase(HttpInfo::getIns()->m_curdbname);

					CSJson::Value tables = sd["tables"];
					int sz1 = tables.size();
					HttpInfo::getIns()->m_tables.clear();
					for (int i = 0; i < sz1; i++){
						string tablename = tables[i].asString();
						HttpInfo::getIns()->m_tables.push_back(tablename);
						if (i == 0){
							HttpInfo::getIns()->m_curtablename = tablename;
							p->ShowTable(tablename);
							HttpInfo::getIns()->requestSQLColumns(tablename);
						}
					}
					if (sz1>0){
						p->ShowFindBar(true);
					}
					else{
						p->ShowFindBar(false);
						p->ShowFindBg(false);
					}
				}
				else{
					p->ShowDatabase(HttpInfo::getIns()->m_pSQLInfo->_db);
					p->ShowTable("暂无表");
					p->ShowFindBar(false);
					p->ShowFindBg(false);
				}


				//requestSQLBackupConnect("majiang");
			}
			else if (err == 1){
				LogoLayer::ShowPrintf("连接数据库失败，请检查配置");
				isc = false;
				
			}
			else if (err == 2){
				LogoLayer::ShowPrintf("数据库已经连接成功");
			}
		}
	}
	if (isc){
		HttpInfo::getIns()->openUpdate();
		p->StartStatus(false);
		p->CloseStatus(true);
		//p->ExcuteStatus(true);
		p->DBStatus(true);
		p->TableStatus(true);
		p->ConnectOprate(true);
	}
	else{
		HttpInfo::getIns()->closeUpdate();
		p->StartStatus(true);
		p->CloseStatus(false);
		p->ExcuteStatus(false);
		p->DBStatus(false);
		p->TableStatus(false);
		p->ConnectOprate(false);
	}
}

void HttpInfo::requestSQLClose(){
	string url = "http://192.168.1.102:8080/";
	YMSocketData sd;
	sd["cmd"] = 0x02;
	
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLCloseCallBack));

}

void HttpInfo::SQLCloseCallBack(HttpClient* client, HttpResponse* response){
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器未开启");
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		if (err == 0){
			LogoLayer::ShowPrintf("关闭数据库成功");
		}
		else if (err == 1){
			LogoLayer::ShowPrintf("服务器未关闭成功（服务器或许未连接）");
		}
		
	}
	
	p->StartStatus(true);
	p->CloseStatus(false);
	p->ExcuteStatus(false);
	p->DBStatus(false);
	p->TableStatus(false);
	p->ShowDatabase(XXIconv::GBK2UTF("未连接"));
	p->ShowTable(XXIconv::GBK2UTF("未连接"));
	HttpInfo::getIns()->closeUpdate();


	p->ShowFindBar(false);
	p->ShowFindBg(false);
	p->ConnectOprate(false);

}

void HttpInfo::requestSQLExcute(string tablename, map<string, string> vec,string key,string value){
	YMSocketData sd;
	sd["cmd"] = 0x03;
	sd["tname"] = tablename;
	sd["key"] = key;
	sd["keyvalue"] = value;
	map<string, string>::iterator itr = vec.begin();
	for (itr; itr!=vec.end();itr++){
		sd[itr->first] = itr->second;
	}
	log("%s",sd.getJsonString());
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLExcuteCallBack));

}

void HttpInfo::SQLExcuteCallBack(HttpClient* client, HttpResponse* response){
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器未开启");
		
		p->StartStatus(true);
		p->CloseStatus(false);
		p->ExcuteStatus(false);
		p->DBStatus(false);
		p->TableStatus(false);
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		if (err == 0){
			LogoLayer::ShowPrintf("操作成功");
			p->resetLastColumns();
		}
		else if (err == 1){
			LogoLayer::ShowPrintf("操作失败");
		}
		
	}
	
}

void HttpInfo::requestSQLDB(string dbname){
	YMSocketData sd;
	sd["cmd"] = 0x04;
	sd["dbname"] = dbname;
	HttpInfo::getIns()->m_curdbname = dbname;
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLDBCallBack));

}

void HttpInfo::SQLDBCallBack(HttpClient* client, HttpResponse* response){
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	bool isc = true;
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器未开启");
		isc = false;
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		char buff[100];
		if (err == 0){
			string dbname = sd["dbname"].asString();
			
			sprintf(buff,"选择%s数据库成功",dbname.c_str());
			LogoLayer::ShowPrintf(buff);

			HttpInfo::getIns()->m_curdbname = dbname;
			p->ShowDatabase(HttpInfo::getIns()->m_curdbname);
			HttpInfo::getIns()->m_tables.clear();
			CSJson::Value tables = sd["tables"];
			int sz1 = tables.size();
			if (sz1 > 0){
				for (int i = 0; i < sz1; i++){
					string tablename = tables[i].asString();
					HttpInfo::getIns()->m_tables.push_back(tablename);
					if (i == 0){
						p->ShowTable(tablename);
						HttpInfo::getIns()->requestSQLColumns(tablename);
					}
				}
				p->ShowFindBar(true);
			}else{
				p->ShowTable(XXIconv::GBK2UTF("暂无表"));
				p->ShowFindBar(false);
				p->ShowFindBg(false);
			}

		}
		else if (err == 1){
			sprintf(buff, "选择%s数据库失败", HttpInfo::getIns()->m_curdbname.c_str());
			LogoLayer::ShowPrintf(buff);
			isc = false;
		}

	}
	if (!isc){
		HttpInfo::getIns()->closeUpdate();
		p->StartStatus(true);
		p->CloseStatus(false);
		p->ExcuteStatus(false);
		p->DBStatus(false);
		p->TableStatus(false);
	}
}

void HttpInfo::requestSQLConnect(){
	YMSocketData sd;
	sd["cmd"] = 0x06;
	
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLConnectCallBack));
}

void HttpInfo::SQLConnectCallBack(HttpClient* client, HttpResponse* response){
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	bool isc = true;
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器已关闭");
		isc = false;
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		
		int err = sd["err"].asInt();
		if (err == 0){
			isc = true;
		}
		else if (err == 1){
			isc = false;
			LogoLayer::ShowPrintf("检测数据库连接失败");
		}
	}
	if (!isc){
		
		p->StartStatus(true);
		p->CloseStatus(false);
		p->ExcuteStatus(false);
		p->DBStatus(false);
		p->TableStatus(false); 
		p->ConnectOprate(false);
		HttpInfo::getIns()->closeUpdate();

		p->ShowFindBar(false);
		p->ShowFindBg(false);
		p->ConnectOprate(false);
		p->ShowDatabase(XXIconv::GBK2UTF("未连接"));
		p->ShowTable(XXIconv::GBK2UTF("未连接"));
	}
}

void HttpInfo::requestSQLColumns(string tablename){
	YMSocketData sd;
	sd["cmd"] = 0x07;
	sd["tname"] = tablename;
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLLColumnsCallBack));
}

void HttpInfo::SQLLColumnsCallBack(HttpClient* client, HttpResponse* response){
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	bool isc = true;
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器已关闭");
		isc = false;
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		string tablename = sd["tname"].asString();
		bool ishave = sd.isMember("columns");
		if (ishave){
			CSJson::Value cocolumns = sd["columns"];
			int sz1 = cocolumns.size();
			vector<string> cos;
			for (int i = 0; i < sz1; i++){
				string co = cocolumns[i].asString();
				cos.push_back(co);
			}
			p->setColumns(cos);
		}
		else{
			p->ShowPrintf(tablename+"表没有列");
		}
	}
}

void HttpInfo::requestSQLFindConnect(string tablename, string coname, string covalue){
	YMSocketData sd;
	sd["cmd"] = 0x08;
	sd["tname"] = tablename;
	sd["coname"] = coname;
	sd["covalue"] = covalue;
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLLFindCallBack));
}

void HttpInfo::SQLLFindCallBack(HttpClient* client, HttpResponse* response){
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	bool isc = true;
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器已关闭");
		isc = false;
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		if (err == 0){
			string tablename = sd["tname"].asString();
			string prikey = sd["prikey"].asString();
			bool ish = sd.isMember("datas");
			if (ish){
				CSJson::Value datas = sd["datas"];
				int sz = datas.size();
				for (int i = 0; i < sz; i++){
					CSJson::Value data = datas[i];
					p->ShowPrintf(getDBDataString(HttpInfo::getIns()->getDBDataFromSocketData(tablename,data), tablename));
					map<string, string>vec = setDBDataToVec(HttpInfo::getIns()->getDBDataFromSocketData(tablename, data), tablename);
					if (i == 0){
						p->ShowDataItems(vec, prikey);
					}
				}
				p->ExcuteStatus(true);
			}
			else{
				LogoLayer::ShowPrintf("未查找到数据");
			}
			
			
		}
		else{
			LogoLayer::ShowPrintf("未查找到数据");
			p->ShowFindBg(false);
		}
	}
}

void HttpInfo::requestSQLBackupConnect(string dbname){
	YMSocketData sd;
	sd["cmd"] = 0x09;
	sd["dbname"] = dbname;
	HttpInfo::getIns()->HttpSend(sd, httpresponse_selector(HttpInfo::SQLBackupCallBack));
}

void HttpInfo::SQLBackupCallBack(HttpClient* client, HttpResponse* response){
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	int sz = 0;
	string str = XXHttpRequest::getIns()->getdata(response, sz);
	bool isc = true;
	if (str.empty()){
		LogoLayer::ShowPrintf("服务器已关闭");
		isc = false;
	}
	else{
		YMSocketData sd = getSocketDataByStr(str, sz);
		log("sd:%s", sd.getJsonString().c_str());
		int err = sd["err"].asInt();
		if (err == 0){
			LogoLayer::ShowPrintf("备份数据库成功");
		}
		else{
			LogoLayer::ShowPrintf("备份数据库失败");
		}
	}
}

map<string, string> HttpInfo::setDBDataToVec(::google::protobuf::Message* msg, string tname){
	map<string, string> vec;
	char buff[30];
	if (tname.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo user;
		user.CopyFrom(*msg);
		vec.insert(make_pair("userid", user.userid()));
		vec.insert(make_pair("username", user.username()));
		sprintf(buff, "%d", user.sex());
		vec.insert(make_pair("sex", buff));
		vec.insert(make_pair("address", user.address()));
		sprintf(buff, "%d", user.gold());
		vec.insert(make_pair("gold", buff));
		sprintf(buff, "%d", user.diamon());
		vec.insert(make_pair("diamond", buff));
		sprintf(buff, "%d", user.card());
		vec.insert(make_pair("card", buff));
		vec.insert(make_pair("code", user.code()));
		vec.insert(make_pair("token", user.token()));
		sprintf(buff, "%d", user.picid());
		vec.insert(make_pair("picid", buff));
		vec.insert(make_pair("unionid", user.unionid()));
		vec.insert(make_pair("picurl", user.picurl()));
	}
	else if (tname.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords rec;
		rec.CopyFrom(*msg);
		sprintf(buff, "%d", rec.id());
		vec.insert(make_pair("id",buff ));
		vec.insert(make_pair("roomid", rec.roomid()));
		sprintf(buff, "%d", rec.rtype());
		vec.insert(make_pair("rtype", buff));
		vec.insert(make_pair("ctime", rec.ctime()));
	}
	else if (tname.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords drec;
		drec.CopyFrom(*msg);
		sprintf(buff, "%d", drec.id());
		vec.insert(make_pair("id", buff));
		sprintf(buff, "%d", drec.fkey());
		vec.insert(make_pair("fkey", buff));
		vec.insert(make_pair("userid",drec.userid()));
		sprintf(buff, "%d", drec.score());
		vec.insert(make_pair("score", buff));
		sprintf(buff, "%d", drec.win());
		vec.insert(make_pair("win", buff));
	}
	else if (tname.compare(MJ_TABLENAME_NOTICE) == 0){
		DBNotice drec;
		drec.CopyFrom(*msg);
		sprintf(buff, "%d", drec.id());
		vec.insert(make_pair("id", buff));
		vec.insert(make_pair("notice1", drec.notice1()));
		vec.insert(make_pair("notice2", drec.notice2()));
	}
	else if (tname.compare(MJ_TABLENAME_ROOM) == 0){
		DBRoom drec;
		drec.CopyFrom(*msg);
		sprintf(buff, "%d", drec.id());
		vec.insert(make_pair("id", buff));
		sprintf(buff, "%d", drec.round());
		vec.insert(make_pair("round", buff));
		vec.insert(make_pair("remark", drec.remark()));
		sprintf(buff, "%d", drec.number());
		vec.insert(make_pair("number", buff));
		sprintf(buff, "%d", drec.piao());
		vec.insert(make_pair("piao", buff));
		sprintf(buff, "%d", drec.laizi());
		vec.insert(make_pair("laizi", buff));
	}
	return vec;
}

YMSocketData HttpInfo::getSocketDataByStr(string str, int sz){
	YMSocketData sd;
	if (str.empty()){
		return sd;
	}
	sd.parse((char *)str.c_str(), sz);

	return sd;
}

void HttpInfo::openUpdate(){
	if (!m_isopen){
		m_isopen = true;
		Director::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HttpInfo::update), this, 8, false);
	}
}

void HttpInfo::closeUpdate(){
	if (m_isopen){
		m_isopen = false;
		Director::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HttpInfo::update), this);
	}
}

void HttpInfo::update(float dt){
	requestSQLConnect();
}

void HttpInfo::HttpSend(YMSocketData sd, SEL_HttpResponse respond){
	SQLInfo *info = HttpInfo::getIns()->m_pSQLInfo;
	string url = "http://";
	url += info->_httpip + ":";
	url += info->_httpport + "/";
	log("url:%s",url.c_str());
	char buff[300];
	int sz = 0;
	sd.serializer(buff, &sz);
	buff[sz] = '\0';

	url += buff;

	XXHttpRequest::getIns()->getServerDataFromUrl(url, respond);
}

string HttpInfo::getDBDataString(::google::protobuf::Message *data, string tname){
	string str;
	if (tname.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo user;
		user.CopyFrom(*data);
		str = user.userid() + "," + user.username() + ",";
		char buff[100];
		sprintf(buff, "%d,", user.sex());
		str += buff + user.address() + ",";
		sprintf(buff, "%d,%d,%d,", user.gold(), user.diamon(), user.card());
		str += buff + user.code() + "," + user.token() + ",";
		sprintf(buff, "%d,", user.picid());
		str += buff + user.unionid() + "," + user.picurl();
	}
	else if (tname.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords user;
		user.CopyFrom(*data);
		char buff[100];
		sprintf(buff, "%d,%s,%d,%s", user.id(), user.roomid().c_str(), user.rtype(), user.ctime().c_str());
		str = buff;
	}
	else if (tname.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords user;
		user.CopyFrom(*data);
		char buff[100];
		sprintf(buff,"%d,%d,%s,%d,%d",user.id(),user.fkey(),user.userid().c_str(),user.score(),user.win());
		str = buff;
	}
	return str;
}

::google::protobuf::Message * HttpInfo::getDBDataFromSocketData(string tablename, CSJson::Value sd){
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		string uid, uname, add, code, token, unionid, picurl;
		int sex, gold, diamond, card, picid;
		uid = sd["userid"].asString();
		uname = sd["username"].asString();
		sex = sd["sex"].asInt();
		add = sd["address"].asString();
		gold = sd["gold"].asInt();
		diamond = sd["diamond"].asInt();
		card = sd["card"].asInt();
		code = sd["code"].asString();
		token = sd["token"].asString();
		picid = sd["picid"].asInt();
		unionid = sd["unionid"].asString();
		picurl = sd["picurl"].asString();

		DBUserInfo user;
		::google::protobuf::Message *user1 = YSEvent::create_message(user.GetTypeName());
		user.set_userid(uid);
		user.set_username(uname);
		user.set_sex(sex);
		user.set_address(add);
		user.set_gold(gold);
		user.set_diamon(diamond);
		user.set_card(card);
		user.set_code(code);
		user.set_token(token);
		user.set_picid(picid);
		user.set_unionid(unionid);
		user.set_picurl(picurl);
		user1->CopyFrom(user);
		return user1;
	}
	else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords record;
		::google::protobuf::Message *record1 = YSEvent::create_message(record.GetTypeName());

		int id, rtype;
		string rid, ctime;

		id = sd["id"].asInt();
		rtype = sd["rtype"].asInt();
		rid = sd["roomid"].asString();
		ctime = sd["ctime"].asString();
		
		record.set_id(id);
		record.set_rtype(rtype);
		record.set_roomid(rid);
		record.set_ctime(ctime);
		record1->CopyFrom(record);
		return record1;
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords de_record;
		::google::protobuf::Message *de_record1 = YSEvent::create_message(de_record.GetTypeName());
		int id, fkey, score, win;
		string uid;

		id = sd["id"].asInt();
		fkey = sd["fkey"].asInt();
		uid = sd["userid"].asString();
		score = sd["score"].asInt();
		win = sd["win"].asInt();
		
		de_record.set_id(id);
		de_record.set_fkey(fkey);
		de_record.set_score(score);
		de_record.set_userid(uid);
		de_record.set_win(win);
		de_record1->CopyFrom(de_record);
		return de_record1;
	}
	else if (tablename.compare(MJ_TABLENAME_NOTICE) == 0){
		DBNotice notice;
		::google::protobuf::Message *noticemsg = YSEvent::create_message(notice.GetTypeName());
		int id=sd["id"].asInt();
		string notice1 = sd["notice1"].asString();
		string notice2 = sd["notice2"].asString();
		notice.set_id(id);
		notice.set_notice1(notice1);
		notice.set_notice2(notice2);
		noticemsg->CopyFrom(notice);
		return noticemsg;
	}
	else if (tablename.compare(MJ_TABLENAME_ROOM) == 0){
		DBRoom room;
		::google::protobuf::Message *room1 = YSEvent::create_message(room.GetTypeName());
		int id = sd["id"].asInt();
		int round = sd["round"].asInt();
		int ante = sd["ante"].asInt();
		string remark = sd["remark"].asString();
		int number = sd["number"].asInt();
		int piao = sd["piao"].asInt();
		int laizi = sd["laizi"].asInt();
		room.set_id(id);
		room.set_round(round);
		room.set_ante(ante);
		room.set_remark(remark);
		room.set_number(number);
		room.set_piao(piao);
		room.set_laizi(laizi);
		room1->CopyFrom(room);
		return room1;
	}
}

void HttpInfo::setDBDataToSocketData(string tablename, ::google::protobuf::Message* data, YMSocketData &sd){
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo user;
		user.CopyFrom(*data);
		string uid = user.userid();
		string uname = user.username();
		int sex = user.sex();
		string add = user.address();
		int gold = user.gold();
		int diamond = user.diamon();
		int card = user.card();
		string code = user.code();
		string token = user.token();
		int picid = user.picid();
		string unionid = user.unionid();
		string picurl = user.picurl();
		sd["userid"] = uid;
		sd["username"] = uname;
		sd["sex"] = sex;
		sd["address"] = add;
		sd["gold"] = gold;
		sd["diamond"] = diamond;
		sd["card"] = card;
		sd["code"] = code;
		sd["token"] = token;
		sd["picid"] = picid;
		sd["unionid"] = unionid;
		sd["picurl"] = picurl;
		
	}
	else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords record;
		record.CopyFrom(*data);
		int id = record.id();
		string rid = record.roomid();
		int rtype = record.rtype();
		string ctime = record.ctime();
		
		sd["id"] = id;
		sd["roomid"] = rid;
		sd["rtype"] = rtype;
		sd["ctime"] = ctime;
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords de_record;
		de_record.CopyFrom(*data);
		int id = de_record.id();
		int fkey = de_record.fkey();
		string uid = de_record.userid();
		int score = de_record.score();
		int win = de_record.win();
		
		sd["id"] = id;
		sd["fkey"] = fkey;
		sd["userid"] = uid;
		sd["score"] = score;
		sd["win"] = win;
	}
}

string HttpInfo::encryptStringFromString(string in, int sz){
	char *out = new char[4096];
	int num = aes_encrypt((char *)in.c_str(), sz, DECKEY, out);
	out[sz + num] = '\0';
	string ss = out;
	int len = ss.length();
	delete out;
	return ss;
}

string HttpInfo::decryptStringFromString(string in, int sz){
	char *out = new char[4096];
	int nn = aes_decrypt((char *)in.c_str(), sz, DECKEY, out);
	out[sz + nn] = '\0';
	string ss = out;
	int len = ss.length();
	delete out;
	return ss;
}