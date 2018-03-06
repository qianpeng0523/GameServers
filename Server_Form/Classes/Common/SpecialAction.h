#ifndef __SpecialAction_SCENE_H__
#define __SpecialAction_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppMacros.h"
using namespace cocos2d;
using namespace std;

class SpecialAction : public Object
{
public:
	SpecialAction();
	~SpecialAction();
	static SpecialAction* shareSpecialAction();
    virtual bool init();

	static void RunFadeout(Node *node, float dt, bool istouch = false, FiniteTimeAction *selector = NULL, bool isdelay = true, bool isparent=true);
	static void RunFadein(Node *node, float dt, bool istouch = false, FiniteTimeAction *selector = NULL, bool isdelay = true, bool isparent=true);
	void TouchenableCall(Node *node);
	void TouchdisableCall(Node *node);

	static void RunMoveAndScale(Node *nodetarget, Node *zeronode, float dt, Point targetpos);

	static void RunHit(Node *target, float dt, float jumph=1.08,float jumpsx=2.5);

	static void RunMove(Node *target, Point targetpos, Point zeropos, float dt);

	static void BlurSprite(Node *parent);

	static void QuickFade(Node *node, float dt, unsigned int times, FiniteTimeAction *selector = NULL);
private:
	static SpecialAction* m_pShareSpecialAction;
	
};

#endif // __SpecialAction_SCENE_H__
