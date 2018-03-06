#ifndef __GameDataSet_SCENE_H__
#define __GameDataSet_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "AppMacros.h"
#include "cocos/ui/UILayout.h"
#include "cocos/ui/UIImageView.h"
#include "cocos/ui/UIButton.h"
#include "cocos/ui/UIScrollView.h"
#include "cocos/ui/UIPageView.h"
#include "cocos/ui/UITextAtlas.h"
#include "cocos/ui/UICheckBox.h"
#include "cocos/ui/UISlider.h"
#include "cocos/ui/UILoadingBar.h"
#include "cocos/ui/UITextBMFont.h"
#include "cocos/editor-support/cocostudio/CCSGUIReader.h"
#include "cocos/ui/UIHelper.h"
#include "cocos/ui/UIText.h"
#include "BaseLayer.h"
using namespace cocos2d;
using namespace ui;
using namespace extension;
using namespace std;
using namespace cocostudio;

//刮奖
struct ScratchItem
{
	DrawNode *m_pBase;
	RenderTexture *m_pRender;
	Sprite *m_pBg;
};

class GameDataSet : public Layer
{
public:
	GameDataSet();
	~GameDataSet();
	static string& replace_all(string&   str, const   string&   old_value, const   string&   new_value);
	static string& replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value);
	static Layout* getLayout(Layout *rootlayer, string name);
	static Button *getButton(Layout *rootlayer, string name, SEL_TouchEvent selector, Object *target);
	static Text *setText(Layout *rootlayer, string name, string labelstr, string fonts = FONTS1, bool ishave = true, ccColor3B c3 = ccc3(0, 0, 0));
	static Text *setText(Layout *rootlayer, string name, int labelstr, string fonts = FONTS1, bool ishave = true, ccColor3B c3 = ccc3(0, 0, 0));
	static TextAtlas *setTextAtlas(Layout *rootlayer, string name, string labelstr, int fontsz = 0, ccColor3B c3 = ccc3(255, 255, 255), bool ishave = true);
	static TextAtlas *setTextAtlas(Layout *rootlayer, string name, int labelstr, int fontsz = 0, ccColor3B c3 = ccc3(255, 255, 255), bool ishave = true);
	static TextBMFont *setTextBMFont(Layout *rootlayer, string name, string labelstr);
	static TextBMFont *setTextBMFont(Layout *rootlayer, string name, long labelstr);
	static ImageView *setImageView(Layout *rootlayer, string name, string imgfile, cocos2d::ui::Widget::TextureResType texType = cocos2d::ui::Widget::TextureResType::LOCAL, bool isbutton = false);
	static Size PushScrollItem(Layout *bg, int wcount, int hcount, Node *item, int index, bool isV = true, int wdis = 0, int secondwd=0);
	static Size PushScrollItem(Layout *bg, int wcount, int hcount, Node *item, int index, ui::ScrollView *Scroll, bool isV = true, bool sbgreset = true, int wdis = 0,int secondwd=0);
	static Size PushScrollItem(Layout *bg, int wcount, Node *item, int index, int prew=0);

	static void PushPageItem(Node *item,PageView *page,int wcount=1,int hcount=1);

	static bool isTouchInChild(Node *node, Point beginpos, Point endpos, float distance, SEL_CallFunc selector);
	static Node* isTouchInChild(ui::ScrollView *Scroll, float distance, SEL_CallFunc selector);
	static Node* isTouchInChild(PageView *page, float distance, SEL_CallFunc selector);
	static LoadingBar *setLoadingBarPercent(Layout *rootlayer, string name, float per);
	static void setVIP(Layout *rootlayer, string name,int vip, bool isB = false);
	static Layout *setIcon(Layout *rootlayer, string name, int sex, int photoid, bool isB = false,string uid="");
	static ImageView *setSex(Layout *rootlayer, string name, int sex);
	static void setZoomScale(Button *btn, TouchEventType type);
	//
	static vector<Point> getNodeFourPoint(CCNode *node);

	static bool isPosInNode(CCNode *node, Point pos);
	static bool IsPointInMatrix(Point p1, Point p2, Point p3, Point p4, Point p);
	static float GetCross(Point p1, Point p2, Point p);

	static vector <string > strLength(const std::string &str);
	static void setLabelLimitInt(string str, Text *label, int count, string replace = "...");
	static void setLabelLimit(string str, Text *label, float width, string replace = "...");
	static void setLabelLimit(Layout *rootlayer, string name, string str, float width, string replace = "...");
	static void setLabelLimitInt(Layout *rootlayer, string name, string str, int count, string replace="...");

	static CCNode *setTextStrokLabel(CCLabelTTF *tx, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname);
	static CCNode *setTextStrokLabel(Text *tx, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname);
	static CCNode *setTextStrokLabel(Layout *root, string name, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname);
	
	static CCNode* textAddStroke(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float lineWidth, const ccColor3B &c2 = ccBLACK, Text *tx = NULL, CCLabelTTF *ttf = NULL);
	
	static Text* textAddShadow(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float shadowSize, float shadowOpacity, const ccColor3B &c2 = ccBLACK, Text *tx = NULL);
	
	static CCNode* textAddOutlineAndShadow(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float lineWidth, float shadowSize, float shadowOpacity, const ccColor3B &c2 = ccBLACK, Text *tx = NULL);

	static string getStringByInteger(int low, bool isInt=false);
	static string getCNStringByInteger(long low, bool isInt = false);
	static int getTime(time_t ptimep, int type);//type 1年 2月 3日 4时 5分 6秒 

	static void LayerRunActOpen(Layer *layer, CCNode *targetnode, int type=1);//在不同层之间的
	static void LayerRunActClose(Layer *layer);//在不同层之间的

	static string getHostNameIp(string hostname);
	static void setBg(Layout *bg,string file="big/dn_inhall_bg.png");

	static bool ScreenShot(string file);
	static int getSceneNextZorder();
	static void PushLayerToScene(Layer *layer);
	static string getTime(time_t tp);
	static string getLocalTime();
	static string getLocalTime1();
	static string getLocalTimeDay();
	static time_t getTime();

	static ScratchItem* ScratchCard(Layout *parent);//刮奖
	static bool ScratchEvent(ScratchItem* p, Button *btn);
	static void DelScratch(ScratchItem *p,Button *btn);
private:
	static int m_zoder;
};

#endif 
