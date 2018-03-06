#ifndef __ServerControl_SCENE_H__
#define __ServerControl_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LogoLayer.h"
#include "DatabasesLayer.h"
#include "TablesLayer.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class ServerControl : public CCObject
{
public:
	ServerControl();
	~ServerControl();
    virtual bool init();
    CREATE_FUNC(ServerControl);
	static ServerControl* getIns();
	
	void setLogoLayer(LogoLayer *p){
		m_pLogoLayer = p;
	}
	LogoLayer *getLogoLayer(){
		return m_pLogoLayer;
	}

	void setDatabasesLayer(DatabasesLayer *p){
		m_pDatabasesLayer = p;
	}
	DatabasesLayer *getDatabasesLayer(){
		return m_pDatabasesLayer;
	}

	void setTablesLayer(TablesLayer *p){
		m_pTablesLayer = p;
	}
	TablesLayer *getTablesLayer(){
		return m_pTablesLayer;
	}
private:
	static ServerControl *m_ins;
	LogoLayer *m_pLogoLayer;
	DatabasesLayer *m_pDatabasesLayer;
	TablesLayer *m_pTablesLayer;
};

#endif // __ServerControl_SCENE_H__
