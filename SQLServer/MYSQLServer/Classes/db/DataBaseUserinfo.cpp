#include "DataBaseUserInfo.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"

DataBaseUserInfo *DataBaseUserInfo::m_ins = NULL;

string g_dbitennames[] = { "id", "userid", "username", "sex", "address", "gold", "diamond", "card", "icon", "code", "token", "picid", "unionid", "picurl" };

DataBaseUserInfo::DataBaseUserInfo(){
	m_maxid = 0;
	m_maxuid = "100000";
	std::string sqlstr = "select id from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where id = ";
	sqlstr += "(select max(id) from userinfo)";
	std::vector<std::string> vecs = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str());
	if (!vecs.empty())
		m_maxid =atoi( (char *)vecs.at(0).c_str());

	sqlstr = "select userid from ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += " where userid = ";
	sqlstr += "(select max(userid) from userinfo)";
	std::vector<std::string> vecs1 = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str());
	if (!vecs.empty())
		m_maxuid = vecs1.at(0);

	printf("maxid:%d,maxuid:%s\n",m_maxid,m_maxuid.c_str());

	DBUserInfo dbuser;
	dbuser.set_id(m_maxid + 1);
	char buff[100];
	sprintf(buff,"%d",atoi(m_maxuid.c_str())+1);
	dbuser.set_userid(buff);
	insertDBUserInfo(dbuser);
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
	for (itr; itr != maps.end(); itr++){
		RobotData *rd = (RobotData *)itr->second;

	}
}

bool DataBaseUserInfo::insertDBUserInfo(DBUserInfo dbuser){
	string sqlstr = "insert into ";
	sqlstr += MJ_TABLENAME_USER;
	sqlstr += "(";
	for (int i = 0; i < 14; i++){
		sqlstr += g_dbitennames[i];
		if (i < 13){
			sqlstr += ",";
		}
	}
	sqlstr += ")";
	sqlstr += " values(";
	char buff[2048];
	string uid = dbuser.userid();
	sprintf(buff, "%d,'%s','%s',%d,'%s',%d,%d,%d,'%s','%s','%s',%d,'%s','%s')", dbuser.id(), uid.c_str(), dbuser.username().c_str(), dbuser.sex(), dbuser.address().c_str(),
		dbuser.gold(), dbuser.diamon(),dbuser.card(), dbuser.icon().c_str(), dbuser.code().c_str(), dbuser.token().c_str(), dbuser.picid(), dbuser.unionid().c_str(), dbuser.picurl().c_str());
	sqlstr += buff;
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),insert_sql);
	setDBUserData(getDBUserInfo((char *)uid.c_str()));
	return true;
}

DBUserInfo DataBaseUserInfo::setDBUserData(std::vector<std::string> vecs){
	DBUserInfo dbuser;
	if (!vecs.empty()){
		string uid = vecs.at(1);
		dbuser.set_id(atoi(vecs.at(0).c_str()));
		dbuser.set_userid(uid);
		dbuser.set_username(vecs.at(2));
		dbuser.set_sex(atoi(vecs.at(3).c_str()));
		dbuser.set_address(vecs.at(4));
		dbuser.set_gold(atoi(vecs.at(5).c_str()));
		dbuser.set_diamon(atoi(vecs.at(6).c_str()));
		dbuser.set_card(atoi(vecs.at(7).c_str()));
		dbuser.set_icon(vecs.at(8));
		dbuser.set_code(vecs.at(9));
		dbuser.set_token(vecs.at(10));
		dbuser.set_picid(atoi(vecs.at(11).c_str()));
		dbuser.set_unionid(vecs.at(12));
		dbuser.set_picurl(vecs.at(13));
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