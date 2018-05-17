
#include "RedisPut.h"
#include "RedisGet.h"

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

bool RedisPut::PushShop(ShopItem item){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_SHOP], &item);
	}
	return ist;
}

bool RedisPut::PushActive(Active at){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_ACTIVE);
	if (ist){
		int type = at.type();
		char buff[100];
		sprintf(buff, "%s%d", g_redisdbnames[REIDS_ACTIVE].c_str(), type);
		m_redis->List(buff, &at);
	}
	return ist;
}

bool RedisPut::PushTask(Task task){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_TASK);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_TASK], &task);
	}
	return ist;
}


bool RedisPut::PushExAward(ExAward ea){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_EXCHANGE]+"award", &ea);
	}
	return ist;
}

bool RedisPut::PushReward(Reward rd){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_REWARD);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_REWARD], &rd);
	}
	return ist;
}

bool RedisPut::PushSignAward(SignAward sa){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_SIGN], &sa);
	}
	return ist;
}

bool RedisPut::PushSignZhuan(SignZhuan sz){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_SIGN]+"zhuan", &sz);
	}
	return ist;
}

bool RedisPut::PushProp(Prop p){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PROP);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_PROP], &p);
	}
	return ist;
}

bool RedisPut::PushFree(Task task){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FREE);
	if (ist){
		m_redis->List(g_redisdbnames[REIDS_FREE], &task);
	}
	return ist;
}

bool RedisPut::PushFirstBuy(CSVFirstBuyItem *sv){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SHOP);
	if (ist){
		string key = g_redisdbnames[REIDS_SHOP] + "first";
		char buff[50];
		sprintf(buff, "%d", sv->_sid);
		m_redis->Hash(key, "id", buff);
		m_redis->Hash(key, "rewardid", sv->_rid);
		m_redis->Hash(key, "conid", sv->_conid);
		return m_redis->Hash("firstbuy", "giveid", sv->_giveid);
	}
	return ist;
}

bool RedisPut::PushExchangeCode(CSVExchangeCode *item){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		char buff[100];
		sprintf(buff, "%d", item->_id);
		string content = buff;
		content += "," + item->_rewardid + "," + item->_code;
		return m_redis->List(g_redisdbnames[REIDS_EXCHANGE]+"code", (char *)content.c_str());
	}
	return ist;
}

bool RedisPut::PushGate(GateData *gd){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		
	}
	return ist;
}