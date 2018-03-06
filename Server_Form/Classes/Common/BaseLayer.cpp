#include "BaseLayer.h"

USING_NS_CC;

BaseLayer::BaseLayer(){
	m_RootLayer = NULL;
}

BaseLayer::~BaseLayer(){
	
}

bool BaseLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	return true;
}

void BaseLayer::setRootLayerOpZero(){
	if (m_RootLayer){
		m_RootLayer->setOpacity(0);
	}
}