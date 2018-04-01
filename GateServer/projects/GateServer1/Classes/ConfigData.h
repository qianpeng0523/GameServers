#ifndef __LYNX_GAME_SERVER_ConfigData_H__
#define __LYNX_GAME_SERVER_ConfigData_H__
#include "stdafx.h"

using namespace std;


class ConfigData 
{
public:
	ConfigData();
	virtual ~ConfigData();
	
	static ConfigData *getIns();
	void init();
private:
	static ConfigData *m_ins;
};


#endif // __LYNX_GAME_SERVER_ConfigData_H__