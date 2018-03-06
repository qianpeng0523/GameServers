#include "CDProgressTimer.h"


USING_NS_CC;

CDProgressTimer::CDProgressTimer() :
mStencil(NULL),
mProgressTimer(NULL),
mCDTime(1.f),
m_curbtn(NULL)
{

}

CDProgressTimer::~CDProgressTimer()
{

}

CDProgressTimer* CDProgressTimer::createCDProgressTimer(float cdTime, const char* stencil_file_name, string btnfile, Button *btn)
{
	CDProgressTimer* pCDProgressTimer = new CDProgressTimer();
	if (pCDProgressTimer && pCDProgressTimer->init(cdTime, stencil_file_name,btnfile, btn))
	{
		pCDProgressTimer->autorelease();
		return pCDProgressTimer;
	}
	else
	{
		delete pCDProgressTimer;
		pCDProgressTimer = NULL;
	}

	return NULL;
}

bool CDProgressTimer::init(float cdTime, const char* stencil_file_name,string btnfile,Button *btn)
{
	CCAssert(stencil_file_name, "CDProgressTimer::init stencil_file_name != NULL");
	m_curbtn = btn;
	// Notice:添加child时要注意上下层
	// 最下方是CCMenuItemImage 其次是模版图片 最上方是CCProgressTimer

	// 添加阴影模版
	mStencil = Sprite::create(stencil_file_name);
	mStencil->setPosition(ccp(0,0));
	mStencil->setVisible(false);
	addChild(mStencil);

	// 添加旋转进度条精灵
	
	Sprite* progressSprite = Sprite::create(btnfile.c_str());
	mProgressTimer = ProgressTimer::create(progressSprite);
	mProgressTimer->setPosition(ccp(0, 0));
	mProgressTimer->setVisible(false);
	addChild(mProgressTimer, 100);

	mCDTime = cdTime;
	return true;
}

/** 技能按钮点击回调 */
void CDProgressTimer::skillClickCallBack()
{
	// 冷却计时，即时状态技能按钮不可点击
	//log("skillClickCallBack 1:");
	m_curbtn->setEnabled(false);
	//log("skillClickCallBack 2:");
	// 模版可见
	mStencil->setVisible(true);
	//log("skillClickCallBack 3:");
	mProgressTimer->setPercentage(0);
	//log("skillClickCallBack 4:");
	// 设置精灵进度条为顺时针
	mProgressTimer->setVisible(true);
	//log("skillClickCallBack 5:");
	mProgressTimer->setType(kCCProgressTimerTypeRadial);
	//log("skillClickCallBack 6:");
	//准备一个5秒旋转360度的动画(逐渐覆盖半透模板形成冷却效果;这里进行计时冷却动画的实现和时间控制)
	CCActionInterval* action_progress_to = CCProgressTo::create(mCDTime, 100);
	CCCallFunc* action_callback = CCCallFuncN::create(this, callfuncN_selector(CDProgressTimer::skillCoolDownCallBack));
	mProgressTimer->runAction(CCSequence::create(action_progress_to, action_callback, NULL));
}

/** 技能冷却完成回调 */
void CDProgressTimer::skillCoolDownCallBack(CCNode* node)
{
	// 设置蒙板不可见
	//log("skillClickCallBack 7:");
	mStencil->setVisible(false);
	//log("skillClickCallBack 8:");
	// 进度条技能不可见
	mProgressTimer->setVisible(false);
	//log("skillClickCallBack 9:");
	// 按钮置为可用
	m_curbtn->setEnabled(true);
	//log("skillClickCallBack 10:");
}