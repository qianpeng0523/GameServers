#include "GameDataSet.h"
#include "XXIconv.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<netdb.h>
#include <netinet/in.h>
#endif
USING_NS_CC;
using namespace cocos2d_xx;

int GameDataSet::m_zoder=3;
GameDataSet::GameDataSet(){
	
}

GameDataSet::~GameDataSet(){

}

ScratchItem* GameDataSet::ScratchCard(Layout *parent){
	ScratchItem *item = new ScratchItem();

	item->m_pBase = DrawNode::create();
	item->m_pBase->retain();
	item->m_pBase->drawDot(Point(0, 0), 20.0f, Color4F(255, 0, 0, 255));
	
	Size s = parent->getSize();
	item->m_pRender = RenderTexture::create(s.width, s.height);
	item->m_pRender->setPosition(s/2.0);
	item->m_pRender->retain();
	parent->addChild(item->m_pRender,100); //渲染纹理层需加入该父节点层  
	item->m_pRender->setName("scratch");

	item->m_pBg = Sprite::create("big/bg_guajiang.png"); //这个作为当“油漆层”  
	item->m_pBg->setAnchorPoint(Point(0.5, 0.5));
	item->m_pBg->setPosition(Vec2(s.width / 2, s.height / 2));

	item->m_pRender->begin();
	item->m_pBg->visit();
	item->m_pRender->end();

	return item;
}

void GameDataSet::DelScratch(ScratchItem *p, Button *btn){
	if (p){
		int *a=(int *) p->m_pBase->getUserData();
		if (a){
			delete a;
			a = NULL;
			p->m_pBase->setUserData(NULL);
		}
		p->m_pBase->release();
		p->m_pRender->release();
		delete p;
		p = NULL;

		btn->setUserData(NULL);
		btn->removeChildByName("scratch");
	}
}

bool GameDataSet::ScratchEvent(ScratchItem* p,Button *btn){
	if (p&&btn){
		if (!p->m_pRender){
			return false;
		}
		if (p->m_pBase&&!p->m_pBase->getUserData()){
			int *a = new int[1];
			a[0] = 0;
			p->m_pBase->setUserData(a);
		}
		Size sz = btn->getSize()*btn->getScale();
		auto touchPoint = p->m_pRender->convertToNodeSpace(btn->getTouchMovePosition() + sz / 2.0);
		//log("%g,%g", touchPoint.x, touchPoint.y);
		p->m_pBase->setPosition(touchPoint.x, touchPoint.y);

		BlendFunc blendFunc = { GL_ONE, GL_ZERO };
		p->m_pBase->setBlendFunc(blendFunc);

		p->m_pRender->begin();
		p->m_pBase->visit();
		p->m_pRender->end();

		if ((touchPoint.x<sz.width*1.5 / 6.0&&touchPoint.y>sz.height*5.8 / 7.6) || (touchPoint.x>sz.width*4.5 / 6.0&&touchPoint.y<sz.height*1.8/ 7.6)){
			int *a = (int *)p->m_pBase->getUserData();
			if (a){
				a[0] = 1;
			}
			return true;
		}
		return false;
	}
	return true;
}

void GameDataSet::setVIP(Layout *rootlayer, string name,int vip, bool isB){
	char buff[50];
	sprintf(buff,"v-%d.png",vip);
	if (isB){
		Button *btn = (Button *)GameDataSet::getLayout(rootlayer,name);
		float sc = btn->getScale();
		btn->loadTextureNormal(buff, cocos2d::ui::Widget::TextureResType::PLIST);
		btn->setScale(sc);
		//btn->setPosition(btn->getPosition()+ccp(8,5));
	}
	else{
		ImageView *img = (ImageView *)GameDataSet::getLayout(rootlayer,name);
		float sc = img->getScale();
		img->loadTexture(buff, cocos2d::ui::Widget::TextureResType::PLIST);
		img->setScale(sc);
		//img->setPosition(img->getPosition() + ccp(8, 5));
	}
}

time_t GameDataSet::getTime(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	return tt;
}

string GameDataSet::getTime(time_t tp){
	char buff[100];
	time_t tt =tp;//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d %02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min);
	return buff;
}

string GameDataSet::getLocalTime1(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%02d:%02d",
		t->tm_hour,
		t->tm_min);
	return buff;
}

string GameDataSet::getLocalTime(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d %02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min);
	return buff;
}

string GameDataSet::getLocalTimeDay(){
	char buff[100];
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	sprintf(buff, "%d-%02d-%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday
		);
	return buff;
}

string& GameDataSet::replace_all(string&   str, const   string&   old_value, const   string&   new_value)
{
	while (true)   {
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}
string& GameDataSet::replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())   {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else   break;
	}
	return   str;
}

void GameDataSet::setBg(Layout *bg, string file){
	if (bg){
		bg->setBackGroundImage(file);
	}
}

Layout* GameDataSet::getLayout(Layout *rootlayer, string name){
	Layout *layout = (Layout *)Helper::seekWidgetByName(rootlayer,name);
	return layout;
}

Button *GameDataSet::getButton(Layout *rootlayer, string name, SEL_TouchEvent selector, Object *target){
	Button *btn = (Button *)Helper::seekWidgetByName(rootlayer, name);
	if (btn){
		btn->setTouchEnabled(true);
		btn->addTouchEventListener(target,selector);
	}
	return btn;
}

Text * GameDataSet::setText(Layout *rootlayer, string name, string labelstr, string fonts, bool ishave, ccColor3B c3){
	Text *label = (Text *)getLayout(rootlayer,name);
	if (label){
		label->setFontName(fonts);
		
		if (labelstr.empty()){
			labelstr = label->getStringValue();
		}
		else{
			label->setText(labelstr);
		}
		if (ishave){
            ccColor4B c4=ccc4(c3.r,c3.g,c3.b,label->getOpacity());
           // label->enableOutline(c4);

		}
	}
	return label;
}

Text *GameDataSet::setText(Layout *rootlayer, string name, int labelstr, string fonts, bool ishave, ccColor3B c3){
	Text *label = (Text *)getLayout(rootlayer, name);
	if (label){
		label->setFontName(fonts);
		char buff[30];
		sprintf(buff,"%d",labelstr);
		label->setText(buff);
		if (ishave){
            ccColor4B c4=ccc4(c3.r,c3.g,c3.b,label->getOpacity());
            //label->enableOutline(c4);

		}
	}
	return label;
}

TextAtlas *GameDataSet::setTextAtlas(Layout *rootlayer, string name, string labelstr,int fontsz,ccColor3B c3,bool ishave){
	TextAtlas *label = (TextAtlas *)getLayout(rootlayer, name);
    
	if (label){
		if (fontsz == 0){
			fontsz=label->getSize().height;
		}
       // if(fontsz!=0){
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        char buff[100];
//        sprintf(buff,"%s_%s_%s_001",name.c_str(),name.c_str(),name.c_str());
//        Text *als=(Text *)label->getChildByName(buff);
//        if(!als){
//            als=Text::create();
//            label->getParent()->addChild(als,label->getZOrder());
//            als->setPosition(label->getPosition());
//            als->setFontSize(fontsz);
//            als->setTextHorizontalAlignment(TextHAlignment::LEFT);
//            als->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
//            als->setColor(c3);
//            als->setName(buff);
//            als->setAnchorPoint(label->getAnchorPoint());
//            als->setSize(Size(200,60));
//			
//        }
//           setText(rootlayer, buff, labelstr,ishave);
//		   return (TextAtlas *)als;
//#else
        label->setVisible(true);
        label->setString(labelstr);
//#endif
	}
    
	return (TextAtlas *)label;
}

TextAtlas *GameDataSet::setTextAtlas(Layout *rootlayer, string name, int labelstr,int fontsz,ccColor3B c3,bool ishave){
	TextAtlas *label = (TextAtlas *)getLayout(rootlayer, name);
	char buff[100];
    if (label){
		if (fontsz == 0){
			fontsz=label->getSize().height;
		}
       
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//            sprintf(buff,"%s_%s_%s_001",name.c_str(),name.c_str(),name.c_str());
//            Text *als=(Text *)label->getChildByName(buff);
//            if(!als){
//                als=Text::create();
//                label->getParent()->addChild(als,label->getZOrder());
//                als->setPosition(label->getPosition());
//                als->setFontSize(fontsz);
//                als->setTextHorizontalAlignment(TextHAlignment::LEFT);
//                als->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
//                als->setColor(c3);
//                als->setName(buff);
//                als->setAnchorPoint(label->getAnchorPoint());
//                als->setSize(Size(200,60));
//                
//            }
//           
//            setText(rootlayer, buff, labelstr,ishave);
//			return (TextAtlas *)als;
//#else
            sprintf(buff, "%d", labelstr);
            label->setVisible(true);
            label->setString(buff);
//#endif

    }
	return label;
}

TextBMFont *GameDataSet::setTextBMFont(Layout *rootlayer, string name, string labelstr){
	TextBMFont *label = (TextBMFont *)getLayout(rootlayer, name);
	if (label){
        label->setString(labelstr);
	}
	return label;
}

TextBMFont *GameDataSet::setTextBMFont(Layout *rootlayer, string name,long labelstr){
	char buff[30];
	sprintf(buff,"%ld",labelstr);
	return setTextBMFont(rootlayer, name, buff);
}

ImageView *GameDataSet::setImageView(Layout *rootlayer, string name, string imgfile, cocos2d::ui::Widget::TextureResType texType,bool isbutton){
	ImageView *img = (ImageView *)getLayout(rootlayer, name);
	if (img){
        if(!isbutton){
            img->loadTexture(imgfile, texType);
        }else{
            Button *btn=(Button *)img;
			btn->setScale9Enabled(true);
            btn->loadTextureNormal(imgfile,texType);
        }
	}
	return img;
}

Size GameDataSet::PushScrollItem(Layout *bg, int wcount, Node *item, int index,int prew){
	if (item&&!item->getParent()){
		bg->addChild(item);
	}
	Size sz = item->getContentSize();
	Size isz = bg->getSize();
	
	int num = isz.width/sz.width;

	float du = isz.width/wcount-sz.width;
	float w1 = (isz.width-sz.width*wcount)/(wcount+1);
	if (prew == 0){
		prew = w1;
	}
    int wh=wcount-1;
    float w=0;
    if(wh!=0){
        w = (isz.width - prew * 2-sz.width*wcount) / wh;
    }
	
	float ww = prew;
	if (index > 0){
		ww = prew + w*index;
	}
	Point pos = ccp(sz.width*(index%wcount) + ww, bg->getSize().height - sz.height*(index / wcount + 1));
	item->setPosition(pos);	
	
	return isz;
}

Size GameDataSet::PushScrollItem(Layout *bg, int wcount, int hcount, Node *item, int index, bool isV, int wdis, int secondwd){
	if (item&&!item->getParent()){
		bg->addChild(item,3);
	}
	Size sz = item->getContentSize()*item->getScale();
	Size isz = sz;
	if (hcount == 0 && wcount != 0){
		Point pos = ccp(sz.width*(index%wcount) + wdis*(index + 1), bg->getSize().height - sz.height*(index / wcount + 1));
		item->setPosition(pos);
		isz.width = sz.width*wcount + wdis*(index+1);
		isz.height = sz.height *(index / wcount + 1);
	}
	else if (wcount == 0 && hcount == 0){
		if (isV){
			Point pos = ccp(wdis, bg->getSize().height - sz.height*(index + 1));
			item->setPosition(pos);
			isz.height = sz.height *(index + 1);
		}
		else{
			Point pos = ccp(sz.width*index + wdis, sz.height / 2.0);
			item->setPosition(pos);
			isz.width = sz.width *(index + 1);
		}
	}
	else if (wcount != 0 && hcount != 0)
	{
		Point pos = ccp(sz.width*(index%wcount) + wdis*(index + 1) + secondwd*(index/wcount), bg->getSize().height - sz.height*(index % (wcount*hcount) / wcount + 1));
		item->setPosition(pos);

		isz.width = sz.width*wcount + wdis*(index + 1) + secondwd*(index / wcount);
		isz.height = sz.height *(index % wcount / +1);
	}

	return isz;
}


Size GameDataSet::PushScrollItem(Layout *bg, int wcount, int hcount, Node *item, int index, ui::ScrollView *Scroll, bool isV, bool sbgreset, int wdis, int secondwd){
	Size ppsz = Size(0, 0);
	ppsz = PushScrollItem(bg, wcount, hcount, item, index, isV, wdis, secondwd);
	Size isz = Scroll->getInnerContainerSize();
	if (isz.width < ppsz.width){
		Scroll->setInnerContainerSize(Size(ppsz.width, isz.height));
	}
	isz = Scroll->getInnerContainerSize();
	if (isz.height < ppsz.height){
		Scroll->setInnerContainerSize(Size(isz.width, ppsz.height));
	}
	isz = Scroll->getInnerContainerSize();
	if (sbgreset){
		bg->setPositionY(isz.height - bg->getSize().height);
	}
	return ppsz;
}

void GameDataSet::PushPageItem(Node *item, PageView *page, int wcount, int hcount){
	int pcount = page->getChildrenCount();
	Size sz = page->getSize();
	Size nsz = item->getContentSize();
	float wdis = (sz.width - wcount*nsz.width) / (wcount + 1);
	float hdis = (sz.height - hcount*nsz.height) / (hcount + 1);
	Layout *ly = NULL;
	Point apos = item->getAnchorPoint();
	if (pcount > 0){
		ly = page->getPage(pcount - 1);
		int icount = ly->getChildrenCount();
		if (icount>0 && icount % (wcount*hcount) == 0){
			ly = Layout::create();
			ly->setSize(sz);
			page->addPage(ly);
			icount = ly->getChildrenCount();
		}

		Point pos = ccp(wdis*(icount % wcount + 1) + nsz.width*(icount%wcount + apos.x), sz.height - (hdis*(icount / wcount + 1) + nsz.height*(icount / wcount + apos.y)));
		ly->addChild(item);
		item->setPosition(pos);
	}
	else{
		ly = Layout::create();
		ly->setSize(sz);
		page->addPage(ly);

		PushPageItem(item, page, wcount, hcount);
	}
}

bool GameDataSet::isTouchInChild(Node *node, Point beginpos, Point endpos, float distance, SEL_CallFunc selector){
	if (node){
		Point btnpos = node->getParent()->convertToNodeSpace(endpos);
		CCRect rect = node->boundingBox();
		if (rect.containsPoint(btnpos)){
			float dis = ccpDistance(beginpos, endpos);
			if (dis < distance){
				if (selector){
					(node->*selector)();
				}
				return true;
			}
			return false;
		}
	}
	return false;
}

Node* GameDataSet::isTouchInChild(ui::ScrollView *Scroll, float distance, SEL_CallFunc selector){
	Vector<Node *>svecs = Scroll->getChildren();
	if (!svecs.empty()){
		Vector<Node *>vecs = svecs.at(svecs.size()-1)->getChildren();
		Point beginpos = Scroll->getTouchBeganPosition();
		Point endpos = Scroll->getTouchEndPosition();
		for (int i = 0; i < vecs.size(); i++){
			if (isTouchInChild(vecs.at(i), beginpos, endpos, distance, selector)){
				Node *node = vecs.at(i);
				return node;
			}
		}
	}
	return NULL;
}

Node* GameDataSet::isTouchInChild(PageView *page, float distance, SEL_CallFunc selector){
	Vector<Node *>svecs = page->getChildren();
	if (!svecs.empty()){
		for (int j = 0; j < svecs.size(); j++){
			Vector<Node *>vecs = svecs.at(j)->getChildren();
			Point beginpos = page->getTouchBeganPosition();
			Point endpos = page->getTouchEndPosition();
			for (int i = 0; i < vecs.size(); i++){
				if (isTouchInChild(vecs.at(i), beginpos, endpos, distance, selector)){
					return vecs.at(i);
				}
			}
		}
	}
	return NULL;
}

LoadingBar *GameDataSet::setLoadingBarPercent(Layout *rootlayer, string name, float per){
	LoadingBar *pro = (LoadingBar *)getLayout(rootlayer, name);
	if (pro){
		pro->setPercent(per);
	}
	return pro;
}

Layout *GameDataSet::setIcon(Layout *rootlayer, string name, int sex, int photoid, bool isB, string uid){
	float sx = 1.0;
	char buff[100];
	int id = atoi(uid.c_str());
	sprintf(buff, "head/head%d%d.png", sex,id%3);
	if (!isB){
		ImageView *img = (ImageView *)getLayout(rootlayer, name);
		if (img){
			img->loadTexture(buff);
			sx = img->getScale();
			img->setScale(sx);
		}
		return (Layout *)img;
	}
	else{
		Button *img = (Button *)getLayout(rootlayer, name);
		if (img){
			img->loadTextureNormal(buff);
			sx = img->getScale();
			img->setScale(sx);
		}
		return (Layout *)img;
	}
	return NULL;
}

ImageView *GameDataSet::setSex(Layout *rootlayer, string name, int sex){
	ImageView *img = (ImageView *)GameDataSet::getLayout(rootlayer,name);
	if (img){
		if (sex == 1){
			img->loadTexture("head/head11.png");
		}
		else{
			img->loadTexture("head/head00.png");
		}
		img->setVisible(false);
	}
	return img;
}

void GameDataSet::setZoomScale(Button *btn, TouchEventType type){
	float sx = 1.0;
	Point beginpos = btn->convertToNodeSpace(btn->getTouchBeganPosition());
	bool isbgIn = btn->getBoundingBox().containsPoint(beginpos);
	if (isbgIn&&type == TOUCH_EVENT_BEGAN){
		sx += 0.1;
	}
	else{
		Point mvpos = btn->convertToNodeSpace(btn->getTouchMovePosition());
		bool ismvIn = btn->getBoundingBox().containsPoint(mvpos);
		if (ismvIn){
			sx += 0.1;
		}
	}

	Vector<Node *>vecs = btn->getChildren();
	for (int i = 0; i < vecs.size(); i++){
		Node *node = vecs.at(i);
		node->setScale(sx);
	}
}



vector<Point> GameDataSet::getNodeFourPoint(CCNode *node){
	//1.算矩形的4个点
	Point cpos = node->getPosition();//中心点
	Size ndsz = node->getContentSize();
	float w = ndsz.width;//长
	float h = ndsz.height;//宽
	float ro = -node->getRotation();//角度
	
	float m_h = ro / 180 * 3.1415926;
	
	float y = sin(m_h)*w / 2;
	float x = cos(m_h)*w / 2;

	Point temp1 = ccp(cpos.x + x, cpos.y + y);
	Point temp2 = ccp(cpos.x - x, cpos.y - y);

	float x1 = sin(m_h)*h / 2;
	float y1 = cos(m_h)*h / 2;

	Point p1 = ccp(temp1.x - x1, temp1.y + y1);
	Point p2 = ccp(temp1.x + x1, temp1.y - y1);
	
	Point p3 = ccp(temp2.x + x1, temp2.y - y1);
	Point p4 = ccp(temp2.x - x1, temp2.y + y1);

	vector<Point> poss;
	poss.push_back(p1);
	poss.push_back(p2);
	poss.push_back(p3);
	poss.push_back(p4);
	return poss;
}

float GameDataSet::GetCross(Point p1, Point p2, Point p)
{// 计算 |p1 p2| X |p1 p|  
	return (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
}

//判断点是否在5X5 以原点为左下角的正方形内（便于测试）  
bool GameDataSet::IsPointInMatrix(Point p1, Point p2, Point p3, Point p4, Point p)
{
	return GetCross(p1, p2, p) * GetCross(p3, p4, p) >= 0 && GetCross(p2, p3, p) * GetCross(p4, p1, p) >= 0;
	//return false;  
}


bool GameDataSet::isPosInNode(CCNode *node, Point pos){
	vector<Point> poss = getNodeFourPoint(node);
	return IsPointInMatrix(poss.at(0), poss.at(1), poss.at(2), poss.at(3),pos);
}

vector <string > GameDataSet::strLength(const std::string &str)
{
	vector <string > dump;
	if (typeid(str) == typeid(std::string) && str.length() > 0) {
		int len = str.length();
		int i = 0;
		while (i < len) {
			if (~(str.at(i) >> 8) == 0) {
				dump.push_back(str.substr(i, 3));
				i = i + 3;
			}
			else {
				dump.push_back(str.substr(i, 1));
				i = i + 1;
			}
		}
		return dump;
	}
	else {
		CCLOG("str is not string\n");
		return dump;
	}
}

void GameDataSet::setLabelLimit(string str, Text *label, float width, string replace){
	vector <string > vecs = strLength(str);
	string temp;
	for (int i = 0; i < vecs.size(); i++){
		string tt = temp + vecs.at(i);
		label->setString(tt);
		float wd = label->getSize().width;
		if (wd <= width){
			temp = tt;
		}
		else{
			label->setString(temp + replace);
			return;
		}
	}
}

void GameDataSet::setLabelLimitInt(string str, Text *label, int count, string replace){
	vector <string > vecs = strLength(str);
	string temp;
	string tp;
	for (int i = 0; i < vecs.size(); i++){
		string tt = temp + vecs.at(i);
		if (i == count - 2){
			tp = tt;
		}
		if (i < count){
			temp = tt;
			if (i == vecs.size() - 1){
				label->setString(temp);
			}
		}
		else{
			label->setString(tp + replace);
			return;
		}
	}
}

void GameDataSet::setLabelLimit(Layout *rootlayer, string name, string str, float width, string replace){
	Text *label = (Text *)getLayout(rootlayer, name);
	setLabelLimit(str, label, width, replace);
}

void GameDataSet::setLabelLimitInt(Layout *rootlayer, string name, string str, int count, string replace){
	Text *label = (Text *)getLayout(rootlayer, name);
	setLabelLimitInt(str, label, count, replace);
}

CCNode *GameDataSet::setTextStrokLabel(CCLabelTTF *tx, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname){
	if (str.empty()){
		str = tx->getString();
	}
	CCNode *label = textAddStroke(str.c_str(), fontname.c_str(), tx->getFontSize(), c1, 1, c2, NULL, tx);

	return (CCNode *)label;

}

CCNode *GameDataSet::setTextStrokLabel(Text *tx, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname){
	if (str.empty()){
		str = tx->getStringValue();
	}
	                                                                                
	CCNode *label = textAddStroke(str.c_str(), fontname.c_str(), tx->getFontSize(), c1, 1, c2, tx);

	return label;

}

CCNode *GameDataSet::setTextStrokLabel(Layout *root, string name, string str, const ccColor3B & c1, const ccColor3B & c2, string fontname){
	Text *tx = (Text *)getLayout(root, name);
	return setTextStrokLabel(tx, str, c1, c2, fontname);
}

/*
制作文字描边效果是很简单的，我们写好一段文字之后，也就是创建出一个CCLabelTTF，称之为正文CCLabelTTF。然后再创建出4个CCLabelTTF，颜色为黑色，大小同正文CCLabelTTF相同，
称之为描边CCLabelTTF。说到这大家可能已经明白了，没错，就是把4个描边CCLabelTTF放于正文CCLabelTTF的下面，分别于左右上下与正文CCLabelTTF错开，这样描边效果就实现啦。。

*string     文本
*fontName   文本字体类型
*fontSize   文本大小
*color3     文本颜色
*lineWidth  所描边的宽度
*/
CCNode* GameDataSet::textAddStroke(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float lineWidth, const ccColor3B &c2, Text *tx, CCLabelTTF *ttf)
{
	//正文CCLabelTTF
	CCNode* center = NULL;
	TextHAlignment alignmenth;
	TextVAlignment alignmentv;
	Size csz = Size(0, 0);
	Size asz = Size(0,0);
	if (tx){
		center = tx;
		tx->setFontName(fontName);
		tx->setFontSize(fontSize);
		tx->setString(string);
		center->setColor(color3);

		alignmenth = tx->getTextHorizontalAlignment();
		alignmentv = tx->getTextVerticalAlignment();
		csz = tx->getSize();
		asz = tx->getTextAreaSize();
		center->setContentSize(csz);
	}
	else if (ttf){
		center = ttf;
		ttf->setFontName(fontName);
		ttf->setFontSize(fontSize);
		ttf->setString(string);
		center->setColor(color3);

		alignmenth = ttf->getHorizontalAlignment();
		alignmentv = ttf->getVerticalAlignment();
		csz = ttf->getContentSize();
		asz = ttf->getDimensions();
	}
	else{
		center = Text::create(string, fontName, fontSize);
		center->setColor(color3);

		alignmenth = ((Text *)center)->getTextHorizontalAlignment();
		alignmentv = ((Text *)center)->getTextVerticalAlignment();
		csz = ((Text *)center)->getSize();
		asz = ((Text *)center)->getTextAreaSize();

		center->setContentSize(csz);
	}
	
	//描边CCLabelTTF 上
	Point apos1 = center->getAnchorPoint();
	Size sz = center->getContentSize();
	int op = center->getOpacity();
	Point apos = ccp(0, 0);
	Point pos = ccp(sz.width,sz.height);
	Layout *parent = (Layout *)center->getParent();
	
	Text* up = (Text *)center->getChildByName("up");
	Text* down = (Text *)center->getChildByName("down");
	Text* left = (Text *)center->getChildByName("left");
	Text* right = (Text *)center->getChildByName("right");
	if (!up){
		up = Text::create(string, fontName, fontSize);
		down = Text::create(string, fontName, fontSize);
		left = Text::create(string, fontName, fontSize);
		right = Text::create(string, fontName, fontSize);
		center->addChild(up, -1);
		center->addChild(down, -1);
		center->addChild(left, -1);
		center->addChild(right, -1);
	}
	up->setColor(c2);
	up->setPosition( ccp(0, 0 + lineWidth));
	
	up->setTextHorizontalAlignment(alignmenth);
	up->setTextVerticalAlignment(alignmentv);
	up->setSize(csz);
	up->setContentSize(csz);
	up->ignoreContentAdaptWithSize(false);
	up->setName("up");
	up->setAnchorPoint(apos);
	up->setOpacity(op);
	//up->setTextAreaSize(asz);

	//描边CCLabelTTF 下
		
	down->setColor(c2);
	down->setPosition(ccp(0, 0 - lineWidth));
	
	down->setTextHorizontalAlignment(alignmenth);
	down->setTextVerticalAlignment(alignmentv);
 	down->setSize(csz);
 	down->setContentSize(csz);
	down->setName("down");
	down->ignoreContentAdaptWithSize(false);
	down->setAnchorPoint(apos);
	down->setOpacity(op);
	//down->setTextAreaSize(asz);

	//描边CCLabelTTF 左
	left->setPosition(ccp(0 - lineWidth, 0));
	left->setColor(c2);
	
	left->setTextHorizontalAlignment(alignmenth);
	left->setTextVerticalAlignment(alignmentv);
	left->setSize(csz);
	left->setContentSize(csz);
	left->setName("left");
	left->ignoreContentAdaptWithSize(false);
	left->setAnchorPoint(apos);
	left->setOpacity(op);
	//left->setTextAreaSize(asz);

	//描边CCLabelTTF 右
	right->setColor(c2);
	right->setPosition(ccp(0 + lineWidth, 0));
	
	right->setTextHorizontalAlignment(alignmenth);
	right->setTextVerticalAlignment(alignmentv);
 	right->setSize(csz);
	right->setContentSize(csz);
	right->setName("right");
	right->ignoreContentAdaptWithSize(false);
	right->setAnchorPoint(apos);
	right->setOpacity(op);
	//right->setTextAreaSize(asz);
	
	up->setString(string);
	down->setString(string);
	left->setString(string);
	right->setString(string);
	return center;
}


/*
给文字添加阴影，一看就懂的。。。
*string         文本
*fontName       文本字体类型
*fontSize       文本大小
*color3         文本颜色
*shadowSize     阴影大小
*shadowOpacity  阴影透明度
*/
Text* GameDataSet::textAddShadow(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float shadowSize, float shadowOpacity, const ccColor3B &c2, Text *tx)
{
	Text* shadow = Text::create(string, fontName, fontSize);
	shadow->setColor(c2);
	shadow->setOpacity(shadowOpacity);

	Text* center = NULL;
	if (!tx){
		center = Text::create(string, fontName, fontSize);
	}
	else{
		center = tx;
	}
	center->setColor(color3);
	center->setPosition(ccp(shadow->getContentSize().width*0.5 - shadowSize, shadow->getContentSize().height*0.5 + shadowSize));
	shadow->addChild(center);

	return shadow;
}


//既添加描边又添加阴影
CCNode* GameDataSet::textAddOutlineAndShadow(const char* string, const char* fontName, float fontSize, const ccColor3B &color3, float lineWidth, float shadowSize, float shadowOpacity, const ccColor3B &c2, Text *tx)
{
	CCNode* center = textAddStroke(string, fontName, fontSize, color3, lineWidth, c2);

	Text* shadow = Text::create(string, fontName, fontSize);
	shadow->setPosition(ccp(center->getContentSize().width*0.5 + shadowSize, center->getContentSize().height*0.5 - shadowSize));
	shadow->setColor(c2);
	shadow->setOpacity(shadowOpacity);
	center->addChild(shadow, -1);

	return center;
}

string GameDataSet::getStringByInteger(int low, bool isInt){
	char buff[100];
	if (abs(low) < 1000){
		sprintf(buff, "%d", low);
	}
	else if (abs(low) < 10000){
		sprintf(buff, "%dK", low / 1000);
	}
	else if (abs(low) < 10000000){
		if (low % 10000 == 0){
			sprintf(buff, "%dW", low / 10000);
		}
		else{
			sprintf(buff, "%dW%dK", low / 10000, low % 10000 / 1000);
		}
	}
	else{
		if (low % 10000000 == 0){
			sprintf(buff, "%dKW", low / 10000000);
		}
		else if (low % 10000 == 0){
			sprintf(buff, "%dKW%dW", low / 10000000, low / 10000);
		}
		else{
			sprintf(buff, "%dKW%dW%dK", low / 10000000, low % 10000000 / 10000, low % 10000000 / low % 10000 / 1000);
		}
	}
	return buff;
}

string GameDataSet::getCNStringByInteger(long low, bool isInt){
	char buff[100];
	if (!isInt){
		if (abs(low) < 10000){
			int a = (int)low;
			sprintf(buff, XXIconv::GBK2UTF("%d").c_str(), low);
		}
		else if (abs(low) < 100000000){
			float a = low / 10000.0;
			sprintf(buff, XXIconv::GBK2UTF("%.02f万").c_str(), a);
		}
		else{
			float a = low / 100000000.0;
			sprintf(buff, XXIconv::GBK2UTF("%.02f亿").c_str(), a);
		}
	}
	else{
		if (abs(low) < 10000){
			sprintf(buff, XXIconv::GBK2UTF("%d").c_str(), low);
		}
		else if (abs(low) < 100000000){
			sprintf(buff, XXIconv::GBK2UTF("%d万").c_str(), low / 10000);
		}
		else{
			sprintf(buff, XXIconv::GBK2UTF("%d亿").c_str(), low / 100000000);
		}
	}

	log("%s",buff);
	
	return buff;
}

int GameDataSet::getTime(time_t ptimep, int type){
	struct   tm  * tm;
	tm = localtime(&ptimep);
	int  year = tm->tm_year + 1900;
	int  month = tm->tm_mon + 1;
	int  day = tm->tm_mday;
	int  hour = tm->tm_hour;
	int  min = tm->tm_min;
	int  second = tm->tm_sec;
	if (type == 1){
		return year;
	}
	else if (type == 2){
		return month;
	}
	else if (type == 3){
		return day;
	}
	else if (type == 4){
		return hour;
	}
	else if (type == 5){
		return min;
	}
	else if (type == 6){
		return second;
	}
	return ptimep;
}

void GameDataSet::LayerRunActOpen(Layer *layer, CCNode *targetnode, int type){
	if (targetnode&&layer){
		Layout *rootlayer = ((BaseLayer *)layer)->getRootLayer();
		Vector<Node *>nodes = rootlayer->getChildren();
		for (int i = 0; i < nodes.size();i++){
			Node *node = nodes.at(i);
			node->setScale(layer->getScale() / 2.0);
			node->runAction(CCSequence::create(CCScaleTo::create(0.2, 1.02), CCScaleTo::create(0.15, 1.0), NULL));
		}
		
		
	}
}

void GameDataSet::LayerRunActClose(Layer *layer){
	if (layer){
		Layout *rootlayer = ((BaseLayer *)layer)->getRootLayer();
		Vector<Node *>nodes = rootlayer->getChildren();
		for (int i = 0; i < nodes.size(); i++){
			Node *node = nodes.at(i);
			node->runAction(CCSequence::create(CCScaleTo::create(0.2, 1.05), CCScaleTo::create(0.2, layer->getScale()*0.5), NULL));
		}
		layer->runAction(CCSequence::create(CCDelayTime::create(0.4), CCRemoveSelf::create(true), NULL));
	}
}

string GameDataSet::getHostNameIp(string hostname){
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];
	uint16_t port;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET 或 AF_INET6   
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(hostname.c_str(), NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return "";
	}

	log("IP addresses for %s:\n\n", hostname.c_str());

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		if (p->ai_family == AF_INET) { // IPv4  
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
			port = ntohs(((struct sockaddr_in*)p->ai_addr)->sin_port);
		}
		else { // IPv6  
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
			port = ntohs(((struct sockaddr_in6*)p->ai_addr)->sin6_port);
		}

		// convert the IP to a string and print it:  
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		log(" %s: %s,port is %u\n", ipver, ipstr, port);
	}

	freeaddrinfo(res);
	return ipstr;
}

void capCallBack(bool b, const std::string& strPath)
{
	return;
}

bool GameDataSet::ScreenShot(string file){
	utils::captureScreen(capCallBack, file);
	return true;
 }

int GameDataSet::getSceneNextZorder(){
	CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
    if(scene){
        Camera* car = scene->getDefaultCamera();
        Vector<Node *>childs = scene->getChildren();
        int count = childs.size();
        return count+1;
    }else{
        return 0;
    }
	
}

void GameDataSet::PushLayerToScene(Layer *layer){
	int zoder = getSceneNextZorder();
	CCScene *scene = CCDirector::sharedDirector()->getRunningScene();
	if (scene){
		scene->addChild(layer,zoder);
	}
}
