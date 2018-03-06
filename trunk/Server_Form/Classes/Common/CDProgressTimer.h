/**
ʵ�ּ�����ȴЧ��

Notice:���childʱҪע�����²��ϵ
���·��ǰ�ť ����ǰ�͸����ģ��ͼƬ ���Ϸ���CCProgressTimer����������
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

	/** ����һ��CDProgressTimer����
	*/
	static CDProgressTimer* createCDProgressTimer(float cdTime,
		const char* stencil_file_name, string btnfile,
		Button *btn);

	/** CDʱ������
	*/
	void    setCDTime(float time) { 
		mCDTime = time;
	}
	float   getCDTime() const {
		return mCDTime; 
	}

	/** ���ܰ�ť����ص� */
	void    skillClickCallBack();

	/** ������ȴ��ϻص�*/
	void    skillCoolDownCallBack(cocos2d::CCNode* node);
	void setStencilVisible(bool isv){
		if (mStencil){
			mStencil->setVisible(isv);
		}
	}
private:

	/** ��ʼ�� */
	bool    init(float cdTime, const char* stencil_file_name, string btnfile, Button *btn);

private:
	cocos2d::Sprite*          mStencil;       // �ɰ徫��,��ɫ��͸��(�����Ϊ����ʾһ����ȴ��ʱЧ��)
	cocos2d::CCProgressTimer*   mProgressTimer; // ʱ�����������(360����ת)
	float                       mCDTime;          // CDʱ��
	Button *m_curbtn;
};


#endif