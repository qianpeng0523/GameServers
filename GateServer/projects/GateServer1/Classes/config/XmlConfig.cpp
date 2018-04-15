#include "XmlConfig.h"
#include "HttpEvent.h"
#include "Common.h"
XmlConfig *XmlConfig::m_Ins = NULL;



XmlConfig::XmlConfig(){
	//this->scheduleUpdate(0.5, 1.0);
}
XmlConfig::~XmlConfig(){
	
}

bool XmlConfig::init()
{
	
    return true;
}

XmlConfig *XmlConfig::getIns(){
	if (!m_Ins){
		m_Ins = new XmlConfig();
		m_Ins->init();
	}
	return m_Ins;
}

string XmlConfig::setXmlData1(map<string, string> valuemap){
	//string xml = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>\n";
	string xml = "<xml>\n";
	for (auto itr = valuemap.begin(); itr != valuemap.end();itr++){
		string name = itr->first;
		string value = itr->second;
		xml += "<"+name+">";
		xml += value;
		xml += "</" + name + ">\n";
	}
	xml += "</xml>\n";
	printf("%s\n", xml.c_str());
	return XXIconv::GBK2UTF(xml.c_str());
}

string XmlConfig::setXmlData(map<string, string> valuemap){
	//string xml = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>\n";
	string xml = "<xml>\n";
	for (auto itr = valuemap.begin(); itr != valuemap.end(); itr++){
		string name = itr->first;
		string value = itr->second;
		xml += "<" + name + ">";
		xml += value;
		xml += "</" + name + ">\n";
	}
	xml += "</xml>\n";
	printf("%s\n", xml.c_str());
	return XXIconv::GBK2UTF(xml.c_str());
}

map<string, string> XmlConfig::parseXmlData(string content){
	Common::replace_all(content, "\n", "");
	string tt = "<![CDATA[";
	string t2 = "]]>";
	Common::replace_all(content, tt, "");
	Common::replace_all(content, t2, "");
	string temp = content;
	int index = temp.find("<xml>");
	temp = temp.substr(index + 5, temp.length());
	map<string, string> mm;
	while (index != -1){
		index=temp.find(">");
		if (index == -1){
			break;
		}
		string name = temp.substr(1,index-1);
		temp = temp.substr(index+1, temp.length());
		index = temp.find("</");
		if (index == -1){
			break;
		}
		string value = temp.substr(0,index);
		temp = temp.substr(index+1 +  name.length()+2,temp.length());
		if (index == -1){
			break;
		}
		mm.insert(make_pair(name,value));
		index = temp.find(">");
	}
	
	return mm;
}

string XmlConfig::setJsonData(map<string, string> valuemap){
	string signA;
	for (auto itr = valuemap.begin(); itr != valuemap.end(); itr++){
		string name = itr->first;
		string value = itr->second;
		if (itr != valuemap.begin()){
			signA += "&";
		}
		signA += name + "=" + value;
	}
	printf("%s\n", signA.c_str());
	return signA;
}