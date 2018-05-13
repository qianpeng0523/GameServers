
#include "RedisPut.h"

RedisPut *RedisPut::m_ins=NULL;
RedisPut::RedisPut(){
	m_redis = redis::getIns();
}

RedisPut::~RedisPut(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

RedisPut *RedisPut::getIns(){
	if (!m_ins){
		m_ins = new RedisPut();
		m_ins->init();
	}
	return m_ins;
}

void RedisPut::init(){
	
}

bool RedisPut::PushConfig(string uid, SConfig scf){
	return m_redis->Hash("config"+uid,&scf);
}

bool RedisPut::PushUserBase(UserBase ub){
	string uid = ub.userid();
	return m_redis->Hash(ub.GetTypeName() + uid, &ub);
}

bool RedisPut::PushRank(Rank rk){
	int type = rk.type();
	char buff[50];
	sprintf(buff,"%s%d",rk.GetTypeName(),type);
	return m_redis->List(buff,&rk);
}

bool RedisPut::PushPass(string uid, string pass){
	return m_redis->set("pass" + uid, (char *)pass.c_str(),pass.length());
}

bool RedisPut::PushShop(ShopItem item){
	return m_redis->List("shop",&item);
}

bool RedisPut::PushMail(string uid, Mail mail){
	return m_redis->List("mail"+uid,&mail);
}

bool RedisPut::PushFriend(string uid, string friuid){
	return m_redis->List("firend"+uid,(char *)friuid.c_str());
}

bool RedisPut::PushFriendNotice(string uid, FriendNotice fn){
	return m_redis->List("friendnotice" + uid, &fn);
}

bool RedisPut::PushActive(Active at){
	int type = at.type();
	char buff[100];
	sprintf(buff,"active%d",type);
	return m_redis->List(buff,&at);
}

bool RedisPut::PushTask(Task task){
	return m_redis->List("task", &task);
}

bool RedisPut::PushTaskStatus(string uid, int taskid, Status status){
	char buff[30];
	sprintf(buff,"task%s%d",uid.c_str(),taskid);
	return m_redis->Hash(buff,&status);
}

bool RedisPut::PushExAward(ExAward ea){
	return m_redis->List("exchangereward",&ea);
}

bool RedisPut::PushReward(Reward rd){
	return m_redis->List("reward", &rd);
}

bool RedisPut::PushExRecord(string uid, ExRecord er){
	return m_redis->List("exchangerecord" + uid, &er);
}

bool RedisPut::setExRecordStatus(string uid, int rid, int status){
	char buff[50];
	sprintf(buff,"exchangestatus%s%d",uid.c_str(),rid);
	return m_redis->set(buff,status);
}

bool RedisPut::PushSignAward(SignAward sa){
	return m_redis->List("sign", &sa);
}

bool RedisPut::setSignStatus(string uid, int signid, int times){
	char buff[50];
	sprintf(buff,"%s%d",uid.c_str(),signid);
	return m_redis->set(buff, times);
}

bool RedisPut::setSConfig(string uid, SConfig sc){
	return m_redis->Hash("config" + uid, &sc);
}

bool RedisPut::PushSignZhuan(SignZhuan sz){
	return m_redis->List("signzhuan", &sz);
}

bool RedisPut::PushProp(Prop p){
	return m_redis->List("prop", &p);
}

bool RedisPut::PushFree(Task task){
	return m_redis->List("free", &task);
}

bool RedisPut::PushFirstBuy(CSVFirstBuyItem *sv){
	char buff[50];
	sprintf(buff, "%d", sv->_sid);
	m_redis->Hash("firstbuy","id", buff);
	m_redis->Hash("firstbuy", "rewardid", sv->_rid);
	m_redis->Hash("firstbuy", "conid", sv->_conid);
	return m_redis->Hash("firstbuy", "giveid", sv->_giveid);
}

bool RedisPut::PushDuiHuanCode(CSVExchangeCode *item){
	string key = "CSVExchangeCode";
	char buff[200];
	sprintf(buff,"%d",item->_id);
	string content =buff;
	content += ","+item->_rewardid+","+item->_code;
	return m_redis->List(key,(char *)content.c_str());
}