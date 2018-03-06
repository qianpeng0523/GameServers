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
	// Notice:���childʱҪע�����²�
	// ���·���CCMenuItemImage �����ģ��ͼƬ ���Ϸ���CCProgressTimer

	// �����Ӱģ��
	mStencil = Sprite::create(stencil_file_name);
	mStencil->setPosition(ccp(0,0));
	mStencil->setVisible(false);
	addChild(mStencil);

	// �����ת����������
	
	Sprite* progressSprite = Sprite::create(btnfile.c_str());
	mProgressTimer = ProgressTimer::create(progressSprite);
	mProgressTimer->setPosition(ccp(0, 0));
	mProgressTimer->setVisible(false);
	addChild(mProgressTimer, 100);

	mCDTime = cdTime;
	return true;
}

/** ���ܰ�ť����ص� */
void CDProgressTimer::skillClickCallBack()
{
	// ��ȴ��ʱ����ʱ״̬���ܰ�ť���ɵ��
	//log("skillClickCallBack 1:");
	m_curbtn->setEnabled(false);
	//log("skillClickCallBack 2:");
	// ģ��ɼ�
	mStencil->setVisible(true);
	//log("skillClickCallBack 3:");
	mProgressTimer->setPercentage(0);
	//log("skillClickCallBack 4:");
	// ���þ��������Ϊ˳ʱ��
	mProgressTimer->setVisible(true);
	//log("skillClickCallBack 5:");
	mProgressTimer->setType(kCCProgressTimerTypeRadial);
	//log("skillClickCallBack 6:");
	//׼��һ��5����ת360�ȵĶ���(�𽥸��ǰ�͸ģ���γ���ȴЧ��;������м�ʱ��ȴ������ʵ�ֺ�ʱ�����)
	CCActionInterval* action_progress_to = CCProgressTo::create(mCDTime, 100);
	CCCallFunc* action_callback = CCCallFuncN::create(this, callfuncN_selector(CDProgressTimer::skillCoolDownCallBack));
	mProgressTimer->runAction(CCSequence::create(action_progress_to, action_callback, NULL));
}

/** ������ȴ��ɻص� */
void CDProgressTimer::skillCoolDownCallBack(CCNode* node)
{
	// �����ɰ岻�ɼ�
	//log("skillClickCallBack 7:");
	mStencil->setVisible(false);
	//log("skillClickCallBack 8:");
	// ���������ܲ��ɼ�
	mProgressTimer->setVisible(false);
	//log("skillClickCallBack 9:");
	// ��ť��Ϊ����
	m_curbtn->setEnabled(true);
	//log("skillClickCallBack 10:");
}