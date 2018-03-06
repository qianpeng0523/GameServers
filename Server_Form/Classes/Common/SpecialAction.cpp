#include "SpecialAction.h"
#include "Adaptation.h"
#include "SpriteBlur.h"
#include "BaseLayer.h"

class BaseLayer;
SpecialAction* SpecialAction::m_pShareSpecialAction=NULL;

SpecialAction::SpecialAction()
{
	
}

SpecialAction::~SpecialAction(){
	
}

SpecialAction* SpecialAction::shareSpecialAction(){
	if (!m_pShareSpecialAction){
		m_pShareSpecialAction = new SpecialAction();
		m_pShareSpecialAction->init();
	}
	return m_pShareSpecialAction;
}

bool SpecialAction::init()
{
	
	return true;
}

void SpecialAction::RunFadeout(Node *node, float dt, bool istouch, FiniteTimeAction *selector, bool isdelay, bool isparent){
	if (node){
		Vector<Node *>childs = node->getChildren();
		for (int i = 0; i < childs.size(); i++){
			Node *child = childs.at(i);
			RunFadeout(child,dt,false,selector,isdelay,false);
		}
		if (istouch){
			if (isdelay){
				if (!isparent){
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeOut::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchdisableCall)),
						NULL));
				}
				else{
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeOut::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchdisableCall)), Hide::create(), selector,
						NULL));
				}
			}
			else{
				if (!isparent){
					node->runAction(CCSequence::create(FadeOut::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchdisableCall)),
						NULL));
				}
				else{
					node->runAction(CCSequence::create(FadeOut::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchdisableCall)), Hide::create(), selector,
						NULL));
				}
			}
		}
		else{
			if (isdelay){
				if (!isparent){
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeOut::create(dt),
						NULL));
				}
				else{
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeOut::create(dt), Hide::create(), selector,
						NULL));
				}
			}
			else{
				if (!isparent){
					node->runAction(CCSequence::create(FadeOut::create(dt),
						NULL));
				}
				else{
					node->runAction(CCSequence::create(FadeOut::create(dt), Hide::create(), selector,
						NULL));
				}
			}
		}
	}
}

void SpecialAction::RunFadein(Node *node, float dt, bool istouch, FiniteTimeAction *selector, bool isdelay, bool isparent){
	if (node){
		if (isparent){
			node->setVisible(true);
		}
		node->setOpacity(0);
		Vector<Node *>childs = node->getChildren();
		for (int i = 0; i < childs.size(); i++){
			Node *child = childs.at(i);
			RunFadein(child, dt, false, selector, isdelay, false);
		}
		
		if (istouch){
			if (isdelay){
				if (isparent){
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeIn::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchenableCall)), selector,
						NULL));
				}
				else{
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeIn::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchenableCall)),
						NULL));
				}
			}
			else{
				if (isparent){
					node->runAction(CCSequence::create(FadeIn::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchenableCall)), selector,
						NULL));
				}
				else{
					node->runAction(CCSequence::create(FadeIn::create(dt),
						CCCallFuncN::create(SpecialAction::shareSpecialAction(), callfuncN_selector(SpecialAction::TouchenableCall)),
						NULL));
				}
			}
			
		}
		else{
			if (isdelay){
				if (isparent){
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeIn::create(dt), selector,
						NULL));
				}
				else{
					node->runAction(CCSequence::create(CCDelayTime::create(dt), FadeIn::create(dt),
						NULL));
				}
			}
			else{
				if (isparent){
					node->runAction(CCSequence::create(FadeIn::create(dt), selector,
						NULL));
				}
				else{
					node->runAction(CCSequence::create(FadeIn::create(dt), 
						NULL));
				}
			}
		}
	}
}

void SpecialAction::TouchenableCall(Node *node){
	if (node){
		((Layout *)node)->setTouchEnabled(true);
	}
}

void SpecialAction::TouchdisableCall(Node *node){
	if (node){
		((Layout *)node)->setTouchEnabled(false);
	}
}

void SpecialAction::RunMoveAndScale(Node *nodetarget, Node *zeronode, float dt, Point targetpos){
	Point pos = nodetarget->getParent()->convertToNodeSpace(zeronode->getParent()->convertToWorldSpace(zeronode->getPosition()));
	nodetarget->setPosition(pos);
	nodetarget->runAction(CCSequence::create(CCSpawn::create(
		CCMoveTo::create(dt, targetpos), CCScaleTo::create(dt, 1.1)
		, NULL),
		CCScaleTo::create(dt/6.0, 1.0)
		, NULL));
}

void SpecialAction::RunHit(Node *target, float dt, float jumph,float jumpsx){
	if (target){
		Size sz = ((Layout *)target)->getSize();
		float sx = target->getScale();
		target->setScale(sx*jumpsx);
		Point zpos = target->getPosition();
		Point tpos = zpos+ccp(0, sz.height*sx*jumph);
		target->setPosition(tpos);
		target->runAction(Sequence::create(
			Spawn::create(ScaleTo::create(dt,sx),
			MoveTo::create(dt,zpos)
			,NULL)
			,NULL));
	}
}

void SpecialAction::RunMove(Node *target, Point targetpos, Point zeropos,float dt){
	if (target){
		target->setPosition(zeropos);
		target->runAction(Sequence::create(MoveTo::create(dt,targetpos),NULL));
	}
}

void SpecialAction::BlurSprite(Node *parent){
	//截取屏幕（不保存图片，不设置回调函数）
	//((BaseLayer *)parent)->getRootLayer()->setBackGroundImageOpacity(0);
	Sprite *spr = Sprite::create("main/dn_inhall_bg.png");
	auto textureBlur = SpriteBlur::SpriteBlurer(spr);
	//将模糊化后的图片保存成一张图片
	auto spriteBlur = Sprite::createWithSpriteFrame(textureBlur->getSprite()->getSpriteFrame());
	Size vsize = Director::getInstance()->getVisibleSize();
	spriteBlur->setPosition(Vec2(vsize.width, vsize.height));

	Size size = Director::getInstance()->getWinSize();
	spriteBlur->setRotationSkewX(0);
	spriteBlur->setPosition(Vec2(size.width / 2, size.height / 2));

	//将此精灵覆盖在原有层之上，成为一个临时背景
	parent->addChild(spriteBlur, -1);
}

void SpecialAction::QuickFade(Node *node, float dt, unsigned int times, FiniteTimeAction *selector){
	node->runAction(CCSequence::create(CCRepeat::create(CCSequence::create(FadeOut::create(dt), FadeIn::create(dt), NULL), times)
		,Hide::create(), selector
		, NULL));
}