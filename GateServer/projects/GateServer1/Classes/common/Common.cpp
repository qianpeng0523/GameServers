#include "Common.h"

	
Common::Common(){

}

Common::~Common() {

}

std::string& Common::replace_all(std::string& str, const std::string& old_value, const std::string& new_value)
{
	while (true)   {
		std::string::size_type   pos(0);
		if ((pos = str.find(old_value)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

std::string&   Common::replace_all(char *str, const   std::string&   old_value, const   std::string&   new_value){
	std::string ss=str;
	return replace_all(ss, old_value, new_value);
}

std::string& Common::replace_all_distinct(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
{
	for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length())   {
		if ((pos = str.find(old_value, pos)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

std::vector<std::string> Common::getStrings(std::string str, std::string findstr){
	std::vector<std::string> vecs;
	int index = str.find(findstr);
	int len = findstr.length();
	while (index != -1){
		std::string temp;
		temp = str.substr(0,index);
		if (!temp.empty()){
			vecs.push_back(temp);
		}
		str = str.substr(index + len, str.length());
		index = str.find(findstr);
	}
	vecs.push_back(str);
	return vecs;
}

bool Common::isHave(std::vector<std::string> vecs, std::string value){
	for (int i = 0; i < vecs.size(); i++){
		if (value == vecs.at(i)){
			return true;
		}
	}
	return false;
}


int Common::getTime(time_t ptimep, int type){
	struct   tm  * tm;
	tm = localtime(&ptimep);
	int  year = tm->tm_year + 1900;
	int  month = tm->tm_mon + 1;
	int  day = tm->tm_mday;
	int  hour = tm->tm_hour;
	int  min = tm->tm_min;
	int  second = tm->tm_sec;
	if (type == 1){
		return year;
	}
	else if (type == 2){
		return month;
	}
	else if (type == 3){
		return day;
	}
	else if (type == 4){
		return hour;
	}
	else if (type == 5){
		return min;
	}
	else if (type == 6){
		return second;
	}
	return ptimep;
}


time_t Common::getTime(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	return tt;
}

string Common::getTime(time_t tp){
	char buff[100];
	time_t tt = tp;//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d %02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min);
	return buff;
}

string Common::getLocalTime1(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%02d:%02d",
		t->tm_hour,
		t->tm_min);
	return buff;
}

string Common::getLocalTime(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d %02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min);
	return buff;
}

string Common::getLocalTimeDay(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday
		);
	return buff;
}
