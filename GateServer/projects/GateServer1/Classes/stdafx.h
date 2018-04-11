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

#include "Object.h"
#include <map>
#include <set>
#include <string.h>
#include <xstring>
#include "MD5.h"
#include "XXIconv.h"
#include "CProtocol.h"
#include "YMSocketData.h"
#include "Poco/Thread.h"
#include "LogicServer.h"
#include "ccEvent.h"
#include "Login.h"

#include "Hall.h"
#include "redis.h"

using namespace std;
using namespace protocol;
using namespace Poco;

#define HEADLEN 10
#define ZHONGFABAI
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../../Debug/libiconv.lib") 
#pragma comment(lib,"../../Debug/libprotobuf.lib")
#pragma comment(lib,"../../Debug/libMessagePack.lib")
#pragma comment(lib,"../../Debug/libcrypto.lib")
#pragma comment(lib,"../../../redis/Debug/hiredis.lib")
#endif

//#define sqlhttp "http://192.168.50.151:8080/"
//#define sqlhttp "http://192.168.1.101:8080/"
#define sqlhttp "http://47.104.165.65:8080/"
#define LOGIC_TOKEN "kashdkiuySKJHFKS312skfhksuiie2342"
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"

#define ZERO_STR '&'
//#define FENG_LIAN 0


/*******微信支付定义********/
#define APPID "wxd5d25d59ae13ac95"
#define MCHID "1501106141"
#define INITNONCEID "18880000"
#define NOTIFYURL "http://27.46.6.74/8082"
#define TRADETYPE "APP"
#define INITNO "00000000"