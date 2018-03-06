#ifndef __DatabasesLayer_SCENE_H__
#define __DatabasesLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"
#include "Adaptation.h"
#include "string.h"
using namespace cocos2d;
using namespace ui;
using namespace std;


class DatabasesLayer : public BaseLayer
{
public:
	DatabasesLayer();
	~DatabasesLayer();
	CREATE_FUNC(DatabasesLayer);
    virtual bool init();
	void TouchEvent(Object *obj, TouchEventType type);
	
	void AddAllDBItem();

	void flush(string dbname);
private:
	ui::ScrollView *m_pScrollView;
	Layout *m_sbg;
};

#endif // __DatabasesLayer_SCENE_H__
