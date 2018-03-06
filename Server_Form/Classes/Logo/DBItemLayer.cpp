#include "DBItemLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GameDataSet.h"
#include "HttpInfo.h"
#include "ServerControl.h"

USING_NS_CC;

DBItemLayer::DBItemLayer(){
	
}

DBItemLayer::~DBItemLayer(){
	
}

bool DBItemLayer::init(string dbname)
{
	if (!Layer::init())
    {
        return false;
    }
	m_dbname = dbname;
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("dbitem.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());

	GameDataSet::setText(m_RootLayer, "label", dbname);
    return true;
}

DBItemLayer *DBItemLayer::create(string dbname){
	DBItemLayer *p = new DBItemLayer();
	if (p&&p->init(dbname)){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

void DBItemLayer::TouchEvent(){
	HttpInfo::getIns()->setDBName(m_dbname);
	
}

void DBItemLayer::select(bool iss){
	GameDataSet::setImageView(m_RootLayer, "bg", iss?"bg3_list2.png":"bg3_list.png");
}