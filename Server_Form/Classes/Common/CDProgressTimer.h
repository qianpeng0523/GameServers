/**
实现技能冷却效果

Notice:添加child时要注意上下层关系
最下方是按钮 其次是半透明的模版图片 最上方是CCProgressTimer进度条精灵
*/
#ifndef _CDProgressTimer_H_
#define _CDProgressTimer_H_
#include <cocos2d.h>
#include "Adaptation.h"

class CDProgressTimer : public cocos2d::CCNode
{
public:
	CDProgressTimer();
	virtual ~CDProgressTimer();

	/** 创建一个CDProgressTimer对象
	*/
	static CDProgressTimer* createCDProgressTimer(float cdTime,
		const char* stencil_file_name, string btnfile,
		Button *btn);

	/** CD时间设置
	*/
	void    setCDTime(float time) { 
		mCDTime = time;
	}
	float   getCDTime() const {
		return mCDTime; 
	}

	/** 技能按钮点击回调 */
	void    skillClickCallBack();

	/** 技能冷却完毕回调*/
	void    skillCoolDownCallBack(cocos2d::CCNode* node);
	void setStencilVisible(bool isv){
		if (mStencil){
			mStencil->setVisible(isv);
		}
	}
private:

	/** 初始化 */
	bool    init(float cdTime, const char* stencil_file_name, string btnfile, Button *btn);

private:
	cocos2d::Sprite*          mStencil;       // 蒙板精灵,黑色半透明(这个是为了显示一个冷却计时效果)
	cocos2d::CCProgressTimer*   mProgressTimer; // 时间进度条精灵(360度旋转)
	float                       mCDTime;          // CD时间
	Button *m_curbtn;
};


#endif