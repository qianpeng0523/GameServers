
#include "RedisGet.h"
#include "RedisPut.h"
#include "CSVDataInfo.h"
#include "LoginInfo.h"
#include "StatTimer.h"

RedisGet *RedisGet::m_ins=NULL;
RedisGet::RedisGet(){
	m_pFirstBuyItem = NULL;
	m_redis = redis::getIns();
}

RedisGet::~RedisGet(){
	StatTimer::getIns()->unscheduleSelector(this, schedule_selector(RedisGet::update));
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RedisGet *RedisGet::getIns(){
	if (!m_ins){
		m_ins = new RedisGet();
	}
	return m_ins;
}

void RedisGet::init(){
	getREDISDBNames();
	getProp();
	getReward();
	getUserBases();
	getShop();
	getTask();
	getSignZhuan();
	getSignAward();
	getSignStatuss();
	getExAward();
	getActives();
	getFree();
	getCSVExchangeCode();
	//getExcode();
	getFirstBuy();
	getPass();
	getFriend();
	getOpenids();
	getSConfig();
	getUserLoginTimes();
	LoginInfo::getIns()->onLineUsers();
	getFriendNotices();
	getRanks();
	getMails();
	getTaskStatus();
	getFreeStatus();
	getExRecords();
	getPayRecords();
	getAliPayNoDatas();
	
	StatTimer::getIns()->scheduleSelector(this, schedule_selector(RedisGet::update), 60,60);
}

map<string, REDISDBName *> RedisGet::getREDISDBNames(){
	if (!m_RedisDBNames.empty()){
		return m_RedisDBNames;
	}
	vector<string> vecs = m_redis->getListStr("redisname");
	for (int i = 0; i < vecs.size(); i++){
		string con = vecs.at(i);
		vector<string> vv = CSVDataInfo::getIns()->getStrFromstr(con, ",");
		REDISDBName *p = new REDISDBName();
		p->_name = vv.at(0);
		p->_dbindex = atoi(vv.at(1).c_str());
		m_RedisDBNames.insert(make_pair(p->_name, p));
	}
	return m_RedisDBNames;
}

int RedisGet::getRedisDBIndex(string name){
	if (m_RedisDBNames.find(name) != m_RedisDBNames.end()){
		return m_RedisDBNames.at(name)->_dbindex;
	}
	return -1;
}

bool RedisGet::SelectDB(REDISTYPE type){
// 	string dbname = g_redisdbnames[type];
// 	int index = /*getRedisDBIndex(dbname)*/0;
// 	return m_redis->SelectDB(index);
	return true;
}


map<string, SConfig *> RedisGet::getSConfig(){
	if (!m_pSConfigs.empty()){
		return m_pSConfigs;
	}
	bool ist = RedisGet::getIns()->SelectDB(REDIS_SCONFIG);
	if (ist){
		auto vec = getUserBases();
		auto itr = vec.begin();
		SConfig sc1;
		for (itr; itr != vec.end();itr++){
			string uid = itr->first;
			SConfig *sc = (SConfig *)m_redis->getHash(g_redisdbnames[REDIS_SCONFIG] + uid, sc1.GetTypeName());
			m_pSConfigs.insert(make_pair(uid,sc));
		}
	}
	return m_pSConfigs;
}

SConfig* RedisGet::getSConfig(string uid){
	SConfig *sc1 = NULL;
	if (m_pSConfigs.find(uid) != m_pSConfigs.end()){
		sc1 = m_pSConfigs.at(uid);
	}
	return sc1;
}

void RedisGet::setSConfig(string uid, SConfig *sc){
	if (m_pSConfigs.find(uid) != m_pSConfigs.end()){
		m_pSConfigs.at(uid) = sc;
	}
	else{
		m_pSConfigs.insert(make_pair(uid, sc));
	}
}


UserBaseMap RedisGet::getUserBases(){
	if (!m_pUserBases.empty()){
		return m_pUserBases;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		string ky = g_redisdbnames[REIDS_USERBASE] + "_user";
		vector<string >keys = m_redis->getKeys(ky,false);
		UserBase ub;
		for (int i = 0; i < keys.size(); i++){
			string key = keys.at(i);
			UserBase *user = (UserBase *)m_redis->getHash(key,ub.GetTypeName());
			if (user){
				string uid = user->userid();
				if (m_pUserBases.find(uid) != m_pUserBases.end()){
					m_pUserBases.at(uid) = user;
				}
				else{
					m_pUserBases.insert(make_pair(uid, user));
				}
				getUserLoginTime(uid);
				getPass(uid);
			}
		}
		return m_pUserBases;
	}
	else{
		return m_pUserBases;
	}
}

UserBase *RedisGet::getUserBase(string uid){
	if (m_pUserBases.find(uid) != m_pUserBases.end()){
		return m_pUserBases.at(uid);
	}
	return NULL;
}

void RedisGet::setUserBase(UserBase *ub){
	string uid = ub->userid();
	if (m_pUserBases.find(uid) != m_pUserBases.end()){
		UserBase *ub1 = m_pUserBases.at(uid);
		ub1->CopyFrom(*ub);
	}
	else{
		UserBase *ub1 = (UserBase *)ccEvent::create_message(ub->GetTypeName());
		ub1->CopyFrom(*ub);
		m_pUserBases.insert(make_pair(uid, ub1));
	}
}

int RedisGet::getUserIndex(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_USERBASE] + "_userindex",len);
		if (!dd){
			dd = "10000000";
			RedisPut::getIns()->PushUserIndex(atoi(dd));
			return atoi(dd);
		}
		RedisPut::getIns()->PushUserIndex(atoi(dd)+1);
		return atoi(dd) + 1;
	}
	return -1;
}

void RedisGet::setUserLoginTime(string uid, time_t t){
	if (m_pUserLoginTime.find(uid) != m_pUserLoginTime.end()){
		m_pUserLoginTime.at(uid) = t;
	}
	else{
		m_pUserLoginTime.insert(make_pair(uid, t));
	}
}

time_t RedisGet::getUserLoginTime(string uid){
	if (m_pUserLoginTime.find(uid) != m_pUserLoginTime.end()){
		return m_pUserLoginTime.at(uid);
	}
	return 0;
}

map<string, time_t> RedisGet::getUserLoginTimes(){
	if (!m_pUserLoginTime.empty()){
		return m_pUserLoginTime;
	}
	else{
		string tt = "logintime";
		auto vec = getUserBases();
		auto itr = vec.begin();
		int len = 0;
		for (itr; itr != vec.end(); itr++){
			string uid = itr->first;
			char *dd = m_redis->get(g_redisdbnames[REIDS_USERBASE] + tt + uid,len);
			if (dd){
				time_t time = atol(dd);
				m_pUserLoginTime.insert(make_pair(uid, time));
			}
		}
	}
	return m_pUserLoginTime;
}

string RedisGet::getPass(string uid){
	if (m_pPass.find(uid) != m_pPass.end()){
		return m_pPass.at(uid);
	}
	return "";
}

map<string, string> RedisGet::getPass(){
	if (m_pPass.empty()){
		bool ist = RedisGet::getIns()->SelectDB(REIDS_PASS);
		if (ist){
			int len = 0;
			auto vecs = getUserBases();
			auto itr = vecs.begin();
			for (itr; itr != vecs.end(); itr++){
				string uid = itr->first;
				char *dd = m_redis->get(g_redisdbnames[REIDS_PASS]+uid, len);
				if (dd){
					m_pPass.insert(make_pair(uid, dd));
				}
			}
		}
		return m_pPass;
	}
	else{
		return m_pPass;
	}
}

void RedisGet::setPass(string uid, string pass){
	if (m_pPass.find(uid) == m_pPass.end()){
		m_pPass.insert(make_pair(uid, pass));
	}
}


map<string, string> RedisGet::getOpenids(){
	if (!m_pOpenids.empty()){
		return m_pOpenids;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PASS);
	if (ist){
		string key = "openid";
		vector<string >keys = m_redis->getKeys(g_redisdbnames[REIDS_PASS]+key,false);
		int len = 0;
		for (int i = 0; i < keys.size(); i++){
			string name = keys.at(i);
			char *dd = m_redis->get(name,len);
			if (dd){
				m_pOpenids.insert(make_pair(name, dd));
			}
		}
	}
	return m_pOpenids;
}

string RedisGet::getOpenidPass(string openid){
	string key = g_redisdbnames[REIDS_PASS] + "openid" + openid;
	if (m_pOpenids.find(key) != m_pOpenids.end()){
		return m_pOpenids.at(key);
	}
	return "";
}

void RedisGet::setOpenidPass(string openid, string uid){
	if (m_pOpenids.find(uid) == m_pOpenids.end()){
		m_pOpenids.insert(make_pair(openid, uid));
	}
}

map<int, vector<Rank *>> RedisGet::getRanks(){
	if (!m_pRanks.empty()){
		return m_pRanks;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_RANK);
	if (ist){
		char buff[50];
		Rank rk;
		for (int i = 1; i <= 2; i++){
			sprintf(buff, "%s%d", g_redisdbnames[REIDS_RANK].c_str(), i);
			vector<Message *>vecs=m_redis->getList(buff,rk.GetTypeName());
			vector<Rank *> rks;
			for (int j = 0; j < vecs.size(); j++){
				Rank *rk1 = (Rank *)vecs.at(j);
				rks.push_back(rk1);
			}
			m_pRanks.insert(make_pair(i, rks));
		}
	}
	return m_pRanks;
}

vector<Rank *> RedisGet::getRank(int type){
	vector<Rank *> vecs;
	if (m_pRanks.find(type)!=m_pRanks.end()){
		return m_pRanks.at(type);
	}
	else{
		m_pRanks.insert(make_pair(type, vecs));
	}
	return vecs;
}

void RedisGet::setRank(Rank *rk, int type){
	vector<Rank *> vecs=getRank(type);
	int sz = vecs.size();
	string uid = rk->info().userid();
	bool ishave = false;
	for (int i = 0; i < sz; i++){
		Rank *rk1 = vecs.at(i);
		if (rk1->info().userid().compare(uid)==0){
			rk1->CopyFrom(*rk);
			ishave = true;
			break;
		}
	}
	if (!ishave){
		Rank *rk1 = (Rank *)ccEvent::create_message(rk->GetTypeName());
		rk1->CopyFrom(*rk);
		vecs.push_back(rk1);
		m_pRanks.at(type) = vecs;
		
	}
}

map<int, ShopItem*> RedisGet::getShop(){
	if (!m_pShopItems.empty()){
		return m_pShopItems;
	}
	
	ShopItem si;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SHOP], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			ShopItem *rkk=(ShopItem *)vv.at(i);
			
			int rid = rkk->prop().rid();
			Reward *rd = rkk->mutable_prop();
			Reward *rd1 = getReward(rid);
			rd->CopyFrom(*rd1);

			int conid = rkk->consume().rid();
			if (conid>0){
				Reward *r = rkk->mutable_consume();
				Reward *r1 = getReward(conid);
				r->CopyFrom(*r1);
			}

			int giveid = rkk->give().rid();
			if (giveid > 0){
				Reward *r = rkk->mutable_give();
				Reward *r1 = getReward(giveid);
				r->CopyFrom(*r1);
			}

			m_pShopItems.insert(make_pair(rkk->id(),rkk));
		}
	}
	return m_pShopItems;
}

ShopItem *RedisGet::getShop(int shopid){
	if (m_pShopItems.find(shopid) != m_pShopItems.end()){
		return m_pShopItems.at(shopid);
	}
	return NULL;
}


FirstBuyItem *RedisGet::getFirstBuy(){
	if (m_pFirstBuyItem){
		return m_pFirstBuyItem;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		std::map<string, string> vv = m_redis->getHash(g_redisdbnames[REIDS_SHOP] + "_firstbuy");
		if (vv.empty()){
			return m_pFirstBuyItem;
		}
		m_pFirstBuyItem = new FirstBuyItem();
		auto itr = vv.begin();
		for (itr; itr != vv.end(); itr++){
			string name = itr->first;
			string value = itr->second;
			if (name.compare("id") == 0){
				m_pFirstBuyItem->_sid = atoi(value.c_str());
			}
			else if (name.compare("rewardid") == 0){
				m_pFirstBuyItem->_rid = CSVDataInfo::getIns()->getIntFromstr(value, "|");
			}
			else if (name.compare("conid") == 0){
				m_pFirstBuyItem->_conid = atoi(value.c_str());
			}
			else if (name.compare("giveid") == 0){
				m_pFirstBuyItem->_giveid = CSVDataInfo::getIns()->getIntFromstr(value, "|");
			}
		}
	}
	return m_pFirstBuyItem;
}


string RedisGet::getNoTime(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_SHOP] + "notime",len);
		if (dd){
			return dd;
		}
	}
	return "";
}

string RedisGet::getAliOuttradeNo(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_SHOP] + "aliouttradeno", len);
		if (dd){
			return dd;
		}
	}
	return "";
}

map<string, AliPayNoData *> RedisGet::getAliPayNoDatas(){
	if (!m_pAliPayNoDatas.empty()){
		return m_pAliPayNoDatas;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		string key = g_redisdbnames[REIDS_SHOP] + "_alipay_no_data";
		vector<string> vecs = m_redis->getListStr(key);
		for (int i = 0; i < vecs.size(); i++){
			string con = vecs.at(i);
			vector<string> convec = CSVDataInfo::getIns()->getStrFromstr(con,",");
			AliPayNoData *p = new AliPayNoData();
			p->_uid = convec.at(0);
			p->_out_trade_no = convec.at(1);
			p->_endtime = atol(convec.at(2).c_str());
			p->_shopid = atoi(convec.at(3).c_str());
			string content = convec.at(4);
			vector<string> mpcontent = CSVDataInfo::getIns()->getStrFromstr(content, "|");
			for (int j = 0; j < mpcontent.size(); j++){
				string mpkeys = mpcontent.at(j);
				vector<string > mpvec = CSVDataInfo::getIns()->getStrFromstr(content, "^");
				p->_maps.insert(make_pair(mpvec.at(0),mpvec.at(1)));
			}
			m_pAliPayNoDatas.insert(make_pair(p->_out_trade_no,p));
		}
	}
	return m_pAliPayNoDatas;
}

AliPayNoData *RedisGet::getAliPayNoData(string out_trade_no){
	if (m_pAliPayNoDatas.find(out_trade_no) != m_pAliPayNoDatas.end()){
		return m_pAliPayNoDatas.at(out_trade_no);
	}
	return NULL;
}

void RedisGet::eraseAliPayNoData(string out_trade_no){
	if (m_pAliPayNoDatas.find(out_trade_no) != m_pAliPayNoDatas.end()){
		AliPayNoData *p = m_pAliPayNoDatas.at(out_trade_no);
		m_pAliPayNoDatas.erase(m_pAliPayNoDatas.find(out_trade_no));
		delete p;
		p = NULL;
	}
}

void RedisGet::setAliPayNoData(AliPayNoData *p){
	string out_trade_no = p->_out_trade_no;
	if (m_pAliPayNoDatas.find(out_trade_no) == m_pAliPayNoDatas.end()){
		m_pAliPayNoDatas.insert(make_pair(out_trade_no,p));
	}
}

map<string, map<string, PayRecord *>> RedisGet::getPayRecords(){
	if (!m_pPayRecords.empty()){
		return m_pPayRecords;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		PayRecord pr;
		auto users = getUserBases();
		auto itr = users.begin();
		for (itr; itr != users.end();itr++){
			string uid = itr->first;
			string key = g_redisdbnames[REIDS_SHOP] + "_PayRecord_" + uid;
			auto vec = m_redis->getList(key, pr.GetTypeName());
			for (int i = 0; i < vec.size();i++){
				PayRecord *p = (PayRecord *)vec.at(i);
				setPayRecord(p);
			}
			redis::getIns()->releaseMessages(vec);
		}
	}
	return m_pPayRecords;
}

string RedisGet::getWXNonceid(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
		if (ist){
			string key = g_redisdbnames[REIDS_SHOP] + "_nonceid";
			int len = 0;
			char *dd = m_redis->get(key,len);
			if (!dd){
				dd = INITNONCEID;
			}
			RedisPut::getIns()->setWXNonceid(atoi(dd)+1);
			return dd;
		}
	}
	return "";
}

void RedisGet::setWXNonceid(int nonceid){
	m_wxnonceid = nonceid;
}

map<string, WXPayNoData *> RedisGet::getWXPayNoDatas(){
	if (!m_pWXPayNoDatas.empty()){
		return m_pWXPayNoDatas;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		string key = g_redisdbnames[REIDS_SHOP] + "_wxouttradeno";
		vector<string> convecs = m_redis->getListStr(key);
		for (int i = 0; i < convecs.size();i++){
			string content = convecs.at(i);
			vector<string> vec = CSVDataInfo::getIns()->getStrFromstr(content,",");
			WXPayNoData *p = new WXPayNoData();
			p->_uid = vec.at(i);
			p->_endtime = atol(vec.at(1).c_str());
			p->_out_trade_no = vec.at(2);
			m_pWXPayNoDatas.insert(make_pair(p->_out_trade_no,p));
		}
	}
	return m_pWXPayNoDatas;
}

WXPayNoData *RedisGet::getWXPayNoData(string outno){
	if (m_pWXPayNoDatas.find(outno) != m_pWXPayNoDatas.end()){
		return m_pWXPayNoDatas.at(outno);
	}
	return NULL;
}

void RedisGet::setWXPayNoData(WXPayNoData *p){
	if (m_pWXPayNoDatas.find(p->_out_trade_no) == m_pWXPayNoDatas.end()){
		m_pWXPayNoDatas.insert(make_pair(p->_out_trade_no, p));
	}
}

void RedisGet::eraseWXPayNoData(WXPayNoData *p){
	if (m_pWXPayNoDatas.find(p->_out_trade_no) != m_pWXPayNoDatas.end()){
		m_pWXPayNoDatas.erase(m_pWXPayNoDatas.find(p->_out_trade_no));
	}
}

void RedisGet::setPayRecord(PayRecord *pr){
	string uid = pr->userid();
	string outno = pr->out_trade_no();
	if (m_pPayRecords.find(uid) == m_pPayRecords.end()){
		map<string, PayRecord *> mps;
		PayRecord *pr1 = (PayRecord *)ccEvent::create_message(pr->GetTypeName());
		pr1->CopyFrom(*pr);
		mps.insert(make_pair(outno,pr1));
		m_pPayRecords.insert(make_pair(uid,mps));
	}
	else{
		map<string, PayRecord *> mps=m_pPayRecords.at(uid);
		if (mps.find(outno) == mps.end()){
			PayRecord *pr1 = (PayRecord *)ccEvent::create_message(pr->GetTypeName());
			pr1->CopyFrom(*pr);
			mps.insert(make_pair(outno,pr1));
			m_pPayRecords.at(uid) = mps;
		}
	}
}

map<string, map<int, Mail *>> RedisGet::getMails(){
	if (!m_pMails.empty()){
		return m_pMails;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_MAIL);
	if (ist){
		Mail ml;
		auto vecs = getUserBases();
		auto itr = vecs.begin();
		for (itr; itr != vecs.end(); itr++){
			string uid = itr->first;
			vector<string> keys = m_redis->getKeys(g_redisdbnames[REIDS_MAIL] + uid, false);
			map<int, Mail *> maps;
			for (int i = 0; i < keys.size(); i++){
				string key = keys.at(i);
				Mail *ml1 = (Mail *)m_redis->getHash(key, ml.GetTypeName());
// 
// 				for (int j = 0; j < ml1->rewardlist_size(); j++){
// 					int rid = ml1->rewardlist(j).rid();
// 					Reward *rd = ml1->add_rewardlist();
// 					Reward *rd1 = getReward(rid);
// 					rd->CopyFrom(*rd1);
// 				}

				int eid = ml1->eid();
				maps.insert(make_pair(eid, ml1));
			}
			m_pMails.insert(make_pair(uid,maps));
		}
	}
	return m_pMails;
}

map<int, Mail *> RedisGet::getMail(string uid){
	map<int, Mail *> vecs;
	if (m_pMails.find(uid) != m_pMails.end()){
		return m_pMails.at(uid);
	}
	else{
		m_pMails.insert(make_pair(uid, vecs));
	}
	
	return vecs;
}

Mail *RedisGet::getMail(string uid, int eid){
	map<int, Mail *> vec=getMail(uid);
	if (vec.find(eid) != vec.end()){
		return vec.at(eid);
	}
	return	NULL;
}

void RedisGet::setMail(string uid, Mail *ml){
	map<int, Mail *> vec = getMail(uid);
	int mid = ml->eid();
	if (vec.find(mid) != vec.end()){
		Mail *ml1 = vec.at(mid);
		ml1->CopyFrom(*ml);
	}
	else{
		Mail *ml1 = (Mail *)ccEvent::create_message(ml->GetTypeName());
		ml1->CopyFrom(*ml);
		vec.insert(make_pair(ml1->eid(), ml1));
		m_pMails.at(uid) = vec;
	}
}

void RedisGet::eraseMail(string uid, Mail *ml){
	map<int, Mail *> vec = getMail(uid);
	int mid = ml->eid();
	if (vec.find(mid) != vec.end()){
		Mail *ml1 = vec.at(mid);
		vec.erase(vec.find(mid));
		delete ml1;
		ml1 = NULL;
		m_pMails.at(uid) = vec;
	}
	
}

FriendMap RedisGet::getFriend(){
	if (!m_pfriends.empty()){
		return m_pfriends;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		auto maps = getUserBases();
		auto itr = maps.begin();
		for (itr; itr != maps.end(); itr++){
			string uid = itr->first;
			map<string, Friend *>vec;
			vector<string> vv = m_redis->getKeys(g_redisdbnames[REIDS_FRIEND] + uid, false);
			Friend fr;
			for (int i = 0; i < vv.size(); i++){
				string key = vv.at(i);
				Friend *fd = (Friend *)m_redis->getHash(key, fr.GetTypeName());
				vec.insert(make_pair(fd->info().userid(), fd));
			}
			if (!vec.empty()){
				m_pfriends.insert(make_pair(uid, vec));
			}
		}
	}
	return m_pfriends;
}

map<string, Friend *> RedisGet::getFriend(string uid){
	if (m_pfriends.find(uid)!=m_pfriends.end()){
		return m_pfriends.at(uid);
	}
	map<string, Friend *> vec;
	return vec;
}

Friend *RedisGet::getFriend(string uid, string fuid){
	auto vec = getFriend(uid);
	if (vec.find(fuid) != vec.end()){
		return vec.at(fuid);
	}
	return NULL;
}

void RedisGet::setFriend(string uid, Friend *fri, bool isadd){
	string fuid = fri->info().userid();
	if (m_pfriends.find(uid) != m_pfriends.end()){
		auto vec = m_pfriends.at(uid);
		if (!isadd){
			if (vec.find(fuid) != vec.end()){
				vec.erase(vec.find(fuid));
				RedisPut::getIns()->eraseFriend(uid, fri);
			}
		}
		else{
			if (vec.find(fuid) == vec.end()){
				vec.insert(make_pair(fuid,fri));
				m_pfriends.at(uid) = vec;
				//RedisPut::getIns()->PushFriend(uid, fri);
			}
		}
	}
	else{
		if (isadd){
			map<string,Friend *>vec;
			UserBase *ub = getUserBase(fuid);
			if (ub){
				vec.insert(make_pair(fuid, fri));
				m_pfriends.insert(make_pair(uid, vec));
			}
		}
	}
}

map<int, FriendNotice *> RedisGet::getFriendNotice(string uid){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		return m_pFriendNotices.at(uid);
	}
	return getFriendNoticeB(uid);
}

map<int, FriendNotice *> RedisGet::getFriendNoticeB(string uid){
	map<int, FriendNotice *> vecs;
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		FriendNotice si;
		vector<string> keys = m_redis->getKeys(g_redisdbnames[REIDS_FRIEND] + "notice" + uid, false);
		for (int i = 0; i < keys.size(); i++){
			string key = keys.at(i);
			FriendNotice *fn = (FriendNotice *)m_redis->getHash(key, si.GetTypeName());
			vecs.insert(make_pair(fn->nid(),fn));
		}
	}
	return vecs;
}

FriendNotice *RedisGet::getFriendNotice(string uid, string fuid){
	auto vec = getFriendNotice(uid);
	auto itr = vec.begin();
	for (itr; itr != vec.end(); itr++){
		FriendNotice *p = itr->second;
		if (p->uid().compare(fuid) == 0){
			return p;
		}
	}
	return NULL;
}

FriendNotice *RedisGet::getFriendNotice(string uid, int nid){
	auto vec = getFriendNotice(uid);
	if (vec.find(nid) != vec.end()){
		return vec.at(nid);
	}
	return NULL;
}

map<string, map<int, FriendNotice *>> RedisGet::getFriendNotices(){
	if (!m_pFriendNotices.empty()){
		return m_pFriendNotices;
	}
	auto vecs = getUserBases();
	auto itr = vecs.begin();
	for (itr; itr != vecs.end(); itr++){
		string uid = itr->first;
		map<int, FriendNotice *> vec = getFriendNoticeB(uid);
		m_pFriendNotices.insert(make_pair(uid, vec));
	}
	return m_pFriendNotices;
}

void RedisGet::setFriendNotice(string uid, FriendNotice *p){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		auto vec = m_pFriendNotices.at(uid);
		if (vec.find(p->nid()) == vec.end()){
			FriendNotice *p1 = (FriendNotice *)ccEvent::create_message(p->GetTypeName());
			p1->CopyFrom(*p);
			vec.insert(make_pair(p1->nid(),p1));
		}
		else{
			vec.at(p->nid())->CopyFrom(*p);
		}
		m_pFriendNotices.at(uid) = vec;
	}
	else{
		map<int,FriendNotice *> vec;
		FriendNotice *p1 = (FriendNotice *)ccEvent::create_message(p->GetTypeName());
		p1->CopyFrom(*p);
		vec.insert(make_pair(p1->nid(),p1));
		m_pFriendNotices.insert(make_pair(uid, vec));
	}
}

void RedisGet::eraseFriendNotice(string uid, FriendNotice *p){
	if (m_pFriendNotices.find(uid) != m_pFriendNotices.end()){
		m_pFriendNotices.erase(m_pFriendNotices.find(uid));
		delete p;
		p = NULL;
	}
}

map<int, Active *> RedisGet::getActive(int type){
	if (m_pActives.find(type)!=m_pActives.end()){
		return m_pActives.at(type);
	}
	map<int, Active *> vecs;
	return vecs;
}

map<int, map<int, Active *>> RedisGet::getActives(){
	if (!m_pActives.empty()){
		return m_pActives;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_ACTIVE);
	if (ist){
		Active si;
		for (int i = 1; i <= 2; i++){
			char buff[100];
			sprintf(buff, "%s%d", g_redisdbnames[REIDS_ACTIVE].c_str(), i);
			map<int, Active *> acs;
			std::vector<Message *> vv = m_redis->getList(buff, si.GetTypeName());
			for (int j = 0; j < vv.size(); j++){
				Active *rkk = (Active *)vv.at(j);
				acs.insert(make_pair(rkk->id(),rkk));
			}
			m_pActives.insert(make_pair(i,acs));
		}
	}
	return m_pActives;
}

map<int, Task *> RedisGet::getTask(){
	if (!m_tasks.empty()){
		return m_tasks;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_TASK);
	if (ist){
		Task si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_TASK], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Task *rkk = (Task *)vv.at(i);
			m_tasks.insert(make_pair(rkk->taskid(),rkk));
		}
	}
	return m_tasks;
}

Task *RedisGet::getTask(int tid){
	if (m_tasks.find(tid) != m_tasks.end()){
		return m_tasks.at(tid);
	}
	return NULL;
}

Status *RedisGet::getTaskStatus(string uid, int taskid){
	auto vec = getTaskStatus(uid);
	if (vec.find(taskid) != vec.end()){
		return vec.at(taskid);
	}
	return NULL;
}

map<string, map<int, Status *>> RedisGet::getTaskStatus(){
	if (!m_taskstatus.empty()){
		return m_taskstatus;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_TASK);
	if (ist){
		Status st;
		auto users = getUserBases();
		auto useritr = users.begin();
		for (useritr; useritr != users.end();useritr++){
			string uid = useritr->first;
			auto vecs = getTask();
			auto itr = vecs.begin();
			map<int, Status *> maps;
			for (itr; itr != vecs.end(); itr++){
				int tid = itr->first;
				char buff[30];
				sprintf(buff, "%sstatus%d", (g_redisdbnames[REIDS_TASK] + uid).c_str(), tid);
				Status *st1 = (Status *)m_redis->getHash(buff, st.GetTypeName());
				maps.insert(make_pair(tid,st1));
			}
			m_taskstatus.insert(make_pair(uid, maps));
		}
	}
	return m_taskstatus;
}

map<int, Status *> RedisGet::getTaskStatus(string uid){
	if (m_taskstatus.find(uid) != m_taskstatus.end()){
		return m_taskstatus.at(uid);
	}
	map<int, Status *> vec;
	return vec;
}

void RedisGet::setTaskStatus(string uid, int taskid, Status *ss){
	Status *st=NULL;
	if (m_taskstatus.find(uid) != m_taskstatus.end()){
		map<int, Status *> *vec = &(m_taskstatus.at(uid));
		if (vec->find(taskid) != vec->end()){
			st = vec->at(taskid);
			st->CopyFrom(*ss);
		}
		else{
			Status *st = (Status *)ccEvent::create_message(ss->GetTypeName());
			st->CopyFrom(*ss);
			vec->insert(make_pair(taskid,st));
		}
	}
	else{
		Status *st = (Status *)ccEvent::create_message(ss->GetTypeName());
		st->CopyFrom(*ss);
		map<int, Status *> vec;
		vec.insert(make_pair(taskid,st));
		m_taskstatus.insert(make_pair(uid, vec));
	}
}

map<int, ExAward *> RedisGet::getExAward(){
	if (!m_pExAwards.empty()){
		return m_pExAwards;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		ExAward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_EXCHANGE] + "award", si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			ExAward *rkk=(ExAward *)vv.at(i);
			m_pExAwards.insert(make_pair(rkk->eid(), rkk));
		}
	}
	return m_pExAwards;
}


ExAward *RedisGet::getExAward(int id){
	if (m_pExAwards.find(id) != m_pExAwards.end()){
		return m_pExAwards.at(id);
	}
	return NULL;
}


map<int, Reward *> RedisGet::getReward(){
	if (!m_pRewards.empty()){
		return m_pRewards;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_REWARD);
	if (ist){
		Reward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_REWARD], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Reward *rkk = (Reward *)vv.at(i);
			m_pRewards.insert(make_pair(rkk->rid(), rkk));
		}
	}
	return m_pRewards;
}

Reward *RedisGet::getReward(int rid){
	if (m_pRewards.find(rid) != m_pRewards.end()){
		return m_pRewards.at(rid);
	}
	return NULL;
}

map<string, map<int, ExRecord *>> RedisGet::getExRecords(){
	if (!m_pExRecords.empty()){
		return m_pExRecords;
	}
	auto vecs = getUserBases();
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		ExRecord er;
		auto itr = vecs.begin();
		for (itr; itr != vecs.end(); itr++){
			string uid = itr->first;
			vector<string> keys = m_redis->getKeys(g_redisdbnames[REIDS_EXCHANGE]+"exrecord"+ uid,false);
			map<int, ExRecord *> maps;
			for (int i = 0; i < keys.size();i++){
				string key = keys.at(i);
				ExRecord *er1 = (ExRecord *)m_redis->getHash(key,er.GetTypeName());
				maps.insert(make_pair(er1->eid(), er1));
			}
			m_pExRecords.insert(make_pair(uid, maps));
		}
	}
	return m_pExRecords;
}

map<int, ExRecord *> RedisGet::getExRecord(string uid){
	if (m_pExRecords.find(uid) != m_pExRecords.end()){
		return m_pExRecords.at(uid);
	}
	map<int, ExRecord *> vecs;
	return vecs;
}

void RedisGet::PushExRecord(string uid, ExRecord *p){
	int eid = p->eid();
	if (m_pExRecords.find(uid) != m_pExRecords.end()){
		auto vec = m_pExRecords.at(uid);
		if (vec.find(eid) != vec.end()){
			vec.at(eid)->CopyFrom(*p);
		}
		else{
			ExRecord *p1 = (ExRecord *)ccEvent::create_message(p->GetTypeName());
			p1->CopyFrom(*p);
			vec.insert(make_pair(eid,p1));
		}
		m_pExRecords.at(uid) = vec;
	}
	else{
		map<int,ExRecord *> vecs;
		ExRecord *p1 = (ExRecord *)ccEvent::create_message(p->GetTypeName());
		p1->CopyFrom(*p);
		vecs.insert(make_pair(eid, p1));
		m_pExRecords.insert(make_pair(uid, vecs));
	}
}


map<int, SignAward*> RedisGet::getSignAward(){
	if (!m_pSignAwards.empty()){
		return m_pSignAwards;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		SignAward si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SIGN], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			SignAward *rkk = (SignAward *)vv.at(i);

			m_pSignAwards.insert(make_pair(rkk->id(), rkk));
		}
	}
	
	return m_pSignAwards;
}

SignStatus *RedisGet::getSignStatus(string uid){
	if (m_pSignStatuss.find(uid) != m_pSignStatuss.end()){
		return m_pSignStatuss.at(uid);
	}
	
	return NULL;
}

void RedisGet::setSignStatus( SignStatus *ss){
	if (m_pSignStatuss.find(ss->_uid) != m_pSignStatuss.end()){
		m_pSignStatuss.at(ss->_uid) = ss;
	}
	else{
		m_pSignStatuss.insert(make_pair(ss->_uid, ss));
	}
}

map<string, SignStatus *> RedisGet::getSignStatuss(){
	if (!m_pSignStatuss.empty()){
		return m_pSignStatuss;
	}
	string key = "status";
	auto vecs = getUserBases();
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		auto itr = vecs.begin();
		int len = 0;
		for (itr; itr != vecs.end(); itr++){
			string uid = itr->first;
			char *content = m_redis->get(g_redisdbnames[REIDS_SIGN] + key + uid, len);
			if (content){
				for (int i = 0; i < vecs.size(); i++){
					vector<string> tt = CSVDataInfo::getIns()->getStrFromstr(content, ",");
					SignStatus *ss = new SignStatus();
					ss->_uid = tt.at(0);
					ss->_signcount = atoi(tt.at(1).c_str());
					ss->_issign = atoi(tt.at(2).c_str());
					ss->_left = atoi(tt.at(3).c_str());
					ss->_time = tt.at(4);
					m_pSignStatuss.insert(make_pair(ss->_uid, ss));
				}
			}
			else{
				SignStatus *ss = new SignStatus();
				ss->_uid = uid;
				m_pSignStatuss.insert(make_pair(ss->_uid, ss));
				RedisPut::getIns()->setSignStatus(ss);
			}
		}
	}
	return m_pSignStatuss;
}

map<int, SignZhuan *> RedisGet::getSignZhuan(){
	if (!m_pSignZhuans.empty()){
		return m_pSignZhuans;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	std::vector<SignZhuan > vecs;
	if (ist){
		SignZhuan si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_SIGN] + "zhuan", si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			SignZhuan *rkk=(SignZhuan *)vv.at(i);
			m_pSignZhuans.insert(make_pair(rkk->index(),rkk));
		}
	}
	return m_pSignZhuans;
}

map<int, Prop *> RedisGet::getProp(){
	if (!m_pProps.empty()){
		return m_pProps;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PROP);
	if (ist){
		Prop si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_PROP], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Prop *rkk = (Prop *)vv.at(i);
			m_pProps.insert(make_pair(rkk->id(), rkk));
		}
	}
	return m_pProps;
}

Prop *RedisGet::getProp(int id){
	map<int, Prop *> vec = getProp();
	if (vec.find(id) != vec.end()){
		return vec.at(id);
	}
	return NULL;
}

map<int, Task*> RedisGet::getFree(){
	if (!m_pFrees.empty()){
		return m_pFrees;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FREE);
	if (ist){
		Task si;
		std::vector<Message *> vv = m_redis->getList(g_redisdbnames[REIDS_FREE], si.GetTypeName());
		for (int i = 0; i < vv.size(); i++){
			Task *rkk = (Task *)vv.at(i);
			m_pFrees.insert(make_pair(rkk->taskid(),rkk));
		}
	}
	return m_pFrees;
}


Status *RedisGet::getFreeStatus(string uid, int fid){
	auto vec = getFreeStatus(uid);
	if (vec.find(fid) != vec.end()){
		return vec.at(fid);
	}
	return NULL;
}

map<string, map<int, Status *>> RedisGet::getFreeStatus(){
	if (!m_pFreeStatus.empty()){
		return m_pFreeStatus;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FREE);
	if (ist){
		Status st;
		auto users = getUserBases();
		auto useritr = users.begin();
		for (useritr; useritr != users.end(); useritr++){
			string uid = useritr->first;
			auto vecs = getFree();
			auto itr = vecs.begin();
			map<int, Status *> maps;
			for (itr; itr != vecs.end(); itr++){
				int fid = itr->first;
				char buff[30];
				sprintf(buff, "%sstatus%d", (g_redisdbnames[REIDS_FREE] + uid).c_str(), fid);
				Status *st1 = (Status *)m_redis->getHash(buff, st.GetTypeName());
				maps.insert(make_pair(fid, st1));
			}
			m_pFreeStatus.insert(make_pair(uid, maps));
		}
	}
	return m_pFreeStatus;
}

map<int, Status *> RedisGet::getFreeStatus(string uid){
	if (m_pFreeStatus.find(uid) != m_pFreeStatus.end()){
		return m_pFreeStatus.at(uid);
	}
	map<int, Status *> vec;
	return vec;
}

void RedisGet::setFreeStatus(string uid, int fid, Status *ss){
	Status *st = NULL;
	if (m_pFreeStatus.find(uid) != m_pFreeStatus.end()){
		map<int, Status *> *vec = &(m_pFreeStatus.at(uid));
		if (vec->find(fid) != vec->end()){
			st = vec->at(fid);
			st->CopyFrom(*ss);
		}
		else{
			Status *st = (Status *)ccEvent::create_message(ss->GetTypeName());
			st->CopyFrom(*ss);
			vec->insert(make_pair(fid, st));
		}
	}
	else{
		Status *st = (Status *)ccEvent::create_message(ss->GetTypeName());
		st->CopyFrom(*ss);
		map<int, Status *> vec;
		vec.insert(make_pair(fid, st));
		m_pFreeStatus.insert(make_pair(uid, vec));
	}
}

string RedisGet::getExchangeCode(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_EXCHANGE] + "exrecordcode", len);
		if (!dd){
			dd = "10000000";
			
		}
		char buff[30];
		sprintf(buff,"%ld",atoi(dd)+1);
		RedisPut::getIns()->setExchangeCode(buff);
		return dd;
	}
	return "";
}

int RedisGet::getExchangeRecordId(string uid){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_EXCHANGE] + "exrecordid" + uid, len);
		if (!dd){
			dd = "1";
			
		}
		RedisPut::getIns()->setExchangeRecordId(uid, atoi(dd)+1);
		return atoi(dd);
	}
	return -1;
}

map<string, PExchangeCode*> RedisGet::getCSVExchangeCode(){
	if (!m_pExchangeCodes.empty()){
		return m_pExchangeCodes;
	}
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		string key = g_redisdbnames[REIDS_EXCHANGE] + "code";
		auto mpas = m_redis->getListStr(key);
		
		for (int i = 0; i < mpas.size(); i++){
			vector<string >vec;
			std::string field = mpas.at(i);
			int index = field.find(",");
			while (index != -1) {
				vec.push_back(field.substr(0, index));
				field = field.substr(index + 1, field.length());
				index = field.find(",");
			}
			vec.push_back(field);
			PExchangeCode* cc = new PExchangeCode();
			if (vec.size() == 3){
				cc->_id = atoi(vec.at(0).c_str());
				cc->_rewardid = CSVDataInfo::getIns()->getIntFromstr(vec.at(1), "|");
				cc->_code = vec.at(2);
				m_pExchangeCodes.insert(make_pair(cc->_code, cc));
			}
			else{
				printf("vvvvvvvvvvvvvvvvvvv\n");
			}
			/*
			map<string, bool> veccodes = getExcode();
			if (veccodes.empty()){
				printf("%d.", i + 1);
				RedisPut::getIns()->setEXCode(cc->_code, false);
			}
			*/
		}
	}
	return m_pExchangeCodes;
}

PExchangeCode* RedisGet::getPExchangeCode(string code){
	if (m_pExchangeCodes.find(code) != m_pExchangeCodes.end()){
		return m_pExchangeCodes.at(code);
	}
	return NULL;
}

void RedisGet::erasePExchangeCode(string code){
	if (m_pExchangeCodes.find(code) != m_pExchangeCodes.end()){
		PExchangeCode *p = m_pExchangeCodes.at(code);
		delete p;
		p = NULL;
		m_pExchangeCodes.erase(m_pExchangeCodes.find(code));
	}
}

int RedisGet::getMailID(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_MAIL);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_MAIL] + "mailid", len);
		if (!dd){
			dd = "1";
		}
		RedisPut::getIns()->setMailID(atoi(dd)+1);
		
		return atoi(dd);
	}
	return 1;
}

int RedisGet::getFriendNoticeID(){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		int len = 0;
		char *dd = m_redis->get(g_redisdbnames[REIDS_FRIEND] + "friendnoticeid", len);
		if (!dd){
			dd = "1";
			RedisPut::getIns()->setFriendNoticeID(atoi(dd)+1);
		}
		RedisPut::getIns()->setFriendNoticeID(atoi(dd) + 1);
		return atoi(dd);
	}
	return 1;
}


bool compareRank(Rank *rk1, Rank *rk2){
	int type = rk1->type();
	if (type == 1){
		unsigned int gold1 = rk1->info().gold();
		unsigned int gold2 = rk2->info().gold();
		if (gold1 > gold2){
			return true;
		}
		else{
			return false;
		}
	}
	else if (type == 2){
		unsigned int gold1 = rk1->info().win();
		unsigned int gold2 = rk2->info().win();
		if (gold1 > gold2){
			return true;
		}
		else{
			return false;
		}
	}
	return false;
}

void RedisGet::sortRank(){
	auto itr = m_pRanks.begin();
	char buff[50];
	for (itr; itr != m_pRanks.end(); itr++){
		auto vec = itr->second;
		sort(vec.begin(),vec.end(),compareRank);
		for (int i = 0; i < vec.size(); i++){
			Rank *rk1 = vec.at(i);
			rk1->set_lv(i + 1);

			sprintf(buff, "%s%d", g_redisdbnames[REIDS_RANK].c_str(), itr->first);
			bool ist = RedisGet::getIns()->SelectDB(REIDS_RANK);
			if (ist){
				m_redis->setList(buff, i, rk1);
			}
		}
	}
}

void RedisGet::update(float){
	int tm = Common::getTimeS();
	if (tm > 0 && tm < 2*60){
		sortRank();
	}
}