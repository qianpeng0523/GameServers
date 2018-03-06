#ifndef __LogoLayer_SCENE_H__
#define __LogoLayer_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"
#include "Adaptation.h"
#include "string.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

struct CursorItem{
	CursorItem(){
		tip = NULL;
	}
	string placeholder;
	string fontName;
	float fontSize;
	Text *tip;
	Size dsz;
};



class ColumnListItemLayer : public BaseLayer
{
public:
	ColumnListItemLayer();
	~ColumnListItemLayer();
	static ColumnListItemLayer *create(string coname);
	virtual bool init(string coname);
	void TouchEvent();
	string getColumnName(){
		return m_coname;
	}

private:
	string m_coname;
};





class LogoLayer : public BaseLayer
{
public:
	LogoLayer();
	~LogoLayer();
	CREATE_FUNC(LogoLayer);
    virtual bool init();
	ui::EditBox *AddCursorTextField(Layout *parent,int fontsz=20,bool ispass=false);
	void TouchEvent(Object *obj, TouchEventType type);
	static void ShowPrintf(string str);

	void ButtonStatus(Button *btn,bool isactive);
	void StartStatus(bool isstatus);
	void CloseStatus(bool isstatus);
	void ExcuteStatus(bool isstatus);
	void DBStatus(bool isstatus);
	void TableStatus(bool isstatus);
	void ShowDatabase(string db);
	void ShowTable(string table);

	void ShowFindBar(bool isshow);
	void ShowFindBg(bool isshow);
	void ShowDataItems(map<string, string> datas, string prikey);
	void HideDataItems();
	void setColumns(vector<string> vec);
	void ShowColumnList(bool isshow);

	void setCurColumnName(string coname);
	void setCurColumnValue(string covalue);

	void resetLastColumns(){
		m_lastConames = m_curCoNames;
	}

	void ConnectOprate(bool iscan);
private:
	
private:
	ui::EditBox *m_pNameEditBox;
	ui::EditBox *m_pPassEditBox;
	ui::EditBox *m_pSQLIPEditBox;
	ui::EditBox *m_pSQLPORTEditBox;
	ui::EditBox *m_pDBEditBox;
	ui::EditBox *m_pHttpPORTEditBox;
	ui::EditBox *m_pHttpIPEditBox;
	
	ui::Button *m_startbtn;
	ui::Button *m_closebtn;
	ui::Button *m_excutebtn;

	ui::Button *m_changedbbtn;
	ui::Button *m_changetablebtn;

	static int m_jilucount;
	ui::ScrollView *m_pScrollView;
	Layout *m_sbg;
	vector<ui::EditBox *> m_pEditBoxs;
	ui::EditBox *m_pFindEditBox;
	ui::ScrollView *m_pScrollView1;
	Layout *m_sbg1;
	string m_curconame;
	map<string, string>m_lastConames;
	map<string, string>m_curCoNames;
	string m_curkey;
	string m_curkeyValue;
	Layout *m_mengly[18];
};

#endif // __LogoLayer_SCENE_H__
