#include "DataBaseUserInfo.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"

DataBaseUserInfo *DataBaseUserInfo::m_ins = NULL;

string DataBaseUserInfo::g_dbitennames[12] = { "userid", "username", "sex", "address", "gold", "diamond", "card", "code", "token", "picid", "unionid", "picurl" };

DataBaseUserInfo::DataBaseUserInfo(){
	m_maxuid = "100000";
	string sqlstr = "select userid from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where userid = ";
	sqlstr += "(select max(userid) from userinfo)";
	std::vector<std::string> vecs1;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vecs1);
	if (!vecs1.empty()){
		char buff[100];
		sprintf(buff, "%d", atoi(m_maxuid.c_str()) + 1);
		m_maxuid = buff;
	}

	printf("maxuid:%s\n",m_maxuid.c_str());
	
	//updateDBUserInfoByUid("gold",(void *)80000,"100000");
	getAllDBUserInfo();
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
		insertDBUserInfo(dbuser);
	}
}

bool DataBaseUserInfo::insertDBUserInfo(DBUserInfo dbuser){
	string sqlstr = "insert into ";
	sqlstr += MJ_TABLENAME_USER;
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
	string uid = dbuser.userid();
	sprintf(buff, "'%s','%s',%d,'%s',%d,%d,%d,'%s','%s',%d,'%s','%s')", uid.c_str(), dbuser.username().c_str(), dbuser.sex(), dbuser.address().c_str(),
		dbuser.gold(), dbuser.diamon(),dbuser.card(),  dbuser.code().c_str(), dbuser.token().c_str(), dbuser.picid(), dbuser.unionid().c_str(), dbuser.picurl().c_str());
	sqlstr += buff;
	vector<string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, insert_sql);
	if (err == 0){
		setDBUserData(getDBUserData((char *)uid.c_str()));
	}
	
	return true;
}

bool DataBaseUserInfo::updateDBUserInfo(DBUserInfo dbuser, std::map<string, void *>maps){
	string sqlstr = "update ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " set ";
	
	char buff[2048];
	string uid = dbuser.userid();
	sprintf(buff, "%s='%s',%s='%s',%s=%d,%s='%s',%s=%d,%s=%d,%s=%d,%s='%s',%s='%s',%s=%d,%s='%s',%s='%s' ", 
		g_dbitennames[0].c_str(), uid.c_str(), g_dbitennames[1].c_str(), dbuser.username().c_str(), g_dbitennames[2].c_str(), dbuser.sex(), 
		g_dbitennames[3].c_str(), dbuser.address().c_str(),g_dbitennames[4].c_str(), dbuser.gold(), g_dbitennames[5].c_str(), dbuser.diamon(), g_dbitennames[6].c_str(), dbuser.card(),
		g_dbitennames[7].c_str(), dbuser.code().c_str(),g_dbitennames[8].c_str(), dbuser.token().c_str(), g_dbitennames[9].c_str(), dbuser.picid(),
		g_dbitennames[10].c_str(), dbuser.unionid().c_str(), g_dbitennames[11].c_str(), dbuser.picurl().c_str());
	sqlstr += buff;
	sqlstr += "where ";
	std::map<string, void *>::iterator itr = maps.begin();
	for (itr; itr != maps.end();itr++){
		string name = itr->first;
		sqlstr += name+"= ";
		if (name.compare(g_dbitennames[0]) == 0 || name.compare(g_dbitennames[1]) == 0 || name.compare(g_dbitennames[3]) == 0
			|| name.compare(g_dbitennames[7]) == 0 || name.compare(g_dbitennames[8]) == 0 || name.compare(g_dbitennames[10]) == 0
			||name.compare(g_dbitennames[11]) == 0){
			sqlstr += "'";
			sqlstr += (char*)itr->second;
			sqlstr += "'";
		}
		else{
			sprintf(buff,"%d",(int)itr->second);
			sqlstr += buff;
		}
	}
	vector<string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, insert_sql);
	if (err == 0){
		setDBUserData(getDBUserData((char *)uid.c_str()));
	}
	maps.clear();
	return true;
}

int DataBaseUserInfo::updateDBUserInfoByKey(std::map<string, string >updatedata, string key, string value){
	string sqlstr = "update ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " set ";
	char buff[255];
	int ct = 0;
	std::map<string, string>::iterator itr1 = updatedata.begin();
	for (itr1; itr1 != updatedata.end(); itr1++){
		string name = itr1->first;
		sqlstr += name + "= ";
		if (name.compare(g_dbitennames[0]) == 0 || name.compare(g_dbitennames[1]) == 0 || name.compare(g_dbitennames[3]) == 0
			|| name.compare(g_dbitennames[7]) == 0 || name.compare(g_dbitennames[8]) == 0 || name.compare(g_dbitennames[10]) == 0
			|| name.compare(g_dbitennames[11]) == 0){
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

	if (key.compare(g_dbitennames[0]) == 0 || key.compare(g_dbitennames[1]) == 0 || key.compare(g_dbitennames[3]) == 0
		|| key.compare(g_dbitennames[7]) == 0 || key.compare(g_dbitennames[8]) == 0 || key.compare(g_dbitennames[10]) == 0
		|| key.compare(g_dbitennames[11]) == 0){
		sqlstr += (char*)value.c_str();
		sqlstr += "'";
	}
	else{
		sprintf(buff, "%d", atoi(value.c_str()));
		sqlstr += buff;
	}

	vector<string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, update_sql);
	if (err == 0){
		if (key.compare("userid") == 0){
			setDBUserData(getDBUserData((char *)value.c_str()));
		}
		else{
			getAllDBUserInfo();
		}
	}
	
	return err;
}

bool DataBaseUserInfo::updateDBUserInfoByUid(std::map<string, void *>updatedata, string uid){
	string sqlstr = "update ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " set ";
	char buff[255];
	int ct = 0;
	std::map<string, void *>::iterator itr1 = updatedata.begin();
	for (itr1; itr1 != updatedata.end(); itr1++){
		string name = itr1->first;
		sqlstr += name + "= ";
		if (name.compare(g_dbitennames[0]) == 0 || name.compare(g_dbitennames[1]) == 0 || name.compare(g_dbitennames[3]) == 0
			|| name.compare(g_dbitennames[7]) == 0 || name.compare(g_dbitennames[8]) == 0 || name.compare(g_dbitennames[10]) == 0
			|| name.compare(g_dbitennames[11]) == 0){
			sqlstr += "'";
			sqlstr += (char*)itr1->second;
			sqlstr += "'";
		}
		else{
			sprintf(buff, "%d", (int)itr1->second);
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
	sqlstr += " where userid = '"+uid+"'";
	
	vector<string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, update_sql);
	if (err == 0){
		setDBUserData(getDBUserData((char *)uid.c_str()));
	}
	updatedata.clear();
	return true;
}

bool DataBaseUserInfo::updateDBUserInfoByUid(string upname, void *upvalue, string uid){
	string sqlstr = "update ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " set ";
	sqlstr += upname+"=";
	char buff[255];
	if (upname.compare(g_dbitennames[0]) == 0 || upname.compare(g_dbitennames[1]) == 0 || upname.compare(g_dbitennames[3]) == 0
		|| upname.compare(g_dbitennames[7]) == 0 || upname.compare(g_dbitennames[8]) == 0 || upname.compare(g_dbitennames[10]) == 0
		|| upname.compare(g_dbitennames[11]) == 0){
		sqlstr += "'";
		sqlstr += (char*)upvalue;
		sqlstr += "'";
	}
	else{
		sprintf(buff, "%d", (int)upvalue);
		sqlstr += buff;
	}
	sqlstr += " where userid = '" + uid + "'";
	vector<string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, update_sql);
	if (err == 0){
		setDBUserData(getDBUserData((char *)uid.c_str()));
	}
	return true;
}

DBUserInfo DataBaseUserInfo::setDBUserData(std::vector<std::string> vecs){
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
	return dbuser;
}

void DataBaseUserInfo::setDBUserData(DBUserInfo dbuser){
	string uid = dbuser.userid();
	if (m_dbusers.find(uid) != m_dbusers.end()){
		m_dbusers.at(uid) = dbuser;
	}
	else{
		m_dbusers.insert(make_pair(uid, dbuser));
	}
}

bool DataBaseUserInfo::getDBUser(char *uid, DBUserInfo &dbuser){
	if (m_dbusers.find(uid) != m_dbusers.end()){
		dbuser = m_dbusers.at(uid);
		return true;
	}
	return false;
}

DBUserInfo DataBaseUserInfo::getDBUserInfo(char *uid){
	DBUserInfo dbuser;
	bool ish = getDBUser(uid,dbuser);
	if (!ish){
		std::vector<std::string> vecs = getDBUserData(uid);
		dbuser = setDBUserData(vecs);
	}
	printf("%s \n", dbuser.DebugString().c_str());
	return dbuser;
}

std::vector<std::string> DataBaseUserInfo::getDBUserData(char *uid){
	std::string sqlstr = "select *from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where userid = ";
	sqlstr += uid;
	std::vector<std::string> vecs;
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vecs);
	return vecs;
}

void DataBaseUserInfo::getAllDBUserInfo(){
	std::string sqlstr = "select * from ";
	sqlstr += MJ_TABLENAME_USER;
	vector < vector < string >> vecs;
	SqlControl::getIns()->ExcuteQueryAll((char *)sqlstr.c_str(),vecs);
	for (int i = 0; i < vecs.size();i++){
		setDBUserData(vecs.at(i));
	}
	printf("所有数据查询完毕!!\n");
}

std::map<string, DBUserInfo> DataBaseUserInfo::getUserInfoDatas(){
	if (m_dbusers.empty()){
		getAllDBUserInfo();
	}
	std::map<string, DBUserInfo> vecs;
	int count = 0;
	std::map<string, DBUserInfo>::iterator itr = m_dbusers.begin();
	for (itr; itr != m_dbusers.end();itr++){
		vecs.insert(make_pair(itr->first,itr->second));
		count++;
		if (count == 10){
			break;
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
		SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec,column_sql);
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
		vector<string> vec1;
		SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec1, column_sql);
		if (!vec1.empty()){
			prikey = vec1.at(0);
		}
	}
	return prikey;
}

DBUserInfo DataBaseUserInfo::getDBUserInfo(string coname, string covalue){
	string sqlstr = "select * from userinfo where "+coname+" = ";
	if (coname.compare(g_dbitennames[0]) == 0 || coname.compare(g_dbitennames[1]) == 0 || coname.compare(g_dbitennames[3]) == 0
		|| coname.compare(g_dbitennames[7]) == 0 || coname.compare(g_dbitennames[8]) == 0 || coname.compare(g_dbitennames[10]) == 0
		|| coname.compare(g_dbitennames[11]) == 0){
		sqlstr += "'";
		sqlstr += covalue;
		sqlstr += "'";
	}
	else{
		sqlstr += covalue;
	}
	vector<string> vec;
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vec,select_sql);
	DBUserInfo user = setDBUserData(vec);
	return user;
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