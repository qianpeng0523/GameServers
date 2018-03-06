#ifndef XXTextFieldTTF_h
#define XXTextFieldTTF_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "XXIconv.h"
#include "Adaptation.h"
#include "BaseLayer.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocos2d_xx;

enum ERRTYPE{
	ERR_NONE,
	ERR_LONG,//太长，超出x长度
	ERR_NUMBER,//只能输入数字
	ERR_NO_CHINESE,//不能输入中文
	ERR_TEL,//只能输入电话号码
	ERR_EMAIL //只能输入电子邮件地址
};

enum INPUTMODEL{
	INPUT_ALL=0,
	INPUT_NUM,
	INPUT_ZIMU,
	INPUT_TEL,
	INPUT_NOT_CHINESE
};

class CursorTextField;
class XXTextFieldTTF : public TextFieldTTF
	, public TextFieldDelegate
{
private:
	// 点击开始位置
	Point m_beginPos;

	// 光标精灵
	Sprite *m_pCursorSprite;

	// 光标动画
	CCAction *m_pCursorAction;

	// 光标坐标
	Point m_cursorPos;

	// 是否加密显示
	bool isPsw;
	int m_limitNum;//最大限制
	int m_limitsmallnum;//最小限制
	// 输入框内容
	//std::string *m_pInputText;
	Size m_designedSize;
	
	int m_CNSZ;
	int m_EGSZ;
	CursorTextField *m_pCursorTextField;
	int m_totalnum;
	int m_totalsize;
	string m_fullstr;
public:
	INPUTMODEL m_inputtype;
public:
	XXTextFieldTTF();
	~XXTextFieldTTF();

	// static，暂时不能使用
	//    static CursorTextField * textFieldWithPlaceHolder(const char *placeholder, const Size& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);

	/** creates a CCLabelTTF from a fontname and font size */
	static XXTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize, CursorTextField *pCursorTextField);

	// CCLayer
	void onEnter();
	void onExit();

	// 初始化光标精灵
	void initCursorSprite(const int nHeight);
	void setColor(const ccColor3B& color3);
	// CCTextFieldDelegate
	bool onTextFieldAttachWithIME(TextFieldTTF *pSender);
	bool onTextFieldDetachWithIME(TextFieldTTF * pSender);
	bool onTextFieldInsertText(TextFieldTTF * pSender, const char * text, int nLen);
	bool onTextFieldDeleteBackward(TextFieldTTF * pSender, const char * delText, int nLen);

	virtual void insertText(const char * text, size_t len);
	virtual void deleteBackward();

	// CCLayer Touch
	//bool onTouchBegan(Touch *pTouch, Event *pEvent);
	//void onTouchEnded(Touch *pTouch, Event *pEvent);

	// 判断是否点击在TextField处
	bool isInTextField(CCTouch *pTouch);
	// 得到TextField矩形
	CCRect getRect();

	// 打开输入法
	void openIME();
	// 关闭输入法
	void closeIME();

	//设置是否星号显示
	bool getIsPsw();
	void setIsPsw(bool bFlag);
	//设置字符长度限制，一个汉字三个字符
	void setLimitNum(int limitNum);
	int getLimitNum();
	void setLimitSmallNum(int smallnum);
	int getLimitSmallNum();
	//重载原函数，用来显示星号
	void setString(const char* displayTx, const char* inputTx);
	//点击弹出输入法的尺寸范围
	void setDesignedSize(Size size);
	Size getDesignedSize();
	void setInputModel(INPUTMODEL inputtype){
		m_inputtype = inputtype;
	}
	bool isNumber(char c, int nLen);
	bool isZiMu(char c, int nLen);
	char*  Utf82Unicode(char* in, int insize);
	void deleteDirtyWord();
	void getUtf8Num(string in);
};






class CursorTextField :public BaseLayer{
public:
	CursorTextField();
	~CursorTextField();
	virtual bool init();
	static CursorTextField *create(const char *placeholder, const char *fontName, float fontSize, Text *tip=NULL);
	static CursorTextField *create(const char *placeholder, const char *fontName, float fontSize, Node *node = NULL);
	void setDesignedSize(Size dsz);
	void setLinitNum(int limitnum);
	void setLimitSmallNum(int lnum);
	void setIsPw(bool isPw);
	void openIME();
	void closeIME();
	const char* getString();
	void setString(const char* text);
	bool isInTextField(CCTouch *pTouch);
	//bool onTouchBegan(Touch *pTouch, Event *pEvent);
	//void onTouchEnded(Touch *pTouch, Event *pEvent);
	void setColor(ccColor3B c3);
	void update(float dt);
	void setInputModel(INPUTMODEL type);
	void setPlaceHolder(const char* PlaceHolder);
	virtual void onEnter();
	virtual void onExit();
	void setTip(ERRTYPE type);
	void TouchEventCallback(Object *obj, TouchEventType type);
	void setTouchInput(bool isOpen);
	void setDemisions(Size sz);
	void setOpaci(int op);
	void setOp(CCNode *node, int op);
	void setNode(Node *node){
		m_node = node;
	}
	
private:
	void AddTextFieldTTF(const char *placeholder, const char *fontName, float fontSize, Text *tip);
	void AddTextFieldTTF(const char *placeholder, const char *fontName, float fontSize, Node *node);
private:
	XXTextFieldTTF *m_pXXTextFieldTTF;
	Layout *m_layout;
	Size m_designSize;
	Point m_beginPos;
	Size m_contentsz;
	
	Text *m_tip;
	Node *m_node;
	
	
};
















#endif