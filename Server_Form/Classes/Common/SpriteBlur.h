#ifndef __BaseLayer_SCENE_H__
#define __BaseLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Adaptation.h"


class SpriteBlur : public cocos2d::Sprite
{
public:
	SpriteBlur();
	~SpriteBlur();
	static SpriteBlur* create(cocos2d::Sprite* pSprite, const float fRadius = 18.0f, const float fSampleNum = 18.0f);
	bool initWithTexture(cocos2d::Texture2D* pTexture, const cocos2d::Rect&  rRect);
	void initGLProgram();

	void setBlurRadius(float fRadius);
	void setBlurSampleNum(float fSampleNum);
	static cocos2d::RenderTexture* SpriteBlurer(cocos2d::Sprite* pSprite, const float fRadius = 18.0f, const float fSampleNum =18.0f);
	static cocos2d::RenderTexture* ScreenShot(const bool bIsSave, std::function<void(cocos2d::RenderTexture*, const std::string&)> pFuncCallback);
protected:
	float fBlurRadius_;
	float fBlurSampleNum_;
};

#endif // __BaseLayer_SCENE_H__
