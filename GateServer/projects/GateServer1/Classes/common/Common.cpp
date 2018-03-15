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