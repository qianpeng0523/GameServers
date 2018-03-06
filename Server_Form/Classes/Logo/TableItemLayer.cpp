#include "TableItemLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GameDataSet.h"
#include "HttpInfo.h"
#include "ServerControl.h"

USING_NS_CC;

TableItemLayer::TableItemLayer(){
	
}

TableItemLayer::~TableItemLayer(){
	
}

bool TableItemLayer::init(string tname)
{
	if (!Layer::init())
    {
        return false;
    }
	m_tname = tname;
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("dbitem.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());

	GameDataSet::setText(m_RootLayer, "label", tname);
    return true;
}

TableItemLayer *TableItemLayer::create(string tname){
	TableItemLayer *p = new TableItemLayer();
	if (p&&p->init(tname)){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

void TableItemLayer::TouchEvent(){
	HttpInfo::getIns()->setTableName(m_tname);
	
}

void TableItemLayer::select(bool iss){
	GameDataSet::setImageView(m_RootLayer, "bg", iss?"bg3_list2.png":"bg3_list.png");
}