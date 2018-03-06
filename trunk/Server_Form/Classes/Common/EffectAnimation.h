#ifndef __EffectAnimation_H__
#define __EffectAnimation_H__

#include "cocos2d.h"
#include "cocos-ext.h"
//#include "kof/KOFHero.h"

using namespace std;
using namespace cocos2d;
using namespace ui;
using namespace extension;

USING_NS_CC;

struct Texture2DVecs:public Object{
	string _key;
	int _id;
	float _duration;
	int _count;
	string  _file;
	vector< CCCallFuncND *>_selector;
	string _name;
};

class EffectAnimation:public Object{
public:
	EffectAnimation();
	~EffectAnimation();
	static EffectAnimation *getIns();
	virtual bool init();

	void AddAnimation(string key,string name, int id, string filepng, int count, float duration, CCCallFuncND *selector = NULL);
	void Play(CCNode*node, string key,string name, bool isForever = true, int count = 0, CCCallFuncND *selector = NULL, int id = 0, int delaytime = 0, bool isRemove = false);
	void removeActFromDict(string key);
	CCAnimation *GetAnimation(string key, int id);

protected:
	void ActFinishCallBack(CCNode *node,void *data);
	
	CCAnimation *PlayAnimation(CCNode*node, string key, string name, bool isForever = true, int count = 0, CCCallFuncND *selector = NULL, int id = 0, int delaytime = 0, bool isRemove = false);
	void AddImgCallBack(CCTexture2D *p2d);
	void preloadImgAsync(Texture2DVecs *p);

	
private:
	static EffectAnimation *m_ins;
	Dictionary *m_dict;
	Dictionary *m_pTexture2DVecs;
	
};

#endif