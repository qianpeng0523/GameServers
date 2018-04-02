// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <stdlib.h>    
#include <string.h>     //for strcat
#include <xstring>
#include <evhttp.h>
#include <signal.h>
#include <map>
#include "XXIconv.h"
#include "YMSocketData.h"
#include "Object.h"
#include "Hall.h"
#include "Login.h"
#include "LogicServer.h"
#include "DBProto.h"

using namespace ::google::protobuf;
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


#define SQLSERVER_IP "47.104.165.65"
#define HTTP_H "http://"
#define SQL_FIND_USERINFO HTTP_H+SQLSERVER_IP

#define DBUSER "qpserver"
#define DBPASSWD "3.1415926qp"
#define DBIP "47.104.165.65"
#define DBNAME "majiang"
// #define MJ_TABLENAME_USER "userinfo"
// #define MJ_TABLENAME_RECORDS "records"
// #define MJ_TABLENAME_DETAIL_RECORDS "detail_records"
#define DBPORT 6379
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../Debug/libiconv.lib")
#pragma comment(lib,"../Debug/hiredis.lib")
#pragma comment(lib,"../Debug/libprotobuf.lib")
#endif
using namespace std;
