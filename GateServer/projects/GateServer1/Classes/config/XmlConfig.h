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
	
	string setXmlData(map<string,string> valuemap);
	map<string, string> parseXmlData(string content);
public:
	
private:
	static XmlConfig *m_Ins;
	
public:
	
};

#endif 