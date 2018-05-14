
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
	UserBase *ub1 = (UserBase *)ccEvent::create_message(ub.GetTypeName());
	ub1->CopyFrom(ub);
	RedisGet *pRedisGet = RedisGet::getIns();
	PushUserLoginTime(ub.userid());
	int index = pRedisGet->getUserBaseIndex(ub.userid());
	pRedisGet->setUserBase(ub1);
	return m_redis->List("userbase", ub1);
}

bool RedisPut::setUserBase(string uid,string key, string value){
	UserBase *ub=RedisGet::getIns()->getUserBase(uid);
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
	int index = RedisGet::getIns()->getUserBaseIndex(uid);
	return m_redis->setList("userbase",index,ub);
}

bool RedisPut::addUserBase(string uid, string key, int value){
	UserBase *ub1=RedisGet::getIns()->getUserBase(uid);
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
			}else if (type == FieldDescriptor::TYPE_UINT32){
				reflection->SetUInt32(ub1, fd, reflection->GetUInt32(*ub1, fd) + value);
			}else if (type == FieldDescriptor::TYPE_UINT64){
				reflection->SetUInt64(ub1, fd, reflection->GetUInt64(*ub1, fd) + value);
			}
			break;
		}
	}
	int index = RedisGet::getIns()->getUserBaseIndex(uid);
	return m_redis->setList("userbase", index, ub1);
}

bool RedisPut::PushUserLoginTime(string uid){
	string tt = "userlogintime";
	char buff[20];
	time_t time = Common::getCurrentTime();
	sprintf(buff, "%ld",time);
	int index = RedisGet::getIns()->getUserBaseIndex(uid);
	RedisGet::getIns()->setUserLoginTime(uid,time);
	if (index == -1){
		return m_redis->List(tt,buff);
	}
	else{
		return m_redis->setList(tt,index,buff);
	}
}

bool RedisPut::PushRank(Rank rk){
	int type = rk.type();
	char buff[50];
	sprintf(buff,"%s%d",rk.GetTypeName().c_str(),type);
	return m_redis->List(buff,&rk);
}

bool RedisPut::PushPass(string uid, string pass){
	string key = "userpass";
	return m_redis->List(key, (char *)(uid + "," + pass).c_str());
}

bool RedisPut::PushOpenid(string openid, string uid){
	string key = "openid";
	return m_redis->List(key,(char *)(openid+","+uid).c_str());
}

bool RedisPut::PushShop(ShopItem item){
	return m_redis->List("shop",&item);
}

bool RedisPut::PushMail(string uid, Mail mail){
	return m_redis->List("mail"+uid,&mail);
}

void RedisPut::ZeroChange(char *&data, int sz){
	for (int i = 0; i < sz; i++){
		if (data[i] == ZERO_STR){
			data[i] = '\0';
		}
	}
}

void RedisPut::PopMail(string uid, Mail ml){
	int sz = ml.ByteSize();
	char *buffer = new char[sz];
	ml.SerializePartialToArray(buffer, sz);
	ZeroChange(buffer, sz);
	m_redis->eraseList("mail"+uid, buffer);
}

bool RedisPut::setMailStatus(string uid, int mid, int status){
	char buff[50];
	sprintf(buff,"mailstatus%s%d",uid.c_str(),mid);
	return m_redis->set(buff,status);
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
	RedisGet::getIns()->PushExRecord(uid, er);
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

bool RedisPut::setMailID(int mid){
	return m_redis->set("mailid_index",mid);
}

bool RedisPut::setSignStatus(SignStatus ss){
	char buff[50];
	sprintf(buff,"signstatus%s",ss._uid.c_str());
	char buff1[50];
	if (ss._signcount != -1){
		sprintf(buff1, "%d", ss._signcount);
		m_redis->Hash(buff, "count", buff1);
	}
	if (ss._issign != -1){
		sprintf(buff1, "%d", ss._issign);
		m_redis->Hash(buff, "sign", buff1);
	}

	if (ss._left != -1){
		sprintf(buff1, "%d", ss._left);
		m_redis->Hash(buff, "left", buff1);
	}

	if (!ss._time.empty()){
		sprintf(buff1, "%s", ss._time.c_str());
		m_redis->Hash(buff, "time", buff1);
	}
	RedisGet::getIns()->setSignStatus(ss);
	return true;
}

bool RedisPut::setSConfig(string uid, SConfig sc){
	return m_redis->Hash("config" + uid, &sc);
}

bool RedisPut::PushFeedBack(CFeedBack cfb){
	string key = "feedback"+cfb.uid();
	return m_redis->List(key, &cfb);
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

bool RedisPut::setConfig(string uid, POINTTIP type, bool ist){
	string key="config"+uid;
	char va[10];
	sprintf(va,"%d",ist);
	return m_redis->Hash(key,g_PointStr[type],va);
}

bool RedisPut::setExchangeCode(string code){
	int len = 0;
	return m_redis->set("exchangecode",(char*)code.c_str(),len);
}

bool RedisPut::setExchangeRecordId(string uid, int id){
	return m_redis->set("exchangerecordid"+uid, id);
}

bool RedisPut::setEXCode(string code, bool isdui){
	string key = "EXCodeStatus";
	char buff[10];
	sprintf(buff,"%d",isdui);
	return m_redis->List(key,buff);
}

bool RedisPut::changeEXCode(string code, bool isdui){
	PExchangeCode *p = RedisGet::getIns()->getPExchangeCode(code);
	if (p){
		char buff[10];
		sprintf(buff, "%d", isdui);
		RedisGet::getIns()->setEXCodeStatus(p->_id, isdui);
		return m_redis->setList("EXCodeStatus",p->_id-1,buff);
	}
}