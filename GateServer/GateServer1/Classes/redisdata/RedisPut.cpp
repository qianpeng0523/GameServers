
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



bool RedisPut::PushConfig(string uid, SConfig *scf){
	bool ist = RedisGet::getIns()->SelectDB(REDIS_SCONFIG);
	if (ist){
		RedisGet::getIns()->setSConfig(uid, scf);
		return m_redis->Hash(g_redisdbnames[REDIS_SCONFIG] + uid, scf);
	}
	return ist;
}

bool RedisPut::setConfig(string uid, POINTTIP type, bool ist){
	SConfig *sc = RedisGet::getIns()->getSConfig(uid);
	if (sc){
		switch (type)
		{
		case POINT_MAIL:
			sc->set_mail(ist);
			break;
		case POINT_SHOUCHONG:
			sc->set_firstbuy(ist);
			break;
		case POINT_ACTIVE:
			sc->set_active(ist);
			break;
		case POINT_SHOP:
			
			break;
		case POINT_MAINFEI:
			sc->set_free(ist);
			break;
		case POINT_HAOYOU:
			sc->set_fri(ist);
			break;
		case POINT_RENWU:
			sc->set_task(ist);
			break;
		case POINT_SIGN:
			sc->set_yqs(ist);
			break;
		default:
			break;
		}
	}
	return PushConfig(uid, sc);
}

bool RedisPut::PushUserBase(UserBase *ub){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		string uid = ub->userid();
		m_redis->Hash(g_redisdbnames[REIDS_USERBASE]+"_user" + uid, ub);
	}
	return ist;
}


bool RedisPut::setUserBase(string uid,string key, string value){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		UserBase *ub = RedisGet::getIns()->getUserBase(uid);
		Descriptor *des = (Descriptor *)ub->GetDescriptor();
		Reflection *reflection = (Reflection *)ub->GetReflection();
		int sz = des->field_count();
		for (int i = 0; i < sz; i++){
			FieldDescriptor *fd = (FieldDescriptor *)des->field(i);
			string name = fd->name();
			FieldDescriptor::Type type = fd->type();
			if (name.compare(key) == 0){
				switch (type)
				{
				case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
					reflection->SetDouble(ub, fd, atof(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_FLOAT:
					reflection->SetFloat(ub, fd, atof(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_INT64:
					reflection->SetInt64(ub, fd, atol(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT64:
					reflection->SetUInt64(ub, fd, atol(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_INT32:
					reflection->SetInt32(ub, fd, atoi(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_BOOL:
					reflection->SetBool(ub, fd, atoi(value.c_str()));
					break;
				case google::protobuf::FieldDescriptor::TYPE_STRING:
					reflection->SetString(ub, fd, value);
					break;
				case google::protobuf::FieldDescriptor::TYPE_BYTES:
					reflection->SetString(ub, fd, value);
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT32:
					reflection->SetUInt32(ub, fd, atol(value.c_str()));
					break;

				default:
					break;
				}
				break;
			}
		}
		return PushUserBase(ub);
	}
	return ist;
}

bool RedisPut::addUserBase(string uid, string key, int value){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		UserBase *ub1 = RedisGet::getIns()->getUserBase(uid);
		Descriptor *des = (Descriptor *)ub1->GetDescriptor();
		Reflection *reflection = (Reflection *)ub1->GetReflection();
		int sz = des->field_count();
		for (int i = 0; i < sz; i++){
			FieldDescriptor *fd = (FieldDescriptor *)des->field(i);
			string name = fd->name();
			FieldDescriptor::Type type = fd->type();
			if (name.compare(key) == 0){
				if (type == FieldDescriptor::TYPE_INT32){
					reflection->SetInt32(ub1, fd, reflection->GetInt32(*ub1, fd) + value);
				}
				else if (type == FieldDescriptor::TYPE_INT64){
					reflection->SetInt64(ub1, fd, reflection->GetInt64(*ub1, fd) + value);
				}
				else if (type == FieldDescriptor::TYPE_UINT32){
					reflection->SetUInt32(ub1, fd, reflection->GetUInt32(*ub1, fd) + value);
				}
				else if (type == FieldDescriptor::TYPE_UINT64){
					reflection->SetUInt64(ub1, fd, reflection->GetUInt64(*ub1, fd) + value);
				}
				break;
			}
		}
		return PushUserBase(ub1);
	}
	return ist;
}

bool RedisPut::PushUserLoginTime(string uid){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_USERBASE);
	if (ist){
		string tt = "logintime";
		char buff[20];
		time_t time = Common::getCurrentTime();
		sprintf(buff, "%ld", time);
		RedisGet::getIns()->setUserLoginTime(uid, time);
		return m_redis->set(g_redisdbnames[REIDS_USERBASE] + tt+uid, buff);
		
	}
	else{
		return false;
	}
}

bool RedisPut::PushPass(string uid, string pass){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PASS);
	if (ist){
		int len = pass.length();
		m_redis->set(g_redisdbnames[REIDS_PASS] + uid, (char *)pass.c_str(), len);
	}
	return ist;
}

bool RedisPut::PushOpenid(string openid, string uid){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_PASS);
	if (ist){
		int len = uid.length();
		return m_redis->set(g_redisdbnames[REIDS_PASS] + "openid" + openid, (char *)uid.c_str(), len);
	}
	return ist;
}

bool RedisPut::PushRank(Rank *rk){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_RANK);
	if (ist){
		int type = rk->type();
		char buff[50];
		sprintf(buff, "%s%d", g_redisdbnames[REIDS_RANK].c_str(), type);
		m_redis->Hash(buff,rk);
	}
	return ist;
}

bool RedisPut::PushMail(string uid, Mail *mail){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_MAIL);
	if (ist){
		int mid = mail->eid();
		char buff[100];
		sprintf(buff, "%s%s%d", g_redisdbnames[REIDS_MAIL].c_str(), uid.c_str(), mid);
		m_redis->Hash(buff, mail);
	}
	return ist;
}

void RedisPut::PopMail(string uid, Mail *ml){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_MAIL);
	if (ist){
		int mid = ml->eid();
		char buff[100];
		sprintf(buff, "%s%s%d", g_redisdbnames[REIDS_MAIL].c_str(), uid.c_str(), mid);
		m_redis->delKey(buff);
	}
}

bool RedisPut::PushFriend(string uid, Friend *fd){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		char buff[100];
		string fuid = fd->info().userid();
		sprintf(buff, "%s%s%s", g_redisdbnames[REIDS_FRIEND].c_str(), uid.c_str(), fuid.c_str());
		m_redis->Hash(buff, fd);
	}
	return ist;
}

bool RedisPut::PushFriendNotice(string uid, FriendNotice *fn){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		char buff[100];
		sprintf(buff, "%s%d", (g_redisdbnames[REIDS_FRIEND] + "notice" + uid).c_str(), fn->nid());
		m_redis->Hash(buff, fn);
	}
	return ist;
}

bool RedisPut::eraseFriendNotice(string uid, FriendNotice *fn){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		char buff[100];
		sprintf(buff, "%s%d", (g_redisdbnames[REIDS_FRIEND] + "notice" + uid).c_str(), fn.nid());
		m_redis->delKey(buff);
	}
	return ist;
}

bool RedisPut::PushTaskStatus(string uid, int taskid, Status *status){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_TASK);
	if (ist){
		char buff[30];
		sprintf(buff, "%sstatus%d", (g_redisdbnames[REIDS_TASK] + uid).c_str(), taskid);
		m_redis->Hash(buff, status);
	}
	return ist;
}

bool RedisPut::PushFreeStatus(string uid, int fid, Status *status){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FREE);
	if (ist){
		char buff[30];
		sprintf(buff, "%sstatus%d", (g_redisdbnames[REIDS_FREE] + uid).c_str(), fid);
		m_redis->Hash(buff, status);
	}
	return ist;
}

bool RedisPut::PushExRecord(string uid, ExRecord *er){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		char buff[100];
		sprintf("%sexrecord%s%d", g_redisdbnames[REIDS_EXCHANGE].c_str(), uid.c_str(), er->eid());
		m_redis->Hash(buff, er);
	}
	return ist;
}

bool RedisPut::setExchangeCode(string code){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		return m_redis->set(g_redisdbnames[REIDS_EXCHANGE]+"exrecordcode",code);
	}
	return ist;
}

bool RedisPut::setExchangeRecordId(string uid, int id){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		return m_redis->set(g_redisdbnames[REIDS_EXCHANGE] + "exrecordid"+uid, id);
	}
	return ist;
}

bool RedisPut::setEXCode(string code, bool isdui){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_EXCHANGE);
	if (ist){
		char buff[10];
		sprintf(buff, "%d", isdui);
		int len = 0;
		return m_redis->set(g_redisdbnames[REIDS_EXCHANGE]+"duihuancode"+code, buff,len);
	}
	return ist;
}

bool RedisPut::setSignStatus(SignStatus *ss){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_SIGN);
	if (ist){
		char buff[50];
		string uid = ss->_uid;
		sprintf(buff, "%s", (g_redisdbnames[REIDS_SIGN] + "status" + uid).c_str());
		char buff1[200];
		sprintf(buff1,"%s,%d,%d,%d,%s",uid.c_str(),ss->_signcount,ss->_issign,ss->_left,ss->_time.c_str());
		m_redis->set(buff, buff1);
	}
	return ist;
}

bool RedisPut::PushFeedBack(CFeedBack *cfb){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FEEDBACK);
	if (ist){
		return m_redis->Hash(g_redisdbnames[REIDS_FEEDBACK], cfb);
	}
	return ist;
}

bool RedisPut::setMailID(int mid){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_MAIL);
	if (ist){
		return m_redis->set(g_redisdbnames[REIDS_MAIL]+"mailid",mid);
	}
	return ist;
}

bool RedisPut::setFriendNoticeID(int id){
	bool ist = RedisGet::getIns()->SelectDB(REIDS_FRIEND);
	if (ist){
		return m_redis->set(g_redisdbnames[REIDS_FRIEND] + "friendnoticeid", id);
	}
	return ist;
}

void RedisPut::ZeroChange(char *&data, int sz){
	for (int i = 0; i < sz; i++){
		if (data[i] == ZERO_STR){
			data[i] = '\0';
		}
	}
}