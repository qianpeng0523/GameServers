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
		DBUserInfo *info = (DBUserInfo *)getDBDataFromSocketData("userinfo", sd["data"]);
		cl.set_allocated_info(info);
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
		DBUserInfo *info = (DBUserInfo *)getDBDataFromSocketData("userinfo", sd["data"]);
		cl.set_allocated_info(info);
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
















































::google::protobuf::Message * LoginInfo::getDBDataFromSocketData(string tablename, CSJson::Value sd){
	if (tablename.compare("userinfo") == 0){
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
		::google::protobuf::Message *user1 = ccEvent::create_message(user.GetTypeName());
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
	else if (tablename.compare("records") == 0){
		DBRecords record;
		::google::protobuf::Message *record1 = ccEvent::create_message(record.GetTypeName());

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
	else if (tablename.compare("detail_records") == 0){
		DBDetailRecords de_record;
		::google::protobuf::Message *de_record1 = ccEvent::create_message(de_record.GetTypeName());
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
	else if (tablename.compare("notice") == 0){
		DBNotice notice;
		::google::protobuf::Message *noticemsg = ccEvent::create_message(notice.GetTypeName());
		int id = sd["id"].asInt();
		string notice1 = sd["notice1"].asString();
		string notice2 = sd["notice2"].asString();
		notice.set_id(id);
		notice.set_notice1(notice1);
		notice.set_notice2(notice2);
		noticemsg->CopyFrom(notice);
		return noticemsg;
	}
	else if (tablename.compare("room") == 0){
		DBRoom room;
		::google::protobuf::Message *room1 = ccEvent::create_message(room.GetTypeName());
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


void LoginInfo::setDBDataToSocketData(string tablename, ::google::protobuf::Message* data, YMSocketData &sd){
	if (tablename.compare("userinfo") == 0){
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
	else if (tablename.compare("records") == 0){
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
	else if (tablename.compare("detail_records") == 0){
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
	else if (tablename.compare("notice") == 0){
		DBNotice notice;
		notice.CopyFrom(*data);
		int id = notice.id();
		string notice1 = notice.notice1();
		string notice2 = notice.notice2();

		sd["notice1"] = notice1;
		sd["notice2"] = notice2;

	}
	else if (tablename.compare("room") == 0){
		DBRoom room;
		room.CopyFrom(*data);
		int id = room.id();
		int round = room.round();
		int ante = room.ante();
		string remark = room.remark();
		int number = room.number();
		int piao = room.piao();
		int laizi = room.laizi();

		sd["id"] = id;
		sd["round"] = round;
		sd["ante"] = ante;
		sd["remark"] = remark;
		sd["number"] = number;
		sd["piao"] = piao;
		sd["laizi"] = laizi;
	}
}
