#ifndef __XmlConfig_SCENE_H__
#define __XmlConfig_SCENE_H__

#include "stdafx.h"

class XmlConfig:public Object
{
public:
	XmlConfig();
	~XmlConfig();
    virtual bool init();
	static XmlConfig *getIns();
	string setXmlData1(map<string, string> valuemap);
	string setXmlData(map<string,string> valuemap);
	map<string, string> parseXmlData(string content);

	string setJsonData(map<string,string> valuemap);
public:
	
private:
	static XmlConfig *m_Ins;
	
public:
	
};

#endif 