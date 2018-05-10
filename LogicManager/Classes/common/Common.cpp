#include "Common.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<netdb.h>
#include <netinet/in.h>
#endif
	
Common::Common(){

}

Common::~Common() {

}

byte*	Common::replace(byte *&str, int beginpos, int len, byte b){
	for (int i = beginpos; i < beginpos + len;i++){
		str[i] = b;
	}
	return str;
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
	replace_all(ss, old_value, new_value);
	memcpy(str,ss.c_str(),ss.length());
	return ss;
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

int64_t Common::getCurrentTime()      //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以  
{
#define EPOCHFILETIME   (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
	tt = (li.QuadPart - EPOCHFILETIME) / 10;
	return tt;
}

time_t Common::getTime(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	return tt;
}

string Common::getTimeStr(time_t tp){
	char buff[100];
	time_t tt = tp;//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%04d%02d%02d%02d%02d%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec
		);
	return buff;
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
	sprintf(buff, "%d-%02d-%02d %02d:%02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
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

string Common::getLocalTimeDay1(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%04d%02d%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday
		);
	return buff;
}

string Common::getHostNameIp(string hostname, unsigned int &port){

	hostent *phost = gethostbyname(hostname.c_str());
	in_addr addr;
	for (int i = 0;; i++)
	{
		char *p = phost->h_addr_list[i];
		if (p == NULL)
			break;
		memcpy(&addr.S_un.S_addr, p, phost->h_length);
		char* ip = inet_ntoa(addr);
		return ip;
	}
	return "";
}