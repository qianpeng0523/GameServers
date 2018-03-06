#ifndef __Adaptation_SCENE_H__
#define __Adaptation_SCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppMacros.h"
#include "cocos/ui/UILayout.h"
#include "cocos/ui/UIImageView.h"
#include "cocos/ui/UIButton.h"
#include "cocos/ui/UIScrollView.h"
#include "cocos/ui/UIPageView.h"
#include "cocos/ui/UITextAtlas.h"
#include "cocos/ui/UICheckBox.h"
#include "cocos/ui/UISlider.h"
#include "cocos/ui/UILoadingBar.h"
#include "cocos/ui/UITextBMFont.h"
#include "cocos/editor-support/cocostudio/CCSGUIReader.h"
#include "cocos/ui/UIHelper.h"
#include "cocos/ui/UIText.h"
#include "SimpleAudioEngine.h"


#define LT_LAYOUT "left_top_layout"
#define RT_LAYOUT "right_top_layout"
#define LB_LAYOUT "left_bottom_layout"
#define RB_LAYOUT "right_bottom_layout"




using namespace cocos2d;
using namespace ui;
using namespace extension;
using namespace std;
using namespace cocostudio;

enum POSTYPE
{
	LEFT_TOP=0,
	LEFT_BOTTOM,
	RIGHT_TOP,
	RIGHT_BOTTOM
};

class Adaptation : public Object
{
public:
	Adaptation();
	~Adaptation();
	static Adaptation* shareAdaptation();
	void removeAdaptation();
    virtual bool init();
public:
	float getScaleFactor()//获取缩放因子
	{
		return m_scaleFactor;
	}
	Size getDesignSize(){//设计大小
		return m_designResolutionSize;
	}
	Size getScreenSize(){//屏幕大小
		return m_screensize;
	}
	Size getAddSize(){
		return m_AddSize;
	}

	void CocosStusioVertexAdaption(Layout *layout);//cocosstudio 根据四个顶点适配(四个顶点的层名分别为lt_layout,lb_layout,rt_layout,rb_layout)

	void CocosStusioAdaption(Layout *layout, bool ischildad=true,float sx=1.0);//cocosstudio 跑到屏幕外的移到屏幕内部

	void addChildToLeftTop(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis = 20);//手写放在左上角适配,layoutparent:m_LeftToplayout的父节点,child所需要放的孩子
	void addChildToLeftBottom(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis = 20);//手写放在左下角适配，layoutparent:m_LeftBottomlayout的父节点,child所需要放的孩子
	void addChildToRightTop(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis = 20);//手写放在右上角适配，layoutparent:m_RightToplayout的父节点,child所需要放的孩子
	void addChildToRightBottom(CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis = 20);//手写放在右下角适配，layoutparent:m_RightBottomlayout的父节点,child所需要放的孩子
	void addChildToMiddle(CCNode *layoutparent, CCNode *child, Point pos);//手写放在中心点适配，layoutparent:m_Middlelayout的父节点,child所需要放的孩子


	/////特殊适配，需要根据分辨率大小改变尺寸
	void AdaptaSize(Layout *node, float x, float y, bool isImg,bool isScroll=false);
	void AdaptaSize(Layout *node, Size addsz, bool isImg, bool isScroll = false);
	void AdaptaPos(Layout *node, float addx, float addy);
	void AdaptaPosScale(Layout *node, float sx, float sy);
	void AdaptaPosScale(Layout *parent,string name, float sx, float sy);
protected:
	void ToLayout(POSTYPE type, CCNode *layoutparent, CCNode *child, bool isHorizontal, float dis);
private:
	void saveToVecs(std::vector<CCNode *>hvec, std::vector<CCNode *>vvec, POSTYPE type);
	void ScreenAdaptationInit();//屏幕适配初始化
	void TraversalChild(CCNode *node);//遍历所有节点
	void LeftTopLayoutResetPos(CCNode *node,Point distance=ccp(10,-10));//左上
	void LeftBottomLayoutResetPos(CCNode *node, Point distance = ccp(10, 10));//左下
	void RightTopLayoutResetPos(CCNode *node, Point distance = ccp(-10, -10));//右上
	void RightBottomLayoutResetPos(CCNode *node, Point distance = ccp(-10, 10));//右下
private:
	static Adaptation* m_pShareAdaptation;
	float m_scaleFactor;
	Size m_designResolutionSize;
	Size m_screensize;
	Size m_AddSize;

	Point m_pLeftTopPos;
	Point m_pLeftBottomPos;
	Point m_pRightTopPos;
	Point m_pRightBottomPos;

	std::vector<CCNode *>m_pLeftTopsNodes[2];
	std::vector<CCNode *>m_pLeftBottomsNodes[2];
	std::vector<CCNode *>m_pRightTopsNodes[2];
	std::vector<CCNode *>m_pRightBottomsNodes[2];

	Layout *m_LeftToplayout;
	Layout *m_LeftBottomlayout;
	Layout *m_RightToplayout;
	Layout *m_RightBottomlayout;
	Layout *m_Middlelayout;
};

#endif // __Adaptation_SCENE_H__
