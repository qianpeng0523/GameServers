#include "Adaptation.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"


Adaptation* Adaptation::m_pShareAdaptation=NULL;

Adaptation::Adaptation()
{
	m_scaleFactor = 1.0;
	m_designResolutionSize = designResolutionSize;
	m_screensize = SCREEN_SIZE;

	m_LeftToplayout=NULL;
	m_LeftBottomlayout = NULL;
	m_RightToplayout = NULL;
	m_RightBottomlayout = NULL;
	m_Middlelayout = NULL;

	m_AddSize = m_designResolutionSize-m_screensize;
}

Adaptation::~Adaptation(){
	
}

Adaptation* Adaptation::shareAdaptation(){
	if (!m_pShareAdaptation){
		m_pShareAdaptation = new Adaptation();
		m_pShareAdaptation->init();
	}
	return m_pShareAdaptation;
}

void Adaptation::removeAdaptation(){
	if (m_pShareAdaptation){
		delete m_pShareAdaptation;
		m_pShareAdaptation = NULL;
	}
}

bool Adaptation::init()
{
	ScreenAdaptationInit();
	float sx = RESOURCE_SCALEX;
	float sy = RESOURCE_SCALEY;
	m_scaleFactor = RESOURCE_SCALEX<RESOURCE_SCALEY?RESOURCE_SCALEX:RESOURCE_SCALEY;
	m_designResolutionSize = designResolutionSize;
	m_screensize = SCREEN_SIZE;

	m_pLeftTopPos = ccp(0, m_screensize.height);
	m_pLeftBottomPos=ccp(0,0);
	m_pRightTopPos = ccp(m_screensize.width, m_screensize.height);
	m_pRightBottomPos=ccp(m_screensize.width,0);
	m_AddSize = m_designResolutionSize - m_screensize;

	return true;
}

void Adaptation::ScreenAdaptationInit(){
	
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("sqlserver", Rect(0, 0, 1136, 640));
		//glview = GLViewImpl::createWithRect("gzmj", Rect(0, 0, 1136, 852));
#else
		glview = GLViewImpl::create("sqlserver");
#endif
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	director->setDisplayStats(false);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
	Size frameSize = glview->getFrameSize();
	// if the frame's height is larger than the height of medium size.
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
// 	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
// #else
// 	//pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
// 	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionFixedHeight);
// #end

}

void Adaptation::CocosStusioVertexAdaption(Layout *layout){
	TraversalChild(layout);
}

void Adaptation::CocosStusioAdaption(Layout *layout, bool ischildad, float sx ){
	Size size = layout->getSize();
	if (size.width == 0 && size.height == 0){
		size = layout->getContentSize();
	}
	Vector<Node *> childs = layout->getChildren();
	Size addsize = (m_designResolutionSize-m_screensize)/2.0;
	for (int i = 0; i < childs.size(); i++){
		Layout *cnode = (Layout *)childs.at(i);
		string name = cnode->getName();
		//CCLOG("name:%s",name.c_str());
		if (name.compare("left_peng_gang") == 0){
			continue;
		}
		Point nodepos = cnode->getPosition();
		Point wnodepos = ((Layout *)cnode->getParent())->convertToWorldSpace(nodepos);
		Size nodesz = cnode->getContentSize()*m_scaleFactor;
		if (nodesz.width == 0 && nodesz.height == 0){
			nodesz = cnode->getSize()*m_scaleFactor;
		}
		if (!ischildad){
			cnode->setScale(sx);
		}
		Point apos = cnode->getAnchorPoint();
		if (nodesz.width <= m_screensize.width&&nodesz.height <= m_screensize.height){
			if (size.width > m_screensize.width){
				if (wnodepos.x - nodesz.width*apos.x<0){
					cnode->setPositionX(nodepos.x + addsize.width);
				}
				else if (wnodepos.x + nodesz.width*(1 - apos.x) >m_screensize.width){
					cnode->setPositionX(nodepos.x - addsize.width);
				}
			}
			if (size.height > m_screensize.height){
				if (wnodepos.y - nodesz.height*apos.y<0){
					cnode->setPositionY(nodepos.y + addsize.height);
				}
				else if (wnodepos.y + nodesz.height*(1 - apos.y) >m_screensize.height){
					cnode->setPositionY(nodepos.y - addsize.height);
				}
			}
		}
		else if (nodesz.width >= m_screensize.width){
			cnode->setPositionX((m_screensize.width/2.0-nodesz.width*(0.5-apos.x)));
		}
		if (ischildad){
			CocosStusioAdaption((Layout *)cnode);
		}
	}
}

void Adaptation::AdaptaSize(Layout *node, float x, float y, bool isImg, bool isScroll){
	Size oldsz = node->getSize();
	Size newsz = Size(oldsz.width/y,oldsz.height/x);
	node->setContentSize(newsz);
	node->setSize(newsz);
	if (isImg){
		((ImageView *)node)->setScale9Enabled(true);
	}
	if (isScroll){
		((ui::ScrollView *)node)->setInnerContainerSize(newsz);
	}
}

void Adaptation::AdaptaSize(Layout *node, Size addsz, bool isImg, bool isScroll){
	Size oldsz = node->getSize();
	Size newsz = Size(oldsz.width + addsz.width, oldsz.height + addsz.height);
	node->setContentSize(newsz);
	node->setSize(newsz);
	if (isImg){
		((ImageView *)node)->setScale9Enabled(true);
	}
	if (isScroll){
		((ui::ScrollView *)node)->setInnerContainerSize(newsz);
	}
}

void Adaptation::AdaptaPos(Layout *node, float addx,float addy){
	Point pos = node->getPosition();
	Point nowpos = pos+ccp(addx,addy);
	node->setPosition(nowpos);
}

void Adaptation::AdaptaPosScale(Layout *node, float sx, float sy){
	Point pos = node->getPosition();
	Point nowpos = ccp(pos.x*sx, pos.y*sy);
	node->setPosition(nowpos);
}

void Adaptation::AdaptaPosScale(Layout *parent, string name, float sx, float sy){
	Layout *layout = (Layout *)Helper::seekWidgetByName(parent, name);
	AdaptaPosScale(layout, sx,sy);
}

void Adaptation::addChildToLeftTop(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis){
	ToLayout(LEFT_TOP, layoutparent, child, isHorizontal, dis);
}

void Adaptation::addChildToLeftBottom(CCNode *layoutparent, CCNode *child, bool isHorizontal,float dis){
	
	ToLayout(LEFT_BOTTOM, layoutparent, child, isHorizontal, dis);
}

void Adaptation::addChildToRightTop(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis){
	ToLayout(RIGHT_TOP, layoutparent, child, isHorizontal, dis);
}

void Adaptation::addChildToRightBottom(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis){
	ToLayout(RIGHT_BOTTOM, layoutparent, child, isHorizontal, dis);
}

void Adaptation::addChildToMiddle(CCNode *layoutparent, CCNode *child, Point pos){
	
}

void Adaptation::ToLayout(POSTYPE type, CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis){
	std::vector<CCNode *>vec;
	std::vector<CCNode *>othervec;
	Layout *bg;
	int index = isHorizontal ? 0 : 1;
	Point dir;
	Size nodesz = child->getContentSize();
	Point nodeapos = child->getAnchorPoint();

	CCNode *lastNode = NULL;
	Point lastpos;
	Size lastnodesz = Size(0, 0);
	Point apos = ccp(0.5, 0.5);

	switch (type)
	{
	case LEFT_TOP:
		vec = m_pLeftTopsNodes[index];
		othervec = m_pLeftTopsNodes[1-index];
		bg = m_LeftToplayout;
		dir = ccp(isHorizontal, -!isHorizontal);
		lastpos = ccp(nodesz.width*nodeapos.x*child->getScale(), m_screensize.height - nodesz.height*(1 - nodeapos.y) *child->getScale());
		break;
	case LEFT_BOTTOM:
		vec = m_pLeftBottomsNodes[index];
		othervec = m_pLeftBottomsNodes[1 - index];
		bg = m_LeftBottomlayout;
		dir = ccp(isHorizontal, !isHorizontal);
		lastpos = ccp(nodesz.width*nodeapos.x*child->getScale(), nodesz.height*nodeapos.y *child->getScale());
		break;
	case RIGHT_TOP:
		vec = m_pRightTopsNodes[index];
		othervec = m_pRightTopsNodes[1 - index];
		bg = m_RightToplayout;
		dir = ccp(-isHorizontal, -!isHorizontal);
		lastpos = ccp(m_screensize.width-nodesz.width*nodeapos.x*child->getScale(), m_screensize.height - nodesz.height*(1 - nodeapos.y) *child->getScale());
		break;
	case RIGHT_BOTTOM:
		vec = m_pRightBottomsNodes[index];
		othervec = m_pRightBottomsNodes[1 - index];
		bg = m_RightBottomlayout;
		dir = ccp(-isHorizontal, !isHorizontal);
		lastpos = ccp(m_screensize.width - nodesz.width*nodeapos.x*child->getScale(), nodesz.height*nodeapos.y *child->getScale());
		break;
	default:
		break;
	}



	if (!bg){
		bg = Layout::create();
		layoutparent->addChild(bg);
	}
	bg->addChild(child);
	
	if (!vec.empty()){
		lastNode = vec.at(vec.size() - 1);
		lastpos = lastNode->getPosition();
		lastnodesz = lastNode->getContentSize()*lastNode->getScale();
		apos = lastNode->getAnchorPoint();
	}

	vec.push_back(child);
	if (othervec.empty()){
		othervec.push_back(child);
	}
	
	child->setPosition(lastpos.x + (lastnodesz.width*apos.x + nodesz.width*nodeapos.x + dis)*dir.x, lastpos.y + (lastnodesz.height*(1 - apos.x) + nodesz.height*(1 - nodeapos.x) + dis)*dir.y);

	saveToVecs(index == 0 ? vec : othervec, index == 1 ? vec : othervec, type);

}

void Adaptation::saveToVecs(std::vector<CCNode *>hvec, std::vector<CCNode *>vvec, POSTYPE type){
	switch (type)
	{
	case LEFT_TOP:
		m_pLeftTopsNodes[0] = hvec;
		m_pLeftTopsNodes[1] = vvec;
		break;
	case LEFT_BOTTOM:
		m_pLeftBottomsNodes[0] = hvec;
		m_pLeftBottomsNodes[1] = vvec;
		break;
	case RIGHT_TOP:
		m_pRightTopsNodes[0] = hvec;
		m_pRightTopsNodes[1] = vvec;
		break;
	case RIGHT_BOTTOM:
		m_pRightBottomsNodes[0] = hvec;
		m_pRightBottomsNodes[1] = vvec;
		break;
	default:
		break;
	}
}

void Adaptation::TraversalChild(CCNode *node){
	Vector<cocos2d::Node *>childs = node->getChildren();
	for (int i = 0; i < childs.size(); i++){
		Layout *cnode = (Layout *)childs.at(i);
		string name = cnode->getName();
		if (name.compare(LT_LAYOUT) == 0){
			LeftTopLayoutResetPos(cnode);
		}
		else if (name.compare(LB_LAYOUT) == 0){
			LeftBottomLayoutResetPos(cnode);
		}
		else if (name.compare(RT_LAYOUT) == 0){
			RightTopLayoutResetPos(cnode);
		}
		else if (name.compare(RB_LAYOUT) == 0){
			RightBottomLayoutResetPos(cnode);
		}
		TraversalChild(cnode);
	}
}

void Adaptation::LeftTopLayoutResetPos(CCNode *node, Point distance){
	Size sz = node->getContentSize()*node->getScale();
	Point apos = node->getAnchorPoint();
	node->setPosition(m_pLeftTopPos+distance+ccp(sz.width*apos.x,sz.height*(apos.y-1)));
}

void Adaptation::LeftBottomLayoutResetPos(CCNode *node, Point distance){
	Size sz = node->getContentSize()*node->getScale();
	Point apos = node->getAnchorPoint();
	node->setPosition(m_pLeftBottomPos + distance + ccp(sz.width*apos.x, sz.height*apos.y));
}

void Adaptation::RightTopLayoutResetPos(CCNode *node, Point distance){
	Size sz = node->getContentSize()*node->getScale();
	Point apos = node->getAnchorPoint();
	node->setPosition(m_pRightTopPos + distance + ccp(sz.width*(apos.x-1), sz.height*(apos.y-1)));
}

void Adaptation::RightBottomLayoutResetPos(CCNode *node, Point distance){
	Size sz = node->getContentSize()*node->getScale();
	Point apos = node->getAnchorPoint();
	node->setPosition(m_pRightBottomPos + distance + ccp(sz.width*(apos.x-1), sz.height*apos.y));
}
