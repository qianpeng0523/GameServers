#ifndef __TablesLayer_SCENE_H__
#define __TablesLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"
#include "Adaptation.h"
#include "string.h"
using namespace cocos2d;
using namespace ui;
using namespace std;


class TablesLayer : public BaseLayer
{
public:
	TablesLayer();
	~TablesLayer();
	CREATE_FUNC(TablesLayer);
    virtual bool init();
	void TouchEvent(Object *obj, TouchEventType type);
	
	void AddAllTableItem();

	void flush(string tablename);
private:
	ui::ScrollView *m_pScrollView;
	Layout *m_sbg;
};

#endif // __TablesLayer_SCENE_H__
