
#include "ConfigData.h"

ConfigData *ConfigData::m_ins=NULL;
ConfigData::ConfigData(){
	
}

ConfigData::~ConfigData(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

ConfigData *ConfigData::getIns(){
	if (!m_ins){
		m_ins = new ConfigData();
		m_ins->init();
	}
	return m_ins;
}

void ConfigData::init(){
	
}