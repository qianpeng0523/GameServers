#include "LoginInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "LibEvent.h"
#include "HttpLogic.h"
#include "StatTimer.h"
LoginInfo *LoginInfo::m_shareLoginInfo=NULL;
LoginInfo::LoginInfo()
{
	EventDispatcher *pe = EventDispatcher::getIns();
	EventListen *p = EventListen::getIns();
	CLogin cl;
	pe->registerProto(cl.cmd(), cl.GetTypeName());
	EventListen::getIns()->addDataPacketListener(cl.cmd(), this, Event_Handler(LoginInfo::HandlerCLoginHand));

	CRegister cl1;
	pe->registerProto(cl1.cmd(), cl1.GetTypeName());
	EventListen::getIns()->addDataPacketListener(cl1.cmd(), this, Event_Handler(LoginInfo::HandlerCRegister));

	StatTimer::getIns()->scheduleSelector(this, schedule_selector(LoginInfo::Check), 8.0);
}

LoginInfo::~LoginInfo(){
	StatTimer::getIns()->unscheduleSelector(this, schedule_selector(LoginInfo::Check));
}

LoginInfo* LoginInfo::getIns(){
	if (!m_shareLoginInfo){
		m_shareLoginInfo = new LoginInfo();
		m_shareLoginInfo->init();
	}
	return m_shareLoginInfo;
}

bool LoginInfo::init()
{
	
    return true;
}

void LoginInfo::SendSLogin(SLogin cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(),&cl,fd);
}

void LoginInfo::HandlerCLoginHand(ccEvent *event){
	CLogin cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();


	SLogin sl;
	sl.set_cmd(sl.cmd());
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion=uid+pwd+LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
		string ip = data->_ip;
		DBUserInfo *info = sl.mutable_info();
		
		Message *msg = redis::getIns()->getHash(info->GetTypeName()+uid,info->GetTypeName());
		if (msg){
			info->CopyFrom(*msg);
			info->set_ip(ip);
		}
		else{
			sl.release_info();
			sl.set_err(1);
		}
		
	}
	else{
		sl.set_err(1);
	}
	printf("login:uid:%s,pwd:%s",uid.c_str(),pwd.c_str());
	
	SendSLogin(sl,event->m_fd);
}


void LoginInfo::SendSRegister(SRegister cl, int fd){
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void LoginInfo::HandlerCRegister(ccEvent *event){
	CRegister cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();
	string uname = cl.uname();

	SRegister sl;
	sl.set_cmd(sl.cmd());
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion = uid + pwd + LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
		
		DBUserInfo *user = sl.mutable_info();
		Message *msg = redis::getIns()->getHash(user->GetTypeName() + uid,user->GetTypeName());
		if (msg){
			sl.release_info();
			sl.set_err(1);
		}
		else{
			user->CopyFrom(*msg);
			user->set_ip(data->_ip);
			bool ist = redis::getIns()->Hash(user->GetTypeName() + uid, user);
			if (ist){
				sl.set_err(0);
				uint32 gold = user->gold();
				Rank rk;
				rk.set_uid(user->userid());
				rk.set_number(gold);
				rk.set_type(1);
				redis::getIns()->List(rk.GetTypeName(), &rk);
				uint32 day = 0;

			}
			else{
				sl.set_err(1);
				sl.release_info();
			}
		}
	}
	else{
		sl.set_err(1);
	}
	
	SendSRegister(sl, event->m_fd);
}


void LoginInfo::Check(float dt){
	if (!redis::getIns()->isConnect()){
		redis::getIns()->reconnect();
	}
}





































::google::protobuf::Message * LoginInfo::getDBDataFromSocketDataVo(string tname, CSJson::Value sd){
	::google::protobuf::Message *msg = ccEvent::create_message(tname);
	::google::protobuf::Descriptor *des = (::google::protobuf::Descriptor *)msg->GetDescriptor();
	::google::protobuf::Reflection *reflection = (::google::protobuf::Reflection *) msg->GetReflection();
	int sz = des->field_count();
	for (int i = 0; i < sz; i++){
		::google::protobuf::FieldDescriptor *fd = (::google::protobuf::FieldDescriptor *)des->field(i);
		string name = fd->name();
		if (name.compare("cmd") == 0){
			continue;
		}
		::google::protobuf::FieldDescriptor::Type type = fd->type();
		int count = 0;
		if (sd.isMember(name)){
			count = sd[name].size();
		}
		else{
			count = sd["data"].size();
		}
		int j = 0;
		::google::protobuf::Message *msg1 = NULL;
		switch (type)
		{
		case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
			if (sd.isMember(name)){
				reflection->SetDouble(msg, fd, sd[name].asDouble());
			}
			else{
				reflection->SetDouble(msg, fd, sd["data"].asDouble());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_FLOAT:
			if (sd.isMember(name)){
				reflection->SetFloat(msg, fd, sd[name].asFloat());
			}
			else{
				reflection->SetFloat(msg, fd, sd["data"].asFloat());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT64:
			if (sd.isMember(name)){
				reflection->SetInt64(msg, fd, sd[name].asInt64());
			}
			else{
				reflection->SetInt64(msg, fd, sd["data"].asInt64());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT64:
			if (sd.isMember(name)){
				reflection->SetUInt64(msg, fd, sd[name].asUInt64());
			}
			else{
				reflection->SetUInt64(msg, fd, sd["data"].asUInt64());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT32:
			if (sd.isMember(name)){
				reflection->SetInt32(msg, fd, sd[name].asInt());
			}
			else{
				printf("%s:%d\n",name.c_str(),sd["data"].asInt());
				reflection->SetInt32(msg, fd, sd["data"].asInt());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_FIXED64:

			break;
		case google::protobuf::FieldDescriptor::TYPE_FIXED32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_BOOL:
			if (sd.isMember(name)){
				reflection->SetBool(msg, fd, sd[name].asBool());
			}
			else{
				reflection->SetBool(msg, fd, sd["data"].asBool());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_STRING:
			if (sd.isMember(name)){
				reflection->SetString(msg, fd, sd[name].asString());
			}
			else{
				printf("%s:%s\n", name.c_str(), sd["data"].asString().c_str());
				reflection->SetString(msg, fd, sd["data"].asString());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_GROUP:

			break;
		case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
			if (fd->is_repeated()){
				for (j = 0; j < count; j++){
					msg1=reflection->AddMessage(msg, fd);
					if (sd.isMember(name)){
						msg1->CopyFrom(*getDBDataFromSocketDataVo(fd->message_type()->full_name(), sd[name][j]));
					}
					else{
						msg1->CopyFrom(*getDBDataFromSocketDataVo(fd->message_type()->full_name(), sd["data"][j]));
					}
				}
			}
			else{
				msg1=reflection->MutableMessage(msg, fd);
				if (sd.isMember(name)){
					msg1->CopyFrom(*getDBDataFromSocketDataVo(fd->message_type()->full_name(), sd[name]));
				}
				else{
					msg1->CopyFrom(*getDBDataFromSocketDataVo(fd->message_type()->full_name(), sd["data"]));
				}
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_BYTES:
			if (sd.isMember(name)){
				reflection->SetString(msg, fd, sd[name].asString());
			}
			else{
				reflection->SetString(msg, fd, sd["data"].asString());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT32:
			if (sd.isMember(name)){
				reflection->SetUInt32(msg, fd, sd[name].asUInt());
			}
			else{
				reflection->SetUInt32(msg, fd, sd["data"].asUInt());
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_ENUM:
			break;
		case google::protobuf::FieldDescriptor::TYPE_SFIXED32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SFIXED64:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SINT32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SINT64:

			break;

		default:
			break;
		}
	}
	return msg;
}




::google::protobuf::Message * LoginInfo::getDBDataFromSocketData(string tablename, CSJson::Value sd){
	string protoname;
	if (protoname.compare("userinfo")==0){
		DBUserInfo user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("records") == 0){
		DBRecords user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("detail_records") == 0){
		DBDetailRecords user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("room") == 0){
		DBRoom user;
		protoname = user.GetTypeName();
	}
	
	return getDBDataFromSocketDataVo(protoname, sd);
}

void LoginInfo::setDBDataToSocketDataVo(::google::protobuf::Message* data, CSJson::Value &sd){
	::google::protobuf::Descriptor *des = (::google::protobuf::Descriptor *) data->GetDescriptor();
	::google::protobuf::Reflection *reflection = (::google::protobuf::Reflection *) data->GetReflection();
	int sz = des->field_count();
	for (int i = 0; i < sz; i++){
		::google::protobuf::FieldDescriptor *fd = (::google::protobuf::FieldDescriptor *)des->field(i);
		string name = fd->name();
		google::protobuf::Message* child;
		int j = 0;
		::google::protobuf::FieldDescriptor::Type type = fd->type();
		switch (type)
		{
		case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
			sd[name] =reflection->GetDouble(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_FLOAT:
			sd[name] =reflection->GetFloat(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT64:
			sd[name] =reflection->GetInt32(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT64:
			sd[name] =reflection->GetInt64(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT32:
			sd[name] =reflection->GetInt32(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_FIXED64:

			break;
		case google::protobuf::FieldDescriptor::TYPE_FIXED32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_BOOL:
			sd[name] =reflection->GetBool(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_STRING:
			sd[name] =reflection->GetString(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_GROUP:

			break;
		case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
			if (fd->is_repeated()){
				for (j = 0; j<reflection->FieldSize(*data,fd); j++){
					
					child = (google::protobuf::Message*)&(reflection->GetRepeatedMessage(*data, fd, j));
					if (child){
						setDBDataToSocketDataVo(child, sd[name][j]);
					}
					else{
						break;
					}
				}
			}
			else{
				child = reflection->MutableMessage(data,fd);
				setDBDataToSocketDataVo(child, sd[name]);
			}
			break;
		case google::protobuf::FieldDescriptor::TYPE_BYTES:
			sd[name] =reflection->GetString(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT32:
			sd[name] =reflection->GetUInt32(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_ENUM:
			sd[name] =reflection->GetEnum(*data, fd);
			break;
		case google::protobuf::FieldDescriptor::TYPE_SFIXED32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SFIXED64:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SINT32:

			break;
		case google::protobuf::FieldDescriptor::TYPE_SINT64:

			break;

		default:
			break;
		}
	}
}

void LoginInfo::setDBDataToSocketData(string tablename, ::google::protobuf::Message* data, YMSocketData &sd){
	string protoname;
	if (protoname.compare("userinfo") == 0){
		DBUserInfo user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("records") == 0){
		DBRecords user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("detail_records") == 0){
		DBDetailRecords user;
		protoname = user.GetTypeName();
	}
	else if (protoname.compare("room") == 0){
		DBRoom user;
		protoname = user.GetTypeName();
	}
	setDBDataToSocketDataVo(data, sd);
}
