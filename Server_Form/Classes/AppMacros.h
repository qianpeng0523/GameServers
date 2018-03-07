#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"
#include "string.h"
#include "DBProto.h"
#include "YSEventDispatcher.h"

//design screen size
static cocos2d::Size designResolutionSize = cocos2d::Size(1136, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(852, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1136, 640);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1136, 640);
//Screen Size
#define SCREEN_SIZE cocos2d::CCDirector::sharedDirector()->getVisibleSize();

//Resource Scale
#define RESOURCE_SCALE	((CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width) > (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height) ?\
						 (CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width) : (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height))

#define RESOURCE_SCALEX (CCDirector::sharedDirector()->getVisibleSize().width/designResolutionSize.width)

#define RESOURCE_SCALEY (CCDirector::sharedDirector()->getVisibleSize().height/designResolutionSize.height)


#define FONTS1 "fonts/yahei.ttf"
#define  FONTS2 "fonts/STXingkai.ttf"
#define APPLE_TEST 2

#define MJ_TABLENAME_USER "userinfo"
#define MJ_TABLENAME_RECORDS "records"
#define MJ_TABLENAME_DETAIL_RECORDS "detail_records"
#define MJ_TABLENAME_NOTICE "notice"
#define MJ_TABLENAME_ROOM "room"

using namespace std;
using namespace protocol;
static string g_version = cocos2d::cocos2dVersion();


struct SQLInfo
{
	void Copy(SQLInfo p){
		_name = p._name;
		_pass = p._pass;
		_db = p._db;
		_ip = p._ip;
		_port = p._port;
		_httpip = p._httpip;
		_httpport = p._httpport;
	}
	string _name;
	string _pass;
	string _ip;
	int _port;
	string _db;
	string _httpip;
	string _httpport;
};

#endif /* __APPMACROS_H__ */
