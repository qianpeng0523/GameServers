#include "DataBaseUserInfo.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"

DataBaseUserInfo *DataBaseUserInfo::m_ins = NULL;

string DataBaseUserInfo::g_dbitennames[12] = { "userid", "username", "sex", "address", "gold", "diamond", "card", "code", "token", "picid", "unionid", "picurl" };
string DataBaseUserInfo::g_dbrecordsnames[4] = { "id", "roomid", "rtype", "createtime"};
string DataBaseUserInfo::g_dbdetailrecordsnames[5] = { "id", "fkey", "userid", "score", "win"};

DataBaseUserInfo::DataBaseUserInfo(){
	m_maxuid = "100000";
	string sqlstr = "select userid from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where userid = ";
	sqlstr += "(select max(userid) from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr+=")";
	vector<vector<string>> vecs1;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vecs1);
	if (!vecs1.empty()){
		char buff[100];
		sprintf(buff, "%d", atoi(m_maxuid.c_str()) + 1);
		m_maxuid = buff;
	}

	printf("maxuid:%s\n",m_maxuid.c_str());
	
	//updateDBUserInfoByUid("gold",(void *)80000,"100000");
	getAllDBData(MJ_TABLENAME_USER);
	//startAI();
}

DataBaseUserInfo::~DataBaseUserInfo(){

}

DataBaseUserInfo *DataBaseUserInfo::getIns(){
	if (!m_ins){
		m_ins = new DataBaseUserInfo();
	}
	return m_ins;
}

void DataBaseUserInfo::startAI(){
	CSVDataInfo::getIns()->openCSVFile("./res/robot.csv", CSV_ROBOT);
	std::map<int, Object *>maps = CSVDataInfo::getIns()->getDatas(CSV_ROBOT);
	std::map<int, Object *>::iterator itr = maps.begin();
	char buff[100];
	for (itr; itr != maps.end(); itr++){
		RobotData *rd = (RobotData *)itr->second;
		string name = "jq"+rd->_name;
		DBUserInfo dbuser;
		dbuser.set_userid(m_maxuid);
		dbuser.set_username(name);
		sprintf(buff, "%d", atoi(m_maxuid.c_str()) + 1);
		m_maxuid = buff;
		insertDBData(&dbuser,MJ_TABLENAME_USER);
	}
}

bool DataBaseUserInfo::insertDBData(::google::protobuf::Message *msg,string tablename){
	string sqlstr = "insert into ";
	sqlstr += tablename;
	sqlstr += "(";
	for (int i = 0; i < 11; i++){
		sqlstr += g_dbitennames[i];
		if (i < 10){
			sqlstr += ",";
		}
	}
	sqlstr += ")";
	sqlstr += " values(";
	char buff[2048];
	string key;
	string keyvalue;
	
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo dbuser;
		dbuser.CopyFrom(*msg);
		sprintf(buff, "'%s','%s',%d,'%s',%d,%d,%d,'%s','%s',%d,'%s','%s')", dbuser.userid().c_str(), dbuser.username().c_str(), dbuser.sex(), dbuser.address().c_str(),
			dbuser.gold(), dbuser.diamon(), dbuser.card(), dbuser.code().c_str(), dbuser.token().c_str(), dbuser.picid(), dbuser.unionid().c_str(), dbuser.picurl().c_str());
		sqlstr += buff;
		key = "userid";
		keyvalue = dbuser.userid();
	}
	else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords dbuser;
		dbuser.CopyFrom(*msg);
		sprintf(buff, "'%d','%s',%d,'%s')", dbuser.id(), dbuser.roomid().c_str(), dbuser.rtype(), dbuser.ctime().c_str());
		sqlstr += buff;
		key = "id";
		char buf[30];
		sprintf(buf,"%d",dbuser.id());
		keyvalue = buf;
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords dbuser;
		dbuser.CopyFrom(*msg);
		sprintf(buff, "'%d','%d',%s,'%d',%d)", dbuser.id(), dbuser.fkey(), dbuser.userid().c_str(), dbuser.score(),
			dbuser.win());
		sqlstr += buff;
		key = "id";
		char buf[30];
		sprintf(buf, "%d", dbuser.id());
		keyvalue = buf;
	}
	vector<vector<string>> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, insert_sql);
	if (err == 0){
		//更新数据
		
		vector<::google::protobuf::Message*> vec = getDBData(key, (char *)keyvalue.c_str(), tablename);
		for (int i = 0; i < vec.size(); i++){
			setDBData(vec.at(i), tablename);
		}
	}
	
	return true;
}

bool DataBaseUserInfo::isStringType(string tablename, string name){
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		if (name.compare(g_dbitennames[0]) == 0 || name.compare(g_dbitennames[1]) == 0 || name.compare(g_dbitennames[3]) == 0
			|| name.compare(g_dbitennames[7]) == 0 || name.compare(g_dbitennames[8]) == 0 || name.compare(g_dbitennames[10]) == 0
			|| name.compare(g_dbitennames[11]) == 0){
			return true;
		}
		else{
			return false;
		}
	}else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		if (name.compare(g_dbrecordsnames[1]) == 0 || name.compare(g_dbrecordsnames[4]) == 0){
			return true;
		}
		else{
			return false;
		}
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		if (name.compare(g_dbdetailrecordsnames[2]) == 0){
			return true;
		}
		else{
			return false;
		}
	}
}

int DataBaseUserInfo::updateDBDataByKey(string tablename,std::map<string, string >updatedata, string key, string value){
	string sqlstr = "update ";
	sqlstr += tablename;
	sqlstr += " set ";
	char buff[255];
	int ct = 0;
	std::map<string, string>::iterator itr1 = updatedata.begin();
	for (itr1; itr1 != updatedata.end(); itr1++){
		string name = itr1->first;
		sqlstr += name + "= ";
		bool istring = isStringType(tablename,name);
		if (istring){
			sqlstr += "'";
			sqlstr += (char*)itr1->second.c_str();
			sqlstr += "'";
		}
		else{
			sprintf(buff, "%d", atoi(itr1->second.c_str()));
			sqlstr += buff;
		}
		ct++;
		if (ct != updatedata.size()){
			sqlstr += ",";
		}
		else{
			sqlstr += " ";
		}
	}
	sqlstr += " where " + key + " = '";
	bool istring = isStringType(tablename, key);
	if (istring){
		sqlstr += (char*)value.c_str();
		sqlstr += "'";
	}
	else{
		sprintf(buff, "%d", atoi(value.c_str()));
		sqlstr += buff;
	}

	vector<vector<string>> vecs;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vecs, update_sql);
	if (err == 0){
		//更新数据
		vector<::google::protobuf::Message*> vec = getDBData(key, (char *)value.c_str(), tablename);
		for (int i = 0; i < vec.size(); i++){
			setDBData(vec.at(i), tablename);
		}
	}
	
	return err;
}

::google::protobuf::Message* DataBaseUserInfo::setDBData(std::vector<std::string> vecs, string tablename){
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo dbuser;
		if (!vecs.empty()){
			string uid = vecs.at(0);
			dbuser.set_userid(uid);
			dbuser.set_username(vecs.at(1));
			dbuser.set_sex(atoi(vecs.at(2).c_str()));
			dbuser.set_address(vecs.at(3));
			dbuser.set_gold(atoi(vecs.at(4).c_str()));
			dbuser.set_diamon(atoi(vecs.at(5).c_str()));
			dbuser.set_card(atoi(vecs.at(6).c_str()));
			dbuser.set_code(vecs.at(7));
			dbuser.set_token(vecs.at(8));
			dbuser.set_picid(atoi(vecs.at(9).c_str()));
			dbuser.set_unionid(vecs.at(10));
			dbuser.set_picurl(vecs.at(11));
			if (m_dbusers.find(uid) != m_dbusers.end()){
				m_dbusers.at(uid) = dbuser;
			}
			else{
				m_dbusers.insert(make_pair(uid.c_str(), dbuser));
			}
		}
		return &dbuser;
	}
	else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords dbrecord;
		return &dbrecord;
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords dbdetailrecord;
		return &dbdetailrecord;
	}
}

void DataBaseUserInfo::setDBData(::google::protobuf::Message* msg, string tablename){
	if (tablename.compare(MJ_TABLENAME_USER) == 0){
		DBUserInfo dbuser;
		dbuser.CopyFrom(*msg);
		string uid = dbuser.userid();
		if (m_dbusers.find(uid) != m_dbusers.end()){
			m_dbusers.at(uid) = dbuser;
		}
		else{
			m_dbusers.insert(make_pair(uid, dbuser));
		}
	}
	else if (tablename.compare(MJ_TABLENAME_RECORDS) == 0){
		DBRecords rec;
		rec.CopyFrom(*msg);
		int id = rec.id();
		if (m_dbrecordss.find(id) != m_dbrecordss.end()){
			m_dbrecordss.at(id) = rec;
		}
		else{
			m_dbrecordss.insert(make_pair(id, rec));
		}
	}
	else if (tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0){
		DBDetailRecords dbdroc;
		dbdroc.CopyFrom(*msg);
		int id = dbdroc.id();
		if (m_dbdetailrecords.find(id) != m_dbdetailrecords.end()){
			m_dbdetailrecords.at(id) = dbdroc;
		}
		else{
			m_dbdetailrecords.insert(make_pair(id, dbdroc));
		}
	}
}

bool DataBaseUserInfo::getDBUser(char *uid, DBUserInfo &dbuser){
	if (m_dbusers.find(uid) != m_dbusers.end()){
		dbuser = m_dbusers.at(uid);
		return true;
	}
	return false;
}

void DataBaseUserInfo::getAllDBData(string tablename){
	std::string sqlstr = "select * from ";
	sqlstr += tablename;
	vector < vector < string >> vecs;
	SqlControl::getIns()->ExcuteQueryAll((char *)sqlstr.c_str(),vecs);
	for (int i = 0; i < vecs.size();i++){
		setDBData(vecs.at(i),tablename);
	}
	printf("所有数据查询完毕!!\n");
}

std::map<string, ::google::protobuf::Message*> DataBaseUserInfo::getDBDatas(string tablename){
	if ((tablename.compare(MJ_TABLENAME_USER) == 0&&m_dbusers.empty()) ||
		(tablename.compare(MJ_TABLENAME_RECORDS) == 0&&m_dbrecordss.empty())||
		tablename.compare(MJ_TABLENAME_DETAIL_RECORDS) == 0 && m_dbdetailrecords.empty()){
		getAllDBData(tablename);
	}
	std::map<string, ::google::protobuf::Message*> vecs;
	int count = 0;
	if (tablename.compare(MJ_TABLENAME_USER)==0){
		std::map<string, DBUserInfo>::iterator itr = m_dbusers.begin();
		for (itr; itr != m_dbusers.end(); itr++){
			vecs.insert(make_pair(itr->first, &itr->second));
			count++;
			if (count == 10){
				break;
			}
		}
	}
	return vecs;
}

vector<string> DataBaseUserInfo::getTableColumnName(string tablename){
	vector<string> vec;
	if (m_tablecolumnsname.find(tablename) != m_tablecolumnsname.end()){
		vec = m_tablecolumnsname.at(tablename);
	}
	else{
		string sqlstr = "select COLUMN_NAME from information_schema.columns where table_name='";
		sqlstr += tablename;
		sqlstr += "'";
		vector< vector<string>> vec1;
		SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec1,column_sql);
		if (!vec1.empty() ){
			vec = vec1.at(0);
		}
	}
	
	return vec;
}

string DataBaseUserInfo::getTablePrikey(string tablename){
	string prikey;
	if (m_tableprikey.find(tablename) != m_tableprikey.end()){
		prikey = m_tableprikey.at(tablename);
	}
	else{
		string sqlstr = "select COLUMN_NAME from information_schema.key_column_usage where table_name = '";
		sqlstr += tablename;
		sqlstr += "'";
		vector< vector<string>> vec1;
		SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec1, column_sql);
		if (!vec1.empty()&&!vec1.at(0).empty()){
			prikey = vec1.at(0).at(0);
		}
	}
	return prikey;
}

std::vector<::google::protobuf::Message*> DataBaseUserInfo::getDBData(string tablename, string coname, string covalue) {
	string sqlstr = "select * from "; 
	sqlstr += tablename;
	sqlstr += " where " + coname + " = ";
	bool istring = isStringType(tablename,coname);
	if (istring){
		sqlstr += "'";
		sqlstr += covalue;
		sqlstr += "'";
	}
	else{
		sqlstr += covalue;
	}
	vector<vector<string>> vec;
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec,select_sql);
	std::vector<::google::protobuf::Message*> msgs;
	for (int i = 0; i < vec.size(); i++){
		msgs.push_back(setDBData(vec.at(i), tablename));
	}
	return msgs;
}

void DataBaseUserInfo::getDBUserFromSocketData(DBUserInfo &user, YMSocketData sd, string listname, int index){
	string uid, uname, add, code, token, unionid, picurl;
	int sex, gold, diamond, card, picid;
	if (listname.empty()){
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
	}
	else{
		uid = sd[listname][index]["userid"].asString();
		uname = sd[listname][index]["username"].asString();
		sex = sd[listname][index]["sex"].asInt();
		add = sd[listname][index]["address"].asString();
		gold = sd[listname][index]["gold"].asInt();
		diamond = sd[listname][index]["diamond"].asInt();
		card = sd[listname][index]["card"].asInt();
		code = sd[listname][index]["code"].asString();
		token = sd[listname][index]["token"].asString();
		picid = sd[listname][index]["picid"].asInt();
		unionid = sd[listname][index]["unionid"].asString();
		picurl = sd[listname][index]["picurl"].asString();
	}
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
}

void DataBaseUserInfo::setDBUserToSocketData(DBUserInfo user, YMSocketData &sd, string listname, int index){
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
	if (listname.empty()){
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
	else{
		sd[listname][index]["userid"] = uid;
		sd[listname][index]["username"] = uname;
		sd[listname][index]["sex"] = sex;
		sd[listname][index]["address"] = add;
		sd[listname][index]["gold"] = gold;
		sd[listname][index]["diamond"] = diamond;
		sd[listname][index]["card"] = card;
		sd[listname][index]["code"] = code;
		sd[listname][index]["token"] = token;
		sd[listname][index]["picid"] = picid;
		sd[listname][index]["unionid"] = unionid;
		sd[listname][index]["picurl"] = picurl;
	}

}