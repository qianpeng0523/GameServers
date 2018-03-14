#include "DataBaseUserInfo.h"
#include "SqlControl.h"
#include "CSVDataInfo.h"

DataBaseUserInfo *DataBaseUserInfo::m_ins = NULL;
static string g_othersql[] = { "information_schema", "mysql", "test", "performance_schema" };
// string DataBaseUserInfo::g_dbitennames[12] = { "userid", "username", "sex", "address", "gold", "diamond", "card", "code", "token", "picid", "unionid", "picurl" };
// string DataBaseUserInfo::g_dbrecordsnames[4] = { "id", "roomid", "rtype", "createtime"};
// string DataBaseUserInfo::g_dbdetailrecordsnames[5] = { "id", "fkey", "userid", "score", "win"};

DataBaseUserInfo::DataBaseUserInfo(){
	m_maxuid = "100000";
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
		YMSocketData dbuser;
		dbuser["userid"] = m_maxuid;
		dbuser["username"] = name;
		sprintf(buff, "%d", atoi(m_maxuid.c_str()) + 1);
		m_maxuid = buff;
		insertDBData(dbuser,"userinfo");
	}
}

bool DataBaseUserInfo::insertDBData(YMSocketData sd, string tablename){
	printf("ddd:%s",sd.getJsonString().c_str());
	string sqlstr1 = "select * from "+tablename +" where ";
	string sqlstr = "insert into ";
	sqlstr += tablename;
	sqlstr += "(";

	vector<string >conames = getTableColumnName(tablename);
	for (int i = 0; i < conames.size(); i++){
		sqlstr += conames.at(i);
		if (i < conames.size() - 1){
			sqlstr += ",";
		}
	}
	sqlstr += ")";
	sqlstr += " values(";
	char buff[2048];
	char buff1[2048];
	string key;
	string keyvalue;
	
	int sz = conames.size();
	for (int i = 0; i < sz; i++){
		string coname = conames.at(i);
		if (sd.isMember("data")){
			if (sd["data"].isMember(coname)){
				ColumnType type = getColumnType(tablename, coname);
				if (type == String_Type){
					sprintf(buff, " '%s' ",  sd["data"][coname].asString().c_str());
					sprintf(buff1, "%s= '%s' ",coname.c_str(), sd["data"][coname].asString().c_str());
				}
				else if (type == Int_Type || type == Bool_Type){
					sprintf(buff, "%d ",  sd["data"][coname].asInt());
					sprintf(buff1, "%s=%d ",coname.c_str(), sd["data"][coname].asInt());
				}
				else if (type == Float_Type || type == Double_Type){
					sprintf(buff, "%g ", sd["data"][coname].asInt());
					sprintf(buff1, "%s=%g ",coname.c_str(), sd["data"][coname].asInt());
				}
				sqlstr += buff;
				sqlstr1 += buff1;
				if (i < sz - 1){
					sqlstr += ",";
					sqlstr1 += " and ";
				}
				else{
					sqlstr += ")";
				}
			}
		}
		else{
			if (sd.isMember(coname)){
				ColumnType type = getColumnType(tablename, coname);
				if (type == String_Type){
					sprintf(buff, "%s='%s' ", coname.c_str(), sd[coname].asString().c_str());
					sprintf(buff1, "%s= '%s' ", coname.c_str(), sd[coname].asString().c_str());
				}
				else if (type == Int_Type || type == Bool_Type){
					sprintf(buff, "%s=%d ", coname.c_str(), sd[coname].asInt());
					sprintf(buff1, "%s=%d ", coname.c_str(), sd[coname].asInt());
				}
				else if (type == Float_Type || type == Double_Type){
					sprintf(buff, "%s=%g ", coname.c_str(), sd[coname].asInt());
					sprintf(buff1, "%s=%g ", coname.c_str(), sd[coname].asInt());
				}
				sqlstr += buff;
				sqlstr1 += buff1;
				if (i < sz - 1){
					sqlstr += ",";
					sqlstr1 += " and ";
				}
				else{
					sqlstr += ")";
				}
			}
		}
		
	}
	map<string,string> vec;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, insert_sql);
	if (err == 0){
		//更新数据
		vector<map<string, string>> vec1;
		err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr1.c_str(), vec1, select_sql);
		if (err == 0){
			for (int i = 0; i < vec1.size(); i++){
				setDBData(vec1.at(i), tablename);
			}
		}
	}
	
	return true;
}

ColumnType DataBaseUserInfo::getColumnType(string tablename, string name){
	vector<string>tableconames = getTableColumnName(tablename);
	vector<string>tabletypes = getTableColumnType(tablename);
	for (int i = 0; i < tableconames.size(); i++){
		if (name.compare(tableconames.at(i)) == 0){
			string type = tabletypes.at(i);
			if (type.compare("varchar") == 0){
				return String_Type;
			}
			else if (type.compare("int") == 0){
				return Int_Type;
			}
			else if (type.compare("float") == 0){
				return Float_Type;
			}
			else if (type.compare("double") == 0){
				return Double_Type;
			}
			else if (type.compare("bool") == 0){
				return Bool_Type;
			}
			else{
				return String_Type;
			}
		}
	}
	return String_Type;
}

int DataBaseUserInfo::updateDBDataByKey(string tablename,std::map<string, string >updatedata, string key, string value){
	string sqlstr = "update ";
	string sqlstr1 = "select * from " + tablename + " where ";
	sqlstr += tablename;
	sqlstr += " set ";
	char buff[255];
	char buff1[255];
	int ct = 0;
	std::map<string, string>::iterator itr1 = updatedata.begin();
	for (itr1; itr1 != updatedata.end(); itr1++){
		string name = itr1->first;
		sqlstr += name + "= ";

		ColumnType type = getColumnType(tablename, name);
		if (type == String_Type){
			sqlstr += "'";
			sqlstr += (char*)itr1->second.c_str();
			sqlstr += "'";

			sprintf(buff1, " %s=%s ",itr1->first.c_str(), itr1->second.c_str());
		}
		else if (type == Int_Type || type == Bool_Type){
			sprintf(buff, "%d", atoi(itr1->second.c_str()));
			sqlstr += buff;

			sprintf(buff1, " %s=%d ", itr1->first.c_str(), atoi(itr1->second.c_str()));
		}
		else if (type == Float_Type || type == Double_Type){
			sprintf(buff, "%g", atoi(itr1->second.c_str()));
			sqlstr += buff;

			sprintf(buff1, " %s=%g ", itr1->first.c_str(), atof(itr1->second.c_str()));
		}
		sqlstr1 += buff1;
		sqlstr1 += " and ";
		ct++;
		if (ct != updatedata.size()){
			sqlstr += ",";
		}
		else{
			sqlstr += " ";
		}
	}
	sqlstr += " where " + key + " = ";
	ColumnType type = getColumnType(tablename, key);
	if (type == String_Type){
		sqlstr += "'";
		sqlstr += (char*)value.c_str();
		sqlstr += "'";

		sprintf(buff1, " %s=%s ", key.c_str(), itr1->second.c_str());
	}
	else if (type == Int_Type || type == Bool_Type){
		sprintf(buff, "%d", atoi(value.c_str()));
		sqlstr += buff;

		sprintf(buff1, " %s=%d ", key.c_str(), atoi(itr1->second.c_str()));
	}
	else if (type == Float_Type || type == Double_Type){
		sprintf(buff, "%g", atoi(value.c_str()));
		sqlstr += buff;

		sprintf(buff1, " %s=%g ", key.c_str(), atof(itr1->second.c_str()));
	}
	sqlstr1 += buff1;
	map<string,string> vecs;
	int err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vecs, update_sql);
	if (err == 0){
		//更新数据
		
		vector<map<string, string>> vec1;
		err = SqlControl::getIns()->ExcuteQuery((char *)sqlstr1.c_str(), vec1, select_sql);
		if (err == 0){
			for (int i = 0; i < vec1.size(); i++){
				setDBData(vec1.at(i), tablename);
			}
		}
	}
	
	return err;
}

YMSocketData DataBaseUserInfo::setDBData(map<string, string> vecs, string tablename){
	string prikey=getTablePrikey(tablename);
	string prikeyvalue;
	YMSocketData sd;
	map<string, string>::iterator itr = vecs.begin();
	for (itr; itr != vecs.end(); itr++){
		string coname = itr->first;
		string covalue = itr->second;
		if (prikey.compare(coname) == 0){
			prikeyvalue = covalue;
		}

		ColumnType type = getColumnType(tablename, coname);
		if (type == String_Type){
			sd[coname] = covalue;
		}
		else if (type == Int_Type || type == Bool_Type){
			sd[coname] = atoi(covalue.c_str());
		}
		else if (type == Float_Type || type == Double_Type){
			sd[coname] = atof(covalue.c_str());
		}

	}
	setDBData(sd, tablename);
	return sd;
}

void DataBaseUserInfo::setDBData(YMSocketData sd, string tablename){
	string key = getTablePrikey(tablename);
	string keyvalue;
	if (sd.isMember(key)){
		ColumnType type = getColumnType(tablename, key);
		if (type == String_Type){
			keyvalue = sd[key].asString();
		}
		else if (type == Int_Type || type == Bool_Type){
			char buff[50];
			sprintf(buff, "%d", sd[key].asInt());
			keyvalue = buff;
		}
		else if (type == Float_Type || type == Double_Type){
			char buff[50];
			sprintf(buff, "%g", sd[key].asInt());
			keyvalue = buff;
		}

	}
	if (m_dbdatas.find(tablename) != m_dbdatas.end()){
		map<string, YMSocketData> dbs=m_dbdatas.at(tablename);
		if (dbs.find(key) != dbs.end()){
			dbs.at(key) = sd;
		}
		else{
			dbs.insert(make_pair(key,sd));
		}
		m_dbdatas.at(tablename) = dbs;
	}
	else{
		map<string, YMSocketData> dbs;
		dbs.insert(make_pair(key,sd));
		m_dbdatas.insert(make_pair(tablename, dbs));
	}
}

bool DataBaseUserInfo::getDBUser(char *uid, YMSocketData &dbuser){
// 	if (m_dbusers.find(uid) != m_dbusers.end()){
// 		dbuser = m_dbusers.at(uid);
// 		return true;
// 	}
	return false;
}

void DataBaseUserInfo::getAllDBData(string tablename){
	std::string sqlstr = "select * from ";
	sqlstr += tablename;
	vector <map<string, string>> vecs;
	SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(),vecs);
	for (int i = 0; i < vecs.size();i++){
		setDBData(vecs.at(i),tablename);
	}
	printf("所有数据查询完毕!!\n");
}

map<string, YMSocketData> DataBaseUserInfo::getDBDatas(string tablename){
	std::map<string, YMSocketData > copyvec;
	if (m_dbdatas.find(tablename)==m_dbdatas.end()){
		getAllDBData(tablename);
		if (m_dbdatas.find(tablename) != m_dbdatas.end()){
			copyvec = m_dbdatas.at(tablename);
		}
	}
	else{
		copyvec = m_dbdatas.at(tablename);
	}

	std::map<string, YMSocketData > vecs;
	int count = 0;
	std::map<string, YMSocketData >::iterator itr = copyvec.begin();
	for (itr; itr != copyvec.end(); itr++){
		vecs.insert(make_pair(itr->first, itr->second));
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
		SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(),vec,column_sql);
		if (!vec.empty()){
			m_tablecolumnsname.insert(make_pair(tablename,vec));
		}
	}
	
	return vec;
}

vector<string> DataBaseUserInfo::getTableColumnType(string tablename){
	vector<string> vec;
	if (m_tablecolumnstype.find(tablename) != m_tablecolumnstype.end()){
		vec = m_tablecolumnstype.at(tablename);
	}
	else{
		string sqlstr = "select DATA_TYPE from information_schema.columns where table_name='";
		sqlstr += tablename;
		sqlstr += "'";
		SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(), vec, column_sql);
		if (!vec.empty()){
			m_tablecolumnstype.insert(make_pair(tablename, vec));
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
		vector<string> vec1;
		SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(),vec1, column_sql);
		if (!vec1.empty()){
			prikey = vec1.at(0);
		}
		m_tableprikey.insert(make_pair(tablename,prikey));
	}
	return prikey;
}

std::vector<YMSocketData> DataBaseUserInfo::getDBData(string tablename, string coname, string covalue) {
	std::vector<YMSocketData > msgs;
	ColumnType type = getColumnType(tablename, coname);
	bool ist = false;
	if (m_dbdatas.find(tablename) != m_dbdatas.end()){
		map<string, YMSocketData> users = m_dbdatas.at(tablename);
		char buff[100];
		if (users.find(covalue) != users.end()){
			YMSocketData sd1 = users.at(covalue);
			if (type == String_Type){
				sprintf(buff,"%s",sd1[coname].asString().c_str());
			}
			else if (type == Int_Type || type == Bool_Type){
				sprintf(buff, "%d", sd1[coname].asInt());
			}
			else if (type == Float_Type || type == Double_Type){
				sprintf(buff, "%g", sd1[coname].asDouble());
			}
			if (covalue.compare(buff) == 0){
				msgs.push_back(sd1);
			}
			ist = true;
		}
	}
	if(!ist){
		string sqlstr = "select * from ";
		sqlstr += tablename;
		sqlstr += " where " + coname + " = ";

		if (type == String_Type){
			sqlstr += "'";
			sqlstr += covalue;
			sqlstr += "'";
		}
		else if (type == Int_Type || type == Bool_Type){
			sqlstr += covalue;
		}
		else if (type == Float_Type || type == Double_Type){
			sqlstr += covalue;
		}

		vector<map<string, string>> vec;
		SqlControl::getIns()->ExcuteQuery((char *)sqlstr.c_str(), vec, select_sql);
		
		for (int i = 0; i < vec.size(); i++){
			msgs.push_back(setDBData(vec.at(i), tablename));
		}
	}
	return msgs;
}


vector<string> DataBaseUserInfo::getAllDatabases(){
	string sqlstr = "show databases";
	vector<string> vecs;
	int err = SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(), vecs, showdatatses_sql);
	if (err == 0 && !vecs.empty()){
		for (int i = 0; i < 4; i++){
			string oname = g_othersql[i];
			vector<string>::iterator itr = vecs.begin();
			for (itr; itr != vecs.end(); itr++){
				if (oname.compare(*itr) == 0){
					vecs.erase(itr);
					break;
				}
			}
		}
		DataBaseUserInfo::getIns()->setDatabases(vecs);
	}
	return vecs;
}

vector<string> DataBaseUserInfo::getAllTables(){
	string sqlstr = "show tables";
	vector<string> vecs;
	int err = SqlControl::getIns()->ExcuteQuery1((char *)sqlstr.c_str(), vecs, showdatatses_sql);
	if (err == 0){
		DataBaseUserInfo::getIns()->setdbtables(vecs);
	}
	return vecs;
}