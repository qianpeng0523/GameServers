#include "EffectAnimation.h"
#include "AppMacros.h"
#include "XXIconv.h"

EffectAnimation *EffectAnimation::m_ins=NULL;

EffectAnimation::EffectAnimation(){
	m_dict=new Dictionary();
	m_pTexture2DVecs = new Dictionary();
}

EffectAnimation::~EffectAnimation(){
	delete m_dict;
	m_dict=NULL;
	delete m_pTexture2DVecs;
	m_pTexture2DVecs = NULL;
}

EffectAnimation *EffectAnimation::getIns() {
	if(!m_ins){
		m_ins=new EffectAnimation();
		m_ins->init();
	}
	return m_ins;
}

bool EffectAnimation::init() {
	return true;
}

void EffectAnimation::Play(CCNode*node, string key,string name, bool isForever, int count, CCCallFuncND *selector, int id, int delaytime, bool isRemove){
	log("key:%s,name:%s,id:%d,count:%d", key.c_str(), name.c_str(), id, count);
	PlayAnimation(node, key,name, isForever, count, selector, id, delaytime,isRemove);
}

void EffectAnimation::AddAnimation(string key, string name, int id, string filepng, int count, float duration, CCCallFuncND *selector){
	log("key:%s,name:%s,id:%d,filepng:%s,count:%d,dur:%f",key.c_str(),name.c_str(),id,filepng.c_str(),count,duration);
	char str[100] = { 0 };
	Texture2DVecs *pTexture2DVecs = (Texture2DVecs *)m_pTexture2DVecs->objectForKey(key+name);
	if (selector){
		selector->retain();
	}
	if (!pTexture2DVecs){
		pTexture2DVecs = new Texture2DVecs();
		pTexture2DVecs->_key = key;
		pTexture2DVecs->_id = id;
		pTexture2DVecs->_duration = duration;
		pTexture2DVecs->_count = count;
		pTexture2DVecs->_file = filepng;
		pTexture2DVecs->_name = name;
	}
	
	vector< CCCallFuncND *>::iterator itr = pTexture2DVecs->_selector.begin();
	bool ist = false;
	for (itr; itr != pTexture2DVecs->_selector.end(); itr++){
		if (*itr == selector){
			ist = true;
		}
	}
	if (!ist){
		pTexture2DVecs->_selector.push_back(selector);
	}
	m_pTexture2DVecs->setObject(pTexture2DVecs, key+name);
	preloadImgAsync(pTexture2DVecs);
}

void EffectAnimation::preloadImgAsync(Texture2DVecs *p){
	char buff[200];
	std::function<void(Texture2D*)> callback = std::bind(&EffectAnimation::AddImgCallBack, this, std::placeholders::_1);

	for (int i = 0; i < p->_count; i++)
	{
		if (p->_id>-1){
			sprintf(buff, p->_file.c_str(), p->_id, p->_id, i + 1);
		}
		else if (p->_id == -1){
			sprintf(buff, p->_file.c_str(), i + 1);
		}
		CCTextureCache::sharedTextureCache()->addImageAsync(buff, callback);
	}
}

void EffectAnimation::AddImgCallBack(CCTexture2D *p2d){
	char buff[300];
	
	DictElement *elt;
	CCDICT_FOREACH(m_pTexture2DVecs, elt){
		Texture2DVecs *p = (Texture2DVecs *)elt->getObject();
		bool isLoadFinished = true;

		for (int i = 0; i < p->_count; i++){
			if (p->_id>-1){
				sprintf(buff, p->_file.c_str(), p->_id, p->_id, i + 1);
			}
			else if (p->_id == -1){
				sprintf(buff, p->_file.c_str(), i + 1);
			}
			CCTexture2D *p2 = CCTextureCache::sharedTextureCache()->getTextureForKey(buff);
			if (!p2){
				isLoadFinished = false;
				break;
			}
		}
		if (isLoadFinished){

			for (int i = 0; i < p->_selector.size(); i++){
				if (p->_selector.at(i)&&i==0){
					CCNode *obj = (CCNode *)p->_selector.at(i)->getTargetCallback();
					p->_selector.at(i)->setTarget(obj);
					p->_selector.at(i)->execute();
					break;
					//delete p;
				}
			}
			p->_selector.clear();
			//m_pTexture2DVecs->removeObjectForKey(p->_key+p->_name);
		}
	}
}

void EffectAnimation::removeActFromDict(string key){
	m_dict->removeObjectForKey(key.c_str());
}

CCAnimation *EffectAnimation::GetAnimation(string key, int id){
	CCAnimation* animation = (CCAnimation *)m_dict->objectForKey(key.c_str());
	if (animation){
		return animation;
	}
	else{
		if (m_pTexture2DVecs->objectForKey(key)){
			Texture2DVecs *pTexture2DVecs = (Texture2DVecs *)m_pTexture2DVecs->objectForKey(key);
			if (pTexture2DVecs){
				animation = CCAnimation::create();
				animation->setRestoreOriginalFrame(false);
				animation->setDelayPerUnit(pTexture2DVecs->_duration);
				m_dict->setObject(animation, key.c_str());
			}
			int count = pTexture2DVecs->_count;
			string file = pTexture2DVecs->_file;
			int id = pTexture2DVecs->_id;
			char buff[200];
			for (int i = 0; i < count; i++){
				if (id > 0){
					sprintf(buff, file.c_str(), id, id, i + 1);
				}
				else if (id == -1){
					sprintf(buff, file.c_str(), i + 1);
				}
				Texture2D *p2d = CCTextureCache::sharedTextureCache()->textureForKey(buff);
				if (!p2d){
					return NULL;
				}
				CCRect rect = CCRect(ccp(0, 0), p2d->getContentSize());
				animation->addSpriteFrameWithTexture(p2d, rect);
			}
			//m_pTexture2DVecs->removeObjectForKey(pTexture2DVecs->_key);
		}
	}
	return animation;
}

CCAnimation *EffectAnimation::PlayAnimation(CCNode*node, string key,string name, bool isForever, int count, CCCallFuncND *selector, int id, int delaytime, bool isRemove ){
	log("EffectAnimation::PlayAnimation:key:%s,name:%s",key.c_str(),name.c_str());
	CCAnimation *ani = GetAnimation(key + name, id);
	if(!ani){
		return NULL;
	}
	log("EffectAnimation::PlayAnimation11111");
	CCAnimate* animate = CCAnimate::create(ani);
	
	CCFiniteTimeAction *act = NULL;
	if(isForever){
		if (delaytime == 0){
			act = CCRepeatForever::create(animate);
		}
		else{
			act = CCRepeatForever::create(CCSequence::create(animate,CCDelayTime::create(delaytime),NULL));
		}
	}
	else{
		if (count <= 1){
			if (selector){
				act = CCSequence::create(animate, CCDelayTime::create(delaytime), selector, NULL);
			}
			else{
				if (isRemove){
					act = CCSequence::create(animate, CCDelayTime::create(delaytime), CCRemoveSelf::create(true), selector);
				}
				else{
					act = CCSequence::create(animate, CCDelayTime::create(delaytime), selector);
				}
			}
		}
		else{
			if (selector){
				act = CCSequence::create(CCRepeat::create(animate, count), CCDelayTime::create(delaytime), selector, NULL);
			}
			else{
				if (isRemove){
					act = CCSequence::create(CCRepeat::create(animate, count), CCDelayTime::create(delaytime), CCRemoveSelf::create(true), selector);
				}
				else{
					act = CCSequence::create(CCRepeat::create(animate, count), CCDelayTime::create(delaytime), selector);
				}
			}
		}
	}
	node->runAction(act);
	
	return ani;
}