#include "TablesLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GameDataSet.h"
#include "HttpInfo.h"
#include "ServerControl.h"
#include "TableItemLayer.h"
USING_NS_CC;

TablesLayer::TablesLayer(){
	ServerControl::getIns()->setTablesLayer(this);
}

TablesLayer::~TablesLayer(){
	if (this==ServerControl::getIns()->getTablesLayer()){
		ServerControl::getIns()->setTablesLayer(NULL);
	}
}

bool TablesLayer::init()
{
	if (!Layer::init())
    {
        return false;
    }

	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("databases.json");
	this->addChild(m_RootLayer);
	
	SEL_TouchEvent selector = toucheventselector(TablesLayer::TouchEvent);
	GameDataSet::getButton(m_RootLayer, "close", selector, this);
	GameDataSet::getButton(m_RootLayer, "ok_btn", selector, this);
	
	m_pScrollView = (ui::ScrollView *)GameDataSet::getButton(m_RootLayer, "ScrollView", selector, this);
	m_sbg = GameDataSet::getLayout(m_RootLayer,"sbg");

	AddAllTableItem();
    return true;
}

void TablesLayer::TouchEvent(Object *obj, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED){
		Button *btn = (Button *)obj;
		string name = btn->getName();
		if (name.compare("close") == 0){
			this->removeFromParentAndCleanup(true);
		}
		else if (name.compare("ScrollView") == 0){
			TableItemLayer *p = (TableItemLayer *)GameDataSet::isTouchInChild(m_pScrollView, 10, NULL);
			if (p){
				p->TouchEvent();
				string tablename = p->getTableName();
				flush(tablename);
				HttpInfo::getIns()->m_curtablename = tablename;
			}
		}
		else if (name.compare("ok_btn") == 0){
			LogoLayer *p = ServerControl::getIns()->getLogoLayer();
			string tablename = HttpInfo::getIns()->m_curtablename;
			if (tablename.empty()){
				p->ShowTable(XXIconv::GBK2UTF("ÔÝÎÞ±í"));
			}
			else{
				p->ShowTable(tablename);
				HttpInfo::getIns()->requestSQLColumns(tablename);
			}
			this->removeFromParentAndCleanup(true);
		}
	}
}

void TablesLayer::AddAllTableItem(){
	string curdtname = HttpInfo::getIns()->getTableName();
	vector<string>tbs = HttpInfo::getIns()->getTables();
	if (tbs.empty()){
		HttpInfo::getIns()->setTableName("");
	}
	for (int i = 0; i < tbs.size(); i++){
		TableItemLayer *p = TableItemLayer::create(tbs.at(i));
		if (curdtname.compare(p->getTableName()) == 0){
			p->select(true);
		}
		GameDataSet::PushScrollItem(m_sbg, 0,0, p, i, m_pScrollView);
	}
}

void TablesLayer::flush(string tname){
	Vector<Node *>nodes = m_sbg->getChildren();
	for (int i = 0; i < nodes.size();i++){
		TableItemLayer *p = (TableItemLayer *)nodes.at(i);
		if (p->getTableName().compare(tname) == 0){
			p->select(true);
		}
		else{
			p->select(false);
		}
	}
}