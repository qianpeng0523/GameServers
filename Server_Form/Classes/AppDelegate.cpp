#include "AppDelegate.h"
#include "Adaptation.h"
#include "LogoScene.h"
USING_NS_CC;



AppDelegate::AppDelegate() {
	
}

AppDelegate::~AppDelegate() 
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    
	Adaptation::shareAdaptation();
	register_all_packages();
	
	auto director = Director::getInstance();
	Scene *scene = LogoScene::create();
	//Scene *scene = PageTurn::createScene();
	director->runWithScene(scene);
	//scene->Test();
	//RuleInfo::getIns()->ZiGameRecordTest(scene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    

    // if you use SimpleAudioEngine, it must be pause
	//CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    log("dlegate :applicationDidEnterBackground");

	//WebSocketControl::getIns()->onClose(false);

	//Director::getInstance()->pause();
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&&!TARGET_IPHONE_SIMULATOR)
	VoiceInfo::getIns()->SystemPause(true);
#endif
	Director::getInstance()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	//SimpleAudioEngine::sharedEngine()->stopAllEffects();
#else
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(vo);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(effectvo);
#endif
#if(CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&&!TARGET_IPHONE_SIMULATOR)
	VoiceInfo::getIns()->SystemResume(true);
#endif
}
