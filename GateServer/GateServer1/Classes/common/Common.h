#ifndef __Common_H__
#define __Common_H__

#include <string>
#include "stdio.h"
#include <vector>
#include "stdafx.h"
#include <time.h>
class Common {
public:
	Common();
	virtual ~Common();
	static std::string&   replace_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);//重复替换
	static std::string&   replace_all(char *str, const   std::string&   old_value, const   std::string&   new_value);//重复替换
	static byte*	replace(byte *&str, int beginpos, int len, byte b);
	static std::string&   replace_all_distinct(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);//只替换一次
	static std::vector<std::string> getStrings(std::string str,std::string findstr);
	static bool isHave(std::vector<std::string> vecs, std::string value);
	static string getTime(time_t tp);
	static string getLocalTime();
	static string getLocalTime1();
	static string getLocalTimeDay();
	static time_t getTime();
	static int64_t getCurrentTime();
	static string getLocalTimeDay1();
	static string getTimeStr(time_t tp);
	static int getTime(time_t ptimep, int type);//type 1年 2月 3日 4时 5分 6秒 
	static string getHostNameIp(string hostname,unsigned int &port);
};
#endif // __Common_H__
