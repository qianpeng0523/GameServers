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

std::string&  Common::replace_all(char *& str, const   std::string&   old_value, const   std::string&   new_value){
	string tt = str;
	replace_all(tt,old_value,new_value);
	strcpy(str,tt.c_str());
	return tt;
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