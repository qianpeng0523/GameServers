#ifndef __Common_H__
#define __Common_H__

#include <string>
#include "stdio.h"
#include <vector>

class Common {
public:
	Common();
	virtual ~Common();
	static std::string&   replace_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);//重复替换
	static std::string&   replace_all(char *str, const   std::string&   old_value, const   std::string&   new_value);//重复替换
	static std::string&   replace_all_distinct(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);//只替换一次
	static std::vector<std::string> getStrings(std::string str,std::string findstr);
	static bool isHave(std::vector<std::string> vecs, std::string value);
};
#endif // __Common_H__
