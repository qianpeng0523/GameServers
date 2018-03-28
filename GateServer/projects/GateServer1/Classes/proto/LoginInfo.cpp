#include "LoginInfo.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "LibEvent.h"
#include "HttpLogic.h"
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
}

LoginInfo::~LoginInfo(){
	
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

void LoginInfo::SendSLogin(YMSocketData sd, int fd){
	SLogin cl;
	int err = sd["err"].asInt();
	cl.set_err(err);
	if (err == 0){
		printf("sd:%s",sd.getJsonString().c_str());
		SLogin *cl1= (SLogin *)getDBDataFromSocketDataVo(cl.GetTypeName(), sd);
		ClientData *data = LibEvent::getIns()->getClientData(fd);
		if (data){
			string ip = data->_ip;
			DBUserInfo *info = cl1->mutable_info();
			info->set_ip(ip);
		}
		cl.CopyFrom(*cl1);
	}
	LibEvent::getIns()->SendData(cl.cmd(),&cl,fd);
}

void LoginInfo::HandlerCLoginHand(ccEvent *event){
	CLogin cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();
	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion=uid+pwd+LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
	}
	printf("login:uid:%s,pwd:%s",uid.c_str(),pwd.c_str());
	HttpLogic::getIns()->requestDBData(uid, pwd);
	//SendSLogin(event->m_fd);
}


void LoginInfo::SendSRegister(YMSocketData sd, int fd){
	SRegister cl;
	int err = sd["err"].asInt();
	cl.set_err(err);
	if (err == 0){
		SRegister *cl1 = (SRegister *)getDBDataFromSocketDataVo(cl.GetTypeName(), sd["data"]);
		ClientData *data = LibEvent::getIns()->getClientData(fd);
		if (data){
			string ip = data->_ip;
			DBUserInfo *info = cl1->mutable_info();
			info->set_ip(ip);
		}
		cl.CopyFrom(*cl1);
	}
	
	LibEvent::getIns()->SendData(cl.cmd(), &cl, fd);
}

void LoginInfo::HandlerCRegister(ccEvent *event){
	CRegister cl;
	cl.CopyFrom(*event->msg);
	string uid = cl.uid();
	string pwd = cl.pwd();
	string uname = cl.uname();

	ClientData *data = LibEvent::getIns()->getClientData(event->m_fd);
	if (data){
		string seesion = uid + pwd + LOGIC_TOKEN;
		MD5 md5;
		md5.update(seesion);
		data->_sessionID = md5.toString();
	}

	DBUserInfo user;
	user.set_userid(uid);
	user.set_username(uname);
	YMSocketData sd;
	YMSocketData sd1;
	setDBDataToSocketData("userinfo",&user,sd1);
	sd1["pwd"] = pwd;
	sd["data"] = sd1;
	HttpLogic::getIns()->requestRegister(sd);
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
