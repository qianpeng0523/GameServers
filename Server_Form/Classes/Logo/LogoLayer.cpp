#include "LogoLayer.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GameDataSet.h"
#include "HttpInfo.h"
#include "ServerControl.h"

USING_NS_CC;




ColumnListItemLayer::ColumnListItemLayer(){

}

ColumnListItemLayer::~ColumnListItemLayer(){

}

bool ColumnListItemLayer::init(string coname)
{
	if (!Layer::init())
	{
		return false;
	}
	m_coname = coname;
	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("listitem.json");
	this->addChild(m_RootLayer);

	this->setContentSize(m_RootLayer->getSize());

	GameDataSet::setText(m_RootLayer, "text", coname);
	return true;
}

ColumnListItemLayer *ColumnListItemLayer::create(string dbname){
	ColumnListItemLayer *p = new ColumnListItemLayer();
	if (p&&p->init(dbname)){
		p->autorelease();
	}
	else{
		CC_SAFE_DELETE(p);
	}
	return p;
}

void ColumnListItemLayer::TouchEvent(){
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	if (p){
		p->setCurColumnName(m_coname);
		p->ShowColumnList(false);
	}
}



















int LogoLayer::m_jilucount = 0;
LogoLayer::LogoLayer(){
	ServerControl::getIns()->setLogoLayer(this);
	
}

LogoLayer::~LogoLayer(){
	if (this==ServerControl::getIns()->getLogoLayer()){
		ServerControl::getIns()->setLogoLayer(NULL);
	}
}

bool LogoLayer::init()
{
	if (!Layer::init())
    {
        return false;
    }

	m_RootLayer = (Layout *)GUIReader::shareReader()->widgetFromJsonFile("sqlserver.json");
	this->addChild(m_RootLayer);
	
	UserDefault *pUserDefault = UserDefault::sharedUserDefault();
	string ip = pUserDefault->getStringForKey("ip");
	string name = pUserDefault->getStringForKey("name");
	string pass = pUserDefault->getStringForKey("pass");
	string db = pUserDefault->getStringForKey("db");
	int port = pUserDefault->getIntegerForKey("port");
	string httpip = pUserDefault->getStringForKey("httpip");
	string httpport = pUserDefault->getStringForKey("httpport");

	m_pNameEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer,"ppname"));
	m_pNameEditBox->setText(name.c_str());
	m_pPassEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "pppwd"),20,true);
	m_pPassEditBox->setText(pass.c_str());
	m_pSQLIPEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "ppip"));
	m_pSQLIPEditBox->setText(ip.c_str());
	m_pSQLPORTEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "ppport"));
	char buff[30];
	sprintf(buff,"%d",port);
	m_pSQLPORTEditBox->setText(buff);
	m_pDBEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "ppdb"));
	m_pDBEditBox->setText(db.c_str());
	
	m_pHttpPORTEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "pphttpport"));
	m_pHttpPORTEditBox->setText(httpport.c_str());
	m_pHttpIPEditBox = AddCursorTextField(GameDataSet::getLayout(m_RootLayer, "pphttp"));
	m_pHttpIPEditBox->setText(httpip.c_str());

	SEL_TouchEvent selector = toucheventselector(LogoLayer::TouchEvent);
	m_startbtn=GameDataSet::getButton(m_RootLayer, "start_btn", selector, this);
	m_closebtn= GameDataSet::getButton(m_RootLayer, "close_btn", selector, this);
	ButtonStatus(m_closebtn,false);
	m_excutebtn= GameDataSet::getButton(m_RootLayer, "excute_btn", selector, this);
	ButtonStatus(m_excutebtn, false);

	m_changedbbtn = GameDataSet::getButton(m_RootLayer, "dbbtn", selector, this);
	ButtonStatus(m_changedbbtn, false);
	m_changetablebtn = GameDataSet::getButton(m_RootLayer, "tablebtn", selector, this);
	ButtonStatus(m_changetablebtn, false);

	ShowDatabase(XXIconv::GBK2UTF("未连接"));
	ShowTable(XXIconv::GBK2UTF("未连接"));

	m_pScrollView = (ui::ScrollView *)GameDataSet::getButton(m_RootLayer, "ScrollView", selector, this);
	m_sbg = GameDataSet::getLayout(m_RootLayer, "ssbg");
	m_pScrollView1 = (ui::ScrollView *)GameDataSet::getButton(m_RootLayer, "ScrollView_1", selector, this);
	m_sbg1 = GameDataSet::getLayout(m_RootLayer, "listbg1");
	ShowFindBar(false);
	ShowFindBg(false);

	GameDataSet::getButton(m_RootLayer, "findbtn", selector, this);
	
	for (int i = 0; i < 18; i++){
		sprintf(buff,"d1bg_%d",i);
		Layout *ly = GameDataSet::getLayout(m_RootLayer,buff);
		Layout *ly1 = GameDataSet::getLayout(ly, "d1sbg");
		ly1->setTouchEnabled(false);
		ui::EditBox *eb = AddCursorTextField(ly1,18);
		m_pEditBoxs.push_back(eb);
		Layout *ll = Layout::create();
		ll->setSize(ly1->getSize());
		ll->setBackGroundColor(ccc3(0x3E, 0x3E, 0x40));
		ll->setBackGroundColorOpacity(120);
		ly1->addChild(ll);
		ll->setVisible(false);
		ll->setTouchEnabled(true);
		ll->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
		m_mengly[i] = ll;
	}
	Layout * fingly = GameDataSet::getLayout(m_RootLayer, "findeditly");
	m_pFindEditBox = AddCursorTextField(fingly,18);
	m_pFindEditBox->setPlaceHolder(XXIconv::GBK2UTF("输入查找的值").c_str());
	GameDataSet::getButton(m_RootLayer, "select_bg", selector, this);

	ShowColumnList(false);
	HideDataItems();
	ConnectOprate(false);
    return true;
}

ui::EditBox *LogoLayer::AddCursorTextField(Layout *parent, int fontsz, bool ispass){
	parent->setClippingEnabled(true);
	parent->setClippingType(Layout::ClippingType::SCISSOR);
	

	CursorItem *pCursorItem = new CursorItem();
	pCursorItem->fontName = "";
	pCursorItem->fontSize = fontsz;
	pCursorItem->dsz = parent->getSize();
	int limit = 100;
	int s = 0;
	ui::EditBox::InputMode mode = ui::EditBox::InputMode::SINGLE_LINE;
	ui::EditBox::InputFlag flag = ui::EditBox::InputFlag::INITIAL_CAPS_WORD;
	if (ispass){
		flag = ui::EditBox::InputFlag::PASSWORD;
	}
	ui::EditBox *pCursorTextField = ui::EditBox::create(pCursorItem->dsz, "touming.png");
	pCursorTextField->setPlaceHolder(pCursorItem->placeholder.c_str());
	pCursorTextField->setTouchEnabled(true);
	pCursorTextField->setAnchorPoint(ccp(0, 0));
	pCursorTextField->setPlaceholderFont(pCursorItem->fontName.c_str(), pCursorItem->fontSize);
	pCursorTextField->setMaxLength(limit);
	pCursorTextField->setInputMode(mode);
	pCursorTextField->setInputFlag(flag);
	pCursorTextField->setFontName(pCursorItem->fontName.c_str());
	pCursorTextField->setFontSize(pCursorItem->fontSize);
	pCursorTextField->setReturnType(ui::EditBox::KeyboardReturnType::DONE);


	parent->addChild(pCursorTextField);


	return pCursorTextField;
}

void LogoLayer::TouchEvent(Object *obj, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED){
		Button *btn = (Button *)obj;
		string name = btn->getName();
		if (name.compare("start_btn") == 0){
			string name = m_pNameEditBox->getText();
			string pass = m_pPassEditBox->getText();
			string ip = m_pSQLIPEditBox->getText();
			int port = atoi(m_pSQLPORTEditBox->getText());
			string db = m_pDBEditBox->getText();
			string httpip = m_pHttpIPEditBox->getText();
			string httpport = m_pHttpPORTEditBox->getText();
			if (ip.empty()){
				ShowPrintf("ip为空");
			}
			else if (name.empty()){
				ShowPrintf("用户名为空");
			}
			else if (pass.empty()){
				ShowPrintf("密码为空");
			}
			else if (db.empty()){
				ShowPrintf("数据库名为空");
			}
			else if (port<=0){
				ShowPrintf("端口为空或为0");
			}
			else if (httpip.empty()){
				ShowPrintf("服务器ip为空");
			}
			else if (httpport.empty()){
				ShowPrintf("服务器端口为空");
			}
			else{
				UserDefault *pUserDefault= UserDefault::sharedUserDefault();
				pUserDefault->setStringForKey("ip", ip);
				pUserDefault->setStringForKey("name", name);
				pUserDefault->setStringForKey("pass", pass);
				pUserDefault->setStringForKey("db", db);
				pUserDefault->setIntegerForKey("port", port);
				pUserDefault->setStringForKey("httpip", httpip);
				pUserDefault->setStringForKey("httpport", httpport);

				SQLInfo info;
				info._ip = ip;
				info._db = db;
				info._name = name;
				info._pass = pass;
				info._port = port;
				info._httpip = httpip;
				info._httpport = httpport;

				HttpInfo::getIns()->requestSQLStart(info);
			}
			
		}
		else if (name.compare("close_btn") == 0){
			HttpInfo::getIns()->requestSQLClose();
		}
		else if (name.compare("ScrollView_1") == 0){
			ColumnListItemLayer *p = (ColumnListItemLayer *)GameDataSet::isTouchInChild(m_pScrollView1, 10, NULL);
			if (p){
				p->TouchEvent();
			}
		}
		else if (name.compare("excute_btn") == 0){
			YMSocketData sd;
			char buff[30];
			m_curCoNames.clear();
			for (int i = 0; i < m_lastConames.size(); i++){
				sprintf(buff, "d1bg_%d", i);
				Layout *ly = GameDataSet::getLayout(m_RootLayer, buff);
				ly->setVisible(true);
				Layout *ly1 = GameDataSet::getLayout(ly, "d1");
				string coname = ((Text *)ly1)->getStringValue();
				string covalue = m_pEditBoxs.at(i)->getText();
				m_curCoNames.insert(make_pair(coname, covalue));
			}
			map<string, string>repairs;
			map<string, string>::iterator itr=m_lastConames.begin();
			for (itr; itr != m_lastConames.end();itr++){
				string coname = itr->first;
				string covalue = itr->second;
				if (m_curCoNames.find(coname) != m_curCoNames.end()){
					string curvalue = m_curCoNames.at(coname);
					if (covalue.compare(curvalue) != 0){
						repairs.insert(make_pair(coname, curvalue));
					}
				}
			}
			
			HttpInfo::getIns()->requestSQLExcute(HttpInfo::getIns()->getTableName(), repairs, m_curkey, m_curkeyValue);
		}
		else if (name.compare("findbtn") == 0){
			HttpInfo *p = HttpInfo::getIns();
			string value = m_pFindEditBox->getText();
			if (value.empty()){
				ShowPrintf("查询的值为空");
			}
			else{
				p->requestSQLFindConnect(p->getTableName(), m_curconame, value);
			}

			Text *t = (Text *)GameDataSet::getLayout(m_RootLayer, "dir_label");
			m_curkeyValue = m_pFindEditBox->getText();
			m_curkey = t->getStringValue();
		}
		else if (name.compare("dbbtn") == 0){
			DatabasesLayer *p = ServerControl::getIns()->getDatabasesLayer();
			if (!p){
				p = DatabasesLayer::create();
				this->addChild(p);
			}
		}
		else if (name.compare("tablebtn") == 0){
			TablesLayer *p = ServerControl::getIns()->getTablesLayer();
			if (!p){
				p = TablesLayer::create();
				this->addChild(p);
			}
		}
		else if (name.compare("select_bg") == 0){
			Layout *ly = GameDataSet::getLayout(m_RootLayer, "listbg");
			if (!ly->isVisible()){
				ShowColumnList(true);
			}
			else{
				ShowColumnList(false);
			}
		}
	}
}

void LogoLayer::ShowPrintf(string str){
	LogoLayer *p = ServerControl::getIns()->getLogoLayer();
	LabelTTF *t = LabelTTF::create();
	char buff[1024];
	sprintf(buff, "%d)%s           %s", m_jilucount+1, XXIconv::GBK2UTF(str.c_str()).c_str(), GameDataSet::getLocalTime().c_str());
	t->setString(buff);
	t->setFontSize(20);
	t->setColor(ccc3(12,12,12));
	t->setAnchorPoint(ccp(0, 0));
	t->setVerticalAlignment(TextVAlignment::CENTER);
	t->setHorizontalAlignment(TextHAlignment::LEFT);
	t->setDimensions(Size(1000,0));
	GameDataSet::PushScrollItem(p->m_sbg, 0, 0, t,m_jilucount,p->m_pScrollView);
	p->m_pScrollView->scrollToBottom(0.02,true);
	m_jilucount++;
}

void LogoLayer::ButtonStatus(Button *btn, bool isactive){
	Color3B c3 = isactive ? ccc3(255,255,255) : ccc3(120,120,120);
	btn->setColor(c3);
	btn->setTouchEnabled(isactive);
}

void LogoLayer::StartStatus(bool isstatus){
	ButtonStatus(m_startbtn,isstatus);
}

void LogoLayer::CloseStatus(bool isstatus){
	ButtonStatus(m_closebtn, isstatus);
}

void LogoLayer::ExcuteStatus(bool isstatus){
	ButtonStatus(m_excutebtn, isstatus);
}

void LogoLayer::DBStatus(bool isstatus){
	ButtonStatus(m_changedbbtn, isstatus);
}

void LogoLayer::TableStatus(bool isstatus){
	ButtonStatus(m_changetablebtn, isstatus);
}

void LogoLayer::ShowDatabase(string db){
	GameDataSet::setText(m_RootLayer,"sjk_label",XXIconv::GBK2UTF("当前数据库:")+db);
}

void LogoLayer::ShowTable(string table){
	GameDataSet::setText(m_RootLayer, "table_label", XXIconv::GBK2UTF("当前表:") + table);
}

void LogoLayer::ShowFindBar(bool isshow){
	Layout *ly = GameDataSet::getLayout(m_RootLayer,"find_bar");
	ly->setVisible(isshow);
}
void LogoLayer::ShowFindBg(bool isshow){
	Layout *ly = GameDataSet::getLayout(m_RootLayer, "editbg");
	ly->setVisible(isshow);
	if (!isshow){
		HideDataItems();
		ShowColumnList(false);
		ExcuteStatus(false);
	}
}

void LogoLayer::ShowDataItems(map<string, string> datas,string prikey){
	ShowFindBg(true);
	char buff[30];
	int i = 0;
	m_lastConames = datas;
	map<string, string>::iterator itr = datas.begin();
	for (itr; itr != datas.end();itr++){
		sprintf(buff, "d1bg_%d", i);
		Layout *ly = GameDataSet::getLayout(m_RootLayer, buff);
		ly->setVisible(true);
		GameDataSet::setText(ly, "d1",itr->first);
		m_pEditBoxs.at(i)->setText(itr->second.c_str());
		if (itr->first.compare(prikey)==0){
			m_pEditBoxs.at(i)->setTouchEnabled(false);
			m_mengly[i]->setVisible(true);
		}
		else{
			m_mengly[i]->setVisible(false);
		}
		i++;
	}
}

void LogoLayer::HideDataItems(){
	char buff[30];
	for (int i = 0; i < 18;i++){
		sprintf(buff,"d1bg_%d",i);
		Layout *ly = GameDataSet::getLayout(m_RootLayer,buff);

		ly->setVisible(false);
	}
}

void LogoLayer::setColumns(vector<string> vec){
	m_sbg1->removeAllChildrenWithCleanup(true);
	m_pScrollView1->setInnerContainerSize(m_pScrollView1->getContentSize());
	m_sbg1->setPosition(ccp(0,0));
	int sz = vec.size();
	for (int i = 0; i < sz;i++){
		ColumnListItemLayer *p = ColumnListItemLayer::create(vec.at(i));
		GameDataSet::PushScrollItem(m_sbg1, 0, 0, p, i, m_pScrollView1);
		if (i == 0){
			setCurColumnName(vec.at(i));
		}
	}
}

void LogoLayer::setCurColumnName(string coname){
	m_curconame = coname;
	GameDataSet::setText(m_RootLayer, "dir_label", coname);
}

void LogoLayer::setCurColumnValue(string covalue){
	m_pFindEditBox->setText(covalue.c_str());
}

void LogoLayer::ShowColumnList(bool isshow){
	Layout *ly = GameDataSet::getLayout(m_RootLayer,"listbg");
	ly->setVisible(isshow);
}

void LogoLayer::ConnectOprate(bool iscan){
	char buff[30];
	for (int i = 0; i < 7; i++){
		sprintf(buff,"q%d",i+1);
		Layout *ly = GameDataSet::getLayout(m_RootLayer,buff);
		ly->setVisible(iscan);
	}
}