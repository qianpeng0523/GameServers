// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include<algorithm>
#include<vector>
#include<iostream>
#include "DBProto.h"
#include "Object.h"
#include <map>
#include <string.h>
#include <xstring>
#include "MD5.h"
using namespace std;
using namespace protocol;
#define CC_REPEAT_FOREVER (UINT_MAX -1)
#define kRepeatForever CC_REPEAT_FOREVER
// TODO: 在此处引用程序需要的其他头文件

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
#define CC_DEPRECATED_ATTRIBUTE
#endif 

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam


enum ERR_CODE{
	ERR_OK,//正常

	UNKOWN,//未知错误

	LOGIN_PASS_ERR=100,//账户或密码错误
	LOGIN_TOKEN=101,//token已失效

	ROOM_OVER=201,//房间已满
	ROOM_AREDY_IN ,//已经在房间
	ROOM_NOT_IN,//不在房间
	ROOM_NOT_EXIT,//房间号不存在
	ROOM_HAVE_OVER,//房间人数已满
	
	OPERATE_NOT_DISCARD=301,//不该你打牌
	OPERATE_NOT_OP,//你没有碰 杠 胡这些操作

	REQUST_NOT_EXIT=401,//请求的数据格式有错

	CARD_NOT_ENOUCH=501,//房卡不足
};

#define SERVER_PORT 5600

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../../Debug/libiconv.lib") 
#pragma comment(lib,"../../Debug/libprotobuf.lib")
#pragma comment(lib,"../../Debug/libMessagePack.lib")
#endif