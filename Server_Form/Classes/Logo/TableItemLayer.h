#ifndef __TableItemLayer_SCENE_H__
#define __TableItemLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"
#include "Adaptation.h"
#include "string.h"
using namespace cocos2d;
using namespace ui;
using namespace std;


class TableItemLayer : public BaseLayer
{
public:
	TableItemLayer();
	~TableItemLayer();
	static TableItemLayer *create(string tname);
	virtual bool init(string tname);
	void TouchEvent();
	string getTableName(){
		return m_tname;
	}

	void select(bool iss);
private:
	string m_tname;
};

#endif // __TableItemLayer_SCENE_H__
