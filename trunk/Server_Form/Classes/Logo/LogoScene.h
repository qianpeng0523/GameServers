#ifndef __LogoScene_SCENE_H__
#define __LogoScene_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LogoLayer.h"

using namespace cocos2d;
using namespace ui;
using namespace std;


class LogoScene : public CCScene
{
public:
	LogoScene();
	~LogoScene();
    virtual bool init();
	CREATE_FUNC(LogoScene);
	
private:
	LogoLayer *m_pLogoLayer;
};

#endif // __LogoScene_SCENE_H__
