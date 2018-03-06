#include "CursorTextField.h"

static int _calcCharCount(const char * pszText)
{
	int n = 0;
	char ch = 0;
	while ((ch = *pszText))
	{
		CC_BREAK_IF(!ch);

		if (0x80 != (0xC0 & ch))
		{
			++n;
		}
		++pszText;
	}
	return n;
}

const static float DELTA = 20.0f;

XXTextFieldTTF::XXTextFieldTTF()
{
	CCTextFieldTTF();

	m_pCursorSprite = NULL;
	m_pCursorAction = NULL;
	m_inputtype = INPUT_ALL;
	isPsw = false;
	m_limitNum = 200;
	m_pCursorTextField = NULL;
	m_totalnum = 0;
// 	m_CNSZ = Utf82Unicode((char *)XXIconv::GBK2UTF("中").c_str());
// 	m_EGSZ = Utf82Unicode("a");
}

XXTextFieldTTF::~XXTextFieldTTF()
{
	
}

void XXTextFieldTTF::onEnter()
{
	CCTextFieldTTF::onEnter();
	//CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
	
	this->setDelegate(this);
}
//静态生成函数
XXTextFieldTTF * XXTextFieldTTF::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize, CursorTextField *pCursorTextField)
{
	XXTextFieldTTF *pRet = new XXTextFieldTTF();

	if (pRet && pRet->initWithPlaceHolder(placeholder, fontName, fontSize))
	{
		pRet->autorelease();
		pRet->m_pCursorTextField = pCursorTextField;
		pRet->initCursorSprite(fontSize);
		pRet->setColor(ccc3(0, 0, 0));
		return pRet;
	}

	CC_SAFE_DELETE(pRet);

	return NULL;
}

void XXTextFieldTTF::initCursorSprite(const int nHeight)
{
	// 初始化光标
// 	const int column = 4;
// 	int *pixels = new int[column*nHeight];
// 	for (int i = 0; i < nHeight; ++i) {
// 		for (int j = 0; j < column; ++j) {
// 			pixels[i*4+j] = 0xffff0000;
// 		}
// 	}
// 
// 	CCTexture2D *texture = new CCTexture2D();
// 	texture->initWithData(pixels, column*nHeight, kCCTexture2DPixelFormat_RGB888, 1, 1, SizeMake(column, nHeight));

	m_pCursorSprite = Sprite::create("cursor.png");
	Size sz = m_pCursorSprite->getContentSize();
	m_pCursorSprite->setScaleX(10/sz.width);
	m_pCursorSprite->setScaleY(nHeight / sz.height);
	Size winSize = getContentSize();
	m_cursorPos = ccp(0, winSize.height / 2);
	m_pCursorSprite->setPosition(m_cursorPos);
	this->addChild(m_pCursorSprite);
	m_pCursorSprite->setName("cursor");

	m_pCursorAction = CCRepeatForever::create((CCActionInterval *)CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));

	m_pCursorSprite->runAction(m_pCursorAction);
	m_pCursorSprite->setVisible(false);

}

// bool XXTextFieldTTF::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
// {
// 	m_beginPos = pTouch->getLocationInView();
// 	m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
// 
// 	return true;
// }

CCRect XXTextFieldTTF::getRect()
{
	Size size;
	if (&m_designedSize != NULL) {
		size = m_designedSize;
	}
	else {
		size = getContentSize();
	}

	CCRect rect = CCRectMake(0 - size.width * getAnchorPoint().x, 0 - size.height * getAnchorPoint().y, size.width, size.height);
	return  rect;
}
//设置触摸弹出输入法的区域大小
void XXTextFieldTTF::setDesignedSize(cocos2d::Size size)
{
	m_designedSize = size;
}

Size XXTextFieldTTF::getDesignedSize()
{
	return m_designedSize;
}

bool XXTextFieldTTF::isInTextField(cocos2d::CCTouch *pTouch)
{
	Point pToushPos = convertTouchToNodeSpaceAR(pTouch);
	return getRect().containsPoint(pToushPos);
}

// void XXTextFieldTTF::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
// {
// 	Point endPos = pTouch->getLocationInView();
// 	endPos = CCDirector::sharedDirector()->convertToGL(endPos);
// 
// 	// 判断是否为点击事件
// 	if (::abs(endPos.x - m_beginPos.x) > DELTA ||
// 		::abs(endPos.y - m_beginPos.y) > DELTA)
// 	{
// 		// 不是点击事件
// 		m_beginPos.x = m_beginPos.y = -1;
// 
// 		return;
// 	}
// 
// 	CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);
// 
// 	// 判断是打开输入法还是关闭输入法
// 	isInTextField(pTouch) ? openIME() : closeIME();
// }

bool XXTextFieldTTF::onTextFieldAttachWithIME(cocos2d::TextFieldTTF *pSender)
{
	if (_inputText.empty()) {
		return false;
	}

	m_pCursorSprite->setPositionX(getContentSize().width);

	return false;
}

bool XXTextFieldTTF::isNumber(char c, int nLen){
	if (nLen > m_EGSZ){
		return false;
	}
	else{
		if (c<'0' || c>'9'){
			if (m_pCursorTextField){
				m_pCursorTextField->setTip(ERR_NUMBER);
			}
			return false;
		}
	}
	return true;
}

bool XXTextFieldTTF::isZiMu(char c, int nLen){
	if (nLen > m_EGSZ){
		return false;
	}
	else{
		if (c<'A' || c>'z'){
			if (m_pCursorTextField){
				m_pCursorTextField->setTip(ERR_NO_CHINESE);
			}
			return false;
		}
	}
	return true;
}

void XXTextFieldTTF::insertText(const char * text, size_t len){
	std::string insert(text, len);

	// insert \n means input end
	int pos = static_cast<int>(insert.find('\n'));
	if ((int)insert.npos != pos)
	{
		log("%s",XXIconv::GBK2UTF("删除换行符！！").c_str());
		len = pos;
		insert.erase(pos);
	}

	if (len > 0)
	{
		if (onTextFieldInsertText(this, insert.c_str(), len))
		{
			// delegate doesn't want to insert text
			return;
		}

		_charCount += _calcCharCount(insert.c_str());
		std::string sText(_inputText);
		sText.append(insert);
		CCTextFieldTTF::setString(sText);
	}

	if ((int)insert.npos == pos) {
		return;
	}

	// '\n' inserted, let delegate process first
	if (onTextFieldInsertText(this, "\n", 1))
	{
		return;
	}

	// if delegate hasn't processed, detach from IME by default
	detachWithIME();
}

void XXTextFieldTTF::deleteBackward(){
	size_t len = _inputText.length();
	if (!len)
	{
		// there is no string
		return;
	}

	// get the delete byte number
	size_t deleteLen = 1;    // default, erase 1 byte

	while (0x80 == (0xC0 & _inputText.at(len - deleteLen)))
	{
		++deleteLen;
	}

	if (onTextFieldDeleteBackward(this, _inputText.c_str() + len - deleteLen, static_cast<int>(deleteLen)))
	{
		// delegate doesn't want to delete backwards
		return;
	}

	// if all text deleted, show placeholder string
	if (len <= deleteLen)
	{
		_inputText = "";
		_charCount = 0;
		setTextColor(_colorSpaceHolder);
		setString(_placeHolder.c_str(), _placeHolder.c_str());
		return;
	}

	// set new input text
	std::string text(_inputText.c_str(), len - deleteLen);
	CCTextFieldTTF::setString(text);
}

bool XXTextFieldTTF::onTextFieldInsertText(cocos2d::TextFieldTTF *pSender, const char *text, int nLen)
{
	m_pCursorTextField->setTip(ERR_NONE);
	CCLOG("Width: %g", pSender->getContentSize().width);
	CCLOG("Text: %s", text);
	CCLOG("Length: %d", nLen);
	
	
	CCLOG("puttext:%s", _inputText.c_str());
	//deleteDirtyWord();
	char* tt = Utf82Unicode((char*)text, nLen);
	
	std::string tempStr = _inputText.substr();
	tempStr.append(tt);
	string tp = tt;
	getUtf8Num(tempStr.c_str());
	if (/*tempStr.size()*/m_totalnum > m_limitNum) {
// 		tempStr = tempStr.substr(0, m_totalsize);
// 		tp = tempStr.substr(0, tempStr.find(_inputText.c_str()) + _inputText.length());
		_inputText = m_fullstr;
		if (m_pCursorTextField){
			m_pCursorTextField->setTip(ERR_LONG);
		}
		//return true;
	}
	else{
		_inputText.append(tp.c_str());
	}


	if (isPsw) {
		std::string tempStr="";
		for (int i = 0; i < _inputText.size(); i++) {
			tempStr.append("*");
		}
		Label::setString(tempStr.c_str());
	}
	else {
		Label::setString(_inputText.c_str());
	}
	deleteDirtyWord();
	Size sszz = getContentSize();
	if (_inputText.empty()){
		m_pCursorSprite->setPositionX(0);
	}
	else{
		m_pCursorSprite->setPositionX(sszz.width);
	}

	if (strcmp(tt, "") != 0){
		CC_SAFE_DELETE(tt);
	}
	
	Label::setColor(ccc3(0,0,0));
	return true;
}

void XXTextFieldTTF::deleteDirtyWord(){
	
}

bool XXTextFieldTTF::onTextFieldDeleteBackward(cocos2d::TextFieldTTF *pSender, const char *delText, int nLen)
{
	m_pCursorTextField->setTip(ERR_NONE);
	int sz = _inputText.size() - nLen;
	if (sz < 0){
		return 0;
	}
	_inputText.resize(_inputText.size() - nLen);
	//CCLog(_inputText.c_str());
	//deleteDirtyWord();
	if (isPsw) {
		std::string tempStr="";
		for (int i = 0; i < _inputText.size(); i++) {
			tempStr.append("*");
		}
		setString(tempStr.c_str(), _inputText.c_str());
	}
	else {
		std::string ptext = _inputText;
		setString(ptext.c_str(), ptext.c_str());
	}
	if (_inputText.empty()){
		m_pCursorSprite->setPositionX(0);
	}
	else{
		m_pCursorSprite->setPositionX(getContentSize().width);
	}

	if (_inputText.empty()) {
		m_pCursorSprite->setPositionX(0);
	}

	return true;
}

bool XXTextFieldTTF::onTextFieldDetachWithIME(cocos2d::TextFieldTTF *pSender)
{
	return false;
}

void XXTextFieldTTF::openIME()
{
	m_pCursorSprite->setVisible(true);
	this->attachWithIME();
}

void XXTextFieldTTF::closeIME()
{
	m_pCursorSprite->setVisible(false);
	this->detachWithIME();
}

void XXTextFieldTTF::onExit()
{
	this->detachWithIME();
	CCTextFieldTTF::onExit();
	
}

bool XXTextFieldTTF::getIsPsw()
{
	return isPsw;
}
//设置星号显示否
void XXTextFieldTTF::setIsPsw(bool bFlag)
{
	isPsw = bFlag;
}

int XXTextFieldTTF::getLimitNum()
{
	return m_limitNum;
}
//设置字符长度
void XXTextFieldTTF::setLimitNum(int limitNum)
{
	m_limitNum = limitNum;
}

void XXTextFieldTTF::setLimitSmallNum(int smallnum){
	m_limitsmallnum = smallnum;
}
int XXTextFieldTTF::getLimitSmallNum(){
	return m_limitsmallnum;
}

void XXTextFieldTTF::setString(const char *displayTx, const char* inputTx)
{
	//CC_SAFE_DELETE(m_pInputText);

	

	if (inputTx)
	{
		_inputText = inputTx;
	}
	else
	{
		_inputText = "";
	}
	
	string str[3] = {"\n","\r","\r\n"};
	for (int i = 0; i < 3; i++){
		int index = _inputText.find(str[i]);
		if (index != -1){
			int len = str[i].length();
			_inputText.replace(index, index + len, "");
		}
	}
	//deleteDirtyWord();
	// if there is no input text, display placeholder instead
	if (isPsw){
		std::string tempStr="";
		for (int i = 0; i < _inputText.size(); i++) {
			tempStr.append("*");
		}
		
		if ((_inputText).empty())
		{
			Label::setString(_placeHolder.c_str());
		}
		else
		{
			Label::setString(tempStr.c_str());
		}
	}
	else{
		if ((_inputText).empty())
		{
			Label::setString(_placeHolder.c_str());
		}
		else
		{
			
			Label::setString(_inputText.c_str());
		}
	}
	_charCount = _calcCharCount(_inputText.c_str());

	if (_inputText.empty()){
		m_pCursorSprite->setPositionX(0);
	}
	else{
		m_pCursorSprite->setPositionX(getContentSize().width);
	}

	if (_inputText.empty()) {
		m_pCursorSprite->setPositionX(0);
	}
}

void XXTextFieldTTF::setColor(const ccColor3B& color3)
{
	// 	m_sColor = m_sColorUnmodified = color3;
	// 
	// 	if (m_bOpacityModifyRGB)
	// 	{
	// 		m_sColor.r = color3.r * m_nOpacity / 255.0f;
	// 		m_sColor.g = color3.g * m_nOpacity / 255.0f;
	// 		m_sColor.b = color3.b * m_nOpacity / 255.0f;
	// 	}
	Label::setColor(color3);
	CCTextFieldTTF::setColor(color3);
	//updateColor();
	m_pCursorSprite->setColor(color3);
}

char*  XXTextFieldTTF::Utf82Unicode(char* in, int insize)
{
	//-------------------------------------------------------------------------------------------  
	//参数有效性判断  
	
	if (in == NULL || insize<0)
	{
		return "";
	}
	m_fullstr = "";
	char *temp = new char[insize * 3];
	memset(temp, 0, insize);
	int typeCount = 0;
	int totalNum = 0;
	char *p = in;
	int index = 0;
	int totalsize = 0;
	for (int i = 0; i<insize; i++)
	{
		
		if (*p >= 0x00 && *p <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！  
		{
			char c = p[0];
			if (m_inputtype == INPUT_NUM || m_inputtype == INPUT_TEL){
				if (c >= '0'&&c <= '9'){
					temp[index++] = c;
					totalsize++;
				}
				else{
					if (m_pCursorTextField){
						m_pCursorTextField->setTip(ERR_NUMBER);
					}
				}
			}
			else if (m_inputtype == INPUT_ZIMU){
				if (c >= 'A'&&c <= 'z'){
					temp[index++] = c;
					totalsize++;
				}
				else{
					if (m_pCursorTextField){
						m_pCursorTextField->setTip(ERR_NO_CHINESE);
					}
				}
			}
			else{
				temp[index++] = c;
				totalsize++;
			}
			
			p++;
			totalNum += 1;
			
		}
		else if ((*p & (0xe0)) == 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！  
		{
			bool isT = false;
			if (m_inputtype == INPUT_ALL){
				temp[index++] = p[0];
				totalsize++;
			}
			else{
				isT = true;
			}
			
			p++;
			if (m_inputtype == INPUT_ALL){
				temp[index++] = p[0];
				totalsize++;
			}
			else{
				isT = true;
			}
			
			p++;
			totalNum += 1;
			if (isT&&m_pCursorTextField){
				m_pCursorTextField->setTip(ERR_NO_CHINESE);
			}
		}
		else if ((*p & (0xf0)) == 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！  
		{
			bool isT = false;
			if (m_inputtype == INPUT_ALL){
				temp[index++] = p[0];
				totalsize++;
			}
			else{
				isT = true;
			}
			
			p++;
			if (m_inputtype == INPUT_ALL){
				temp[index++] = p[0];
				totalsize++;
			}
			else{
				isT = true;
			}
			
			p++;
			if (m_inputtype == INPUT_ALL){
				temp[index++] = p[0];
				totalsize++;
			}
			else{
				isT = true;
			}
			
			p++;
			totalNum += 1;

			if (isT&&m_pCursorTextField){
				m_pCursorTextField->setTip(ERR_NO_CHINESE);
			}
		}
		
	}
	m_totalsize = totalsize;
	m_totalnum = totalNum;
	temp[index] = '\0';
	if (temp[0]=='\0'){
		CC_SAFE_DELETE(temp);
		return "";
	}

	return temp;
}

void XXTextFieldTTF::getUtf8Num(string in){
	if (in.empty())
	{
		return ;
	}
	int len = in.length();
	int totalNum = 0;
	char *p = (char*)in.c_str();
	int totalsize = 0;
	char buff[10];
	for (int i = 0; i < len; )
	{
		char zifu[4] = {0};
		if (*p >= 0x00 && *p <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！  
		{
			totalsize++;
			zifu[0] = p[0];
			p++;
			totalNum += 1;
			i++;
			zifu[1] = '\0';
		}
		else if ((*p & (0xe0)) == 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！  
		{
			
			totalsize++;
			zifu[0] = p[0];
			p++;
			totalsize++;
			zifu[1] = p[0];
			p++;
			totalNum += 1;
			i+=2;
			zifu[2] = '\0';
			
		}
		else if ((*p & (0xf0)) == 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！  
		{
			totalsize++;
			zifu[0] = p[0];
			p++;
			totalsize++;
			zifu[1] = p[0];
			p++;
			totalsize++;
			zifu[2] = p[0];
			p++;
			totalNum += 2;
			i += 3;
			zifu[3] = '\0';
		}
		
		log("zifuzifu:%s",zifu);
		if (totalNum <= m_limitNum){
			m_fullstr.append(zifu);
		}
	}
	m_totalsize = totalsize;
	m_totalnum = totalNum;
}




































CursorTextField::CursorTextField(){
	m_pXXTextFieldTTF = NULL;
	m_designSize = Size(0,0);
	m_contentsz = Size(0, 0);
	m_tip = NULL;
	m_node = NULL;
	this->scheduleUpdate();
	
	//this->setSwallowsTouches(true);
	
}

CursorTextField::~CursorTextField(){
	this->unscheduleUpdate();
	
}

void CursorTextField::onEnter(){
	CCLayer::onEnter();
	//this->setTouchEnabled(true);
}

void CursorTextField::onExit(){
	CCLayer::onExit();
	//this->setTouchEnabled(false);
}

bool CursorTextField::init(){
	if (!Layer::init()){
		return false;
	}
	
	m_layout = Layout::create();
	this->addChild(m_layout);
	m_layout->setClippingEnabled(true);
	m_layout->setName("root");
	m_layout->setTouchEnabled(true);

	Layout *bg = Layout::create();
	this->addChild(bg);
	Size winsz = CCDirector::sharedDirector()->getWinSize();
	bg->setSize(winsz * 2);
	bg->setContentSize(winsz * 2);
	bg->setName("bg");
	bg->setTouchEnabled(true);
	bg->setSwallowTouches(false);

	bg->setPosition(winsz/-1.0);

	//m_layout->setClippingType(LAYOUT_CLIPPING_SCISSOR);

// 	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
// 	auto listen = EventListenerTouchOneByOne::create();
// 	listen->onTouchBegan = CC_CALLBACK_2(CursorTextField::onTouchBegan, this);
// 	listen->onTouchEnded = CC_CALLBACK_2(CursorTextField::onTouchEnded, this);
// 
// 	eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);

	
	bg->addTouchEventListener(this, toucheventselector(CursorTextField::TouchEventCallback));
	return true;
}

CursorTextField *CursorTextField::create(const char *placeholder, const char *fontName, float fontSize, Text *tip){
	CursorTextField *pCursorTextField = new CursorTextField();
	if (pCursorTextField->init()){
		pCursorTextField->AddTextFieldTTF(placeholder, fontName, fontSize,tip);
		pCursorTextField->autorelease();
	}
	else{
		CC_SAFE_DELETE(pCursorTextField);
	}
	return pCursorTextField;
}

CursorTextField *CursorTextField::create(const char *placeholder, const char *fontName, float fontSize, Node *node){
	CursorTextField *pCursorTextField = new CursorTextField();
	if (pCursorTextField->init()){
		pCursorTextField->AddTextFieldTTF(placeholder, fontName, fontSize, node);
		pCursorTextField->autorelease();
	}
	else{
		CC_SAFE_DELETE(pCursorTextField);
	}
	return pCursorTextField;
}

void CursorTextField::AddTextFieldTTF(const char *placeholder, const char *fontName, float fontSize,Text *tip){
	if (!m_pXXTextFieldTTF){
		m_pXXTextFieldTTF = XXTextFieldTTF::textFieldWithPlaceHolder(placeholder,fontName,fontSize,this);

		//Size sz = CCDirector::sharedDirector()->getWinSize();
		//m_pXXTextFieldTTF->setPosition(sz / 2.0);
		//m_pXXTextFieldTTF->setDesignedSize(Size(100, 50));
		m_pXXTextFieldTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pXXTextFieldTTF->setAnchorPoint(ccp(0, 0.5));
		
		m_layout->addChild(m_pXXTextFieldTTF);

		if (!tip){
			m_tip = Text::create();
			m_tip->setString("");
			m_tip->setFontSize(14);
			this->addChild(m_tip);
			m_tip->setColor(ccc3(255, 0, 0));
			m_tip->setAnchorPoint(ccp(0, 0.5));
		}
		else{
			m_tip = tip;
		}
		
	}
}

void CursorTextField::AddTextFieldTTF(const char *placeholder, const char *fontName, float fontSize, Node *node){
	if (!m_pXXTextFieldTTF){
		m_pXXTextFieldTTF = XXTextFieldTTF::textFieldWithPlaceHolder(placeholder, fontName, fontSize, this);

		//Size sz = CCDirector::sharedDirector()->getWinSize();
		//m_pXXTextFieldTTF->setPosition(sz / 2.0);
		//m_pXXTextFieldTTF->setDesignedSize(Size(100, 50));
		m_pXXTextFieldTTF->setHorizontalAlignment(kCCTextAlignmentLeft);
		m_pXXTextFieldTTF->setAnchorPoint(ccp(0, 0.5));

		m_layout->addChild(m_pXXTextFieldTTF);

		m_node = node;

	}
}

void CursorTextField::setTouchInput(bool isOpen){
	if (m_layout){
		m_layout->setTouchEnabled(isOpen);
	}
}

void CursorTextField::setDesignedSize(Size dsz){
	m_designSize = dsz+Size(-10,10);
	if (!m_layout){
		return;
	}
	m_layout->setSize(dsz);
	m_layout->setContentSize(dsz);
	m_layout->addTouchEventListener(this, toucheventselector(CursorTextField::TouchEventCallback));
	m_pXXTextFieldTTF->setPositionY(m_layout->getSize().height / 2.0);
	if (m_tip->getParent() == this){
		Point pos = m_pXXTextFieldTTF->getParent()->convertToWorldSpace(m_pXXTextFieldTTF->getPosition());
		m_tip->setPosition(ccp(pos.x + m_layout->getSize().width*1.02, pos.y));
	}
}

void CursorTextField::setLinitNum(int limitnum){
	m_pXXTextFieldTTF->setLimitNum(limitnum);
}

void CursorTextField::setLimitSmallNum(int lnum){
	m_pXXTextFieldTTF->setLimitSmallNum(lnum);
}

void CursorTextField::setIsPw(bool isPw){
	m_pXXTextFieldTTF->setIsPsw(isPw);
}

void CursorTextField::openIME(){
	setTip(ERR_NONE);
	m_pXXTextFieldTTF->openIME();
	if (m_node){
		m_node->setVisible(true);
	}
	
}

void CursorTextField::closeIME(){
	m_pXXTextFieldTTF->closeIME();
}

const char* CursorTextField::getString(){
	return m_pXXTextFieldTTF->getString().c_str();
}

void CursorTextField::setString(const char* text){
	m_pXXTextFieldTTF->setString(text, text);
}

void CursorTextField::setPlaceHolder(const char* PlaceHolder){
	m_pXXTextFieldTTF->setPlaceHolder(PlaceHolder);
}

bool CursorTextField::isInTextField(CCTouch *pTouch){
	Point pToushPos = pTouch->getLocationInView();
	pToushPos = CCDirector::sharedDirector()->convertToGL(pToushPos);

	Point pp = CCDirector::sharedDirector()->convertToGL(this->convertToWorldSpace(m_layout->getPosition()));

	CCRect rect = m_layout->boundingBox();
	rect.size = m_layout->getContentSize();
	

	//CCLOG("touchpos:(%g,%g) (%g,%g,%g,%g)", pToushPos.x, pToushPos.y, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	return rect.containsPoint(pToushPos);
}

void CursorTextField::TouchEventCallback(Object *obj, TouchEventType type){
	if (type == TOUCH_EVENT_ENDED){
		Layout *lay = (Layout *)obj;
		string name = lay->getName();
		if (name.compare("root") == 0){
			openIME();
		}
		else if (name.compare("bg") == 0){
			closeIME();
		}
	}
}
// 
// bool CursorTextField::onTouchBegan(Touch *pTouch, Event *pEvent){
// 	m_beginPos = pTouch->getLocationInView();
// 	m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
// 	CCLOG("begin:%g", m_beginPos.x);
// 	return true;
// }
// 
// void CursorTextField::onTouchEnded(Touch *pTouch, Event *pEvent){
// 	Point endPos = pTouch->getLocationInView();
// 	endPos = CCDirector::sharedDirector()->convertToGL(endPos);
// 
// 	// 判断是否为点击事件
// 	if (::abs(endPos.x - m_beginPos.x) > DELTA ||
// 		::abs(endPos.y - m_beginPos.y) > DELTA)
// 	{
// 		// 不是点击事件
// 		m_beginPos.x = m_beginPos.y = -1;
// 		CCLOG("END RETURN ");
// 		return;
// 	}
// 
// 	//CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);
// 
// 	// 判断是打开输入法还是关闭输入法
// 	bool isIn = isInTextField(pTouch);
// 	CCLOG("%s", isIn ? "true" : "false");
// 	isIn ? openIME() : closeIME();
// }

void CursorTextField::setColor(ccColor3B c3){
	m_pXXTextFieldTTF->setColor(c3);
}


void CursorTextField::update(float dt){
	if (m_pXXTextFieldTTF){
		string str = m_pXXTextFieldTTF->getString();
		if (!str.empty()){
			Size sz = m_pXXTextFieldTTF->getContentSize();
			if (sz.width > m_designSize.width && (m_contentsz.width != sz.width || m_contentsz.height != sz.height)){

				m_pXXTextFieldTTF->setPositionX(-(sz.width - m_designSize.width));
				m_contentsz = sz;
			}
			else if (sz.width <= m_designSize.width){
				m_pXXTextFieldTTF->setPositionX(0);
			}
		}
		else{
			m_pXXTextFieldTTF->setPositionX(0);
		}
	}
}

void CursorTextField::setInputModel(INPUTMODEL type){
	if (m_pXXTextFieldTTF){
		m_pXXTextFieldTTF->setInputModel(type);
	}
}

void CursorTextField::setTip(ERRTYPE type){
	if (m_tip){
		char buff[300];
		switch(type){
		case ERR_LONG:
			if (m_pXXTextFieldTTF->m_inputtype == INPUT_ALL){
				sprintf(buff, XXIconv::GBK2UTF("输入的长度不能超过%d个中文，%d个字母数字").c_str(), m_pXXTextFieldTTF->getLimitNum() / 2, m_pXXTextFieldTTF->getLimitNum());
			}
			else{
				sprintf(buff, XXIconv::GBK2UTF("输入的长度已经不能超过%d个").c_str(), m_pXXTextFieldTTF->getLimitNum());
			}
			break;
		case ERR_NUMBER:
			sprintf(buff,"%s", XXIconv::GBK2UTF("只能输入数字字符").c_str());
			break;
		case ERR_EMAIL:
			sprintf(buff, "%s", XXIconv::GBK2UTF("只能输入电子邮件地址").c_str());
			break;
		case ERR_NO_CHINESE:
			sprintf(buff, "%s", XXIconv::GBK2UTF("不能输入中文字符").c_str());
			break;
		case ERR_TEL:
			sprintf(buff, "%s", XXIconv::GBK2UTF("只能输入11位数的电话号码").c_str());
			break;
		default:
			sprintf(buff,"%s","");
			break;
		}
		string temp = buff;
		
	}
}

void CursorTextField::setDemisions(Size sz){
	m_pXXTextFieldTTF->setDimensions(sz.width,sz.height);
}

void CursorTextField::setOpaci(int op){
	setOp(this,op);
}

void CursorTextField::setOp(CCNode *node, int op){
	Vector<Node *>nodes = node->getChildren();
	for (int i = 0; i < nodes.size(); i++){
		Node * tempnode = nodes.at(i);
		tempnode->setOpacity(op);
		setOp(tempnode,op);
		if (tempnode->getName().compare("cursor") == 0){
			tempnode->setScale(0.0);
		}
	}
}