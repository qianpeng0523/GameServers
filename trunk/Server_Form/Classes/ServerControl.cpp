#include "ServerControl.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"



USING_NS_CC;

ServerControl *ServerControl::m_ins=NULL;

ServerControl::ServerControl(){
	m_pLogoLayer = NULL;
	m_pDatabasesLayer = NULL;
	m_pTablesLayer = NULL;
}

ServerControl::~ServerControl(){
	
}

bool ServerControl::init()
{
	
    return true;
}

ServerControl* ServerControl::getIns(){
	if (!m_ins){
		m_ins = new ServerControl();
		m_ins->init();
	}
	return m_ins;
}
