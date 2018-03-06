#ifndef __DBItemLayer_SCENE_H__
#define __DBItemLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"
#include "Adaptation.h"
#include "string.h"
using namespace cocos2d;
using namespace ui;
using namespace std;


class DBItemLayer : public BaseLayer
{
public:
	DBItemLayer();
	~DBItemLayer();
	static DBItemLayer *create(string dbname);
	virtual bool init(string dbname);
	void TouchEvent();
	string getDBName(){
		return m_dbname;
	}

	void select(bool iss);
private:
	string m_dbname;
};

#endif // __DBItemLayer_SCENE_H__
