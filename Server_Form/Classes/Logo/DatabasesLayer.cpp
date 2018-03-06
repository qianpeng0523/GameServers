#include "DatabasesLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GameDataSet.h"
#include "HttpInfo.h"
#include "ServerControl.h"
#include "DBItemLayer.h"
USING_NS_CC;

DatabasesLayer::DatabasesLayer(){
	ServerControl::getIns()->setDatabasesLayer(this);
}

DatabasesLayer::~DatabasesLayer(){
	if (this==ServerControl::getIns()->getDatabasesLayer()){
		ServerControl::getIns()->setDatabasesLayer(NULL);
	}
}

bool DatabasesLayer::init()
{
	if (!Layer::init())
    {
        return false;
    }

	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("databases.json");
	this->addChild(m_RootLayer);
	
	SEL_TouchEvent selector = toucheventselector(DatabasesLayer::TouchEvent);
	GameDataSet::getButton(m_RootLayer, "close", selector, this);
	GameDataSet::getButton(m_RootLayer, "ok_btn", selector, this);
	
	m_pScrollView = (ui::ScrollView *)GameDataSet::getButton(m_RootLayer, "ScrollView", selector, this);
	m_sbg = GameDataSet::getLayout(m_RootLayer,"sbg");

	AddAllDBItem();
    return true;
}

void DatabasesLayer::TouchEvent(Object *obj, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED){
		Button *btn = (Button *)obj;
		string name = btn->getName();
		if (name.compare("close") == 0){
			this->removeFromParentAndCleanup(true);
		}
		else if (name.compare("ScrollView") == 0){
			DBItemLayer *p = (DBItemLayer *)GameDataSet::isTouchInChild(m_pScrollView, 10, NULL);
			if (p){
				p->TouchEvent();
				flush(p->getDBName());
			}
		}
		else if (name.compare("ok_btn") == 0){
			HttpInfo::getIns()->requestSQLDB(HttpInfo::getIns()->getDBName());
			this->removeFromParentAndCleanup(true);
		}
	}
}

void DatabasesLayer::AddAllDBItem(){
	string curdbname = HttpInfo::getIns()->getDBName();
	vector<string>dbs = HttpInfo::getIns()->getDatabases();
	for (int i = 0; i < dbs.size();i++){
		DBItemLayer *p = DBItemLayer::create(dbs.at(i));
		if (curdbname.compare(p->getDBName()) == 0){
			p->select(true);
		}
		GameDataSet::PushScrollItem(m_sbg, 0,0, p, i, m_pScrollView);
	}
}

void DatabasesLayer::flush(string dbname){
	Vector<Node *>nodes = m_sbg->getChildren();
	for (int i = 0; i < nodes.size();i++){
		DBItemLayer *p = (DBItemLayer *)nodes.at(i);
		if (p->getDBName().compare(dbname) == 0){
			p->select(true);
		}
		else{
			p->select(false);
		}
	}
}