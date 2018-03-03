#include "DataBaseUserInfo.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"

DataBaseUserInfo *DataBaseUserInfo::m_ins = NULL;

string g_dbitennames[] = { "userid", "username", "sex", "address", "gold", "diamond", "card",  "code", "token", "picid", "unionid", "picurl" };

DataBaseUserInfo::DataBaseUserInfo(){
	m_maxuid = "100000";
	string sqlstr = "select userid from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where userid = ";
	sqlstr += "(select max(userid) from userinfo)";
	std::vector<std::string> vecs1 = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str());
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
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),insert_sql);
	setDBUserData(getDBUserInfo((char *)uid.c_str()));
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
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), update_sql);
	setDBUserData(getDBUserInfo((char *)uid.c_str()));
	maps.clear();
	return true;
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
	
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), update_sql);
	setDBUserData(getDBUserInfo((char *)uid.c_str()));
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
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), update_sql);
	setDBUserData(getDBUserInfo((char *)uid.c_str()));
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
	std::vector<std::string> vecs = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str());
	return vecs;
}

void DataBaseUserInfo::getAllDBUserInfo(){
	std::string sqlstr = "select * from ";
	sqlstr += MJ_TABLENAME_USER;
	vector < vector < string >> vecs= SqlControl::getIns()->ExcuteQueryAll((char *)sqlstr.c_str());
	for (int i = 0; i < vecs.size();i++){
		setDBUserData(vecs.at(i));
	}
	printf("所有数据查询完毕!!\n");
}

std::map<string, DBUserInfo> DataBaseUserInfo::getUserInfoDatas(){
// 	std::string sqlstr = "select top(10) * from ";
// 	sqlstr += MJ_TABLENAME_USER;
// 	sqlstr += " where 1=1";
// 	vector < vector < string >> vecs = SqlControl::getIns()->ExcuteQueryAll((char *)sqlstr.c_str());
// 	for (int i = 0; i < vecs.size(); i++){
// 		setDBUserData(vecs.at(i));
// 	}
// 	printf("所有前10条数据查询完毕!!\n");
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