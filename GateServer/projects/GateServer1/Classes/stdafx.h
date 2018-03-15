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
#include "XXIconv.h"
#include "CProtocol.h"
#include "YMSocketData.h"
#include "Poco/Thread.h"
#include "LogicServer.h"
#include "ccEvent.h"
#include "Login.h"
#include "DBProto.h"
using namespace std;
using namespace protocol;
using namespace Poco;


static int SERVER_PORT = 5600;
static string SERVER_CODE = "wg1";
#define HEADLEN 10

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../../Debug/libiconv.lib") 
#pragma comment(lib,"../../Debug/libprotobuf.lib")
#pragma comment(lib,"../../Debug/libMessagePack.lib")
#pragma comment(lib,"../../Debug/libcrypto.lib")
#endif

#define sqlhttp "http://192.168.1.101:8080/"
#define LOGIC_TOKEN "kashdkiuySKJHFKS312skfhksuiie2342"
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"