#include "LogoScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"

USING_NS_CC;

LogoScene::LogoScene(){
	m_pLogoLayer = NULL;
}

LogoScene::~LogoScene(){
	
}

bool LogoScene::init()
{
	if (!CCScene::init())
	{
		return false;
	}

	if (!m_pLogoLayer){
		m_pLogoLayer = LogoLayer::create();
		this->addChild(m_pLogoLayer);
	}
   
	return true;
}