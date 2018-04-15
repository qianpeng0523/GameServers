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
#include "ConfigProto.h"
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
#define sqlhttp "http://lesharecs.com:8080/"
#define LOGIC_TOKEN "kashdkiuySKJHFKS312skfhksuiie2342"
#define DECKEY "FQ6M1w0GswdKkTuZWcFmM1rU3bDB/CTiw+KrONdCPOg"

#define ZERO_STR '&'
//#define FENG_LIAN 0


/*******微信支付定义********/
#define APPID "wxd5d25d59ae13ac95"
#define MCHID "1501106141"
#define INITNONCEID "18880000"
#define NOTIFYURL "http://lesharecs.com:8082"
#define TRADETYPE "APP"
#define INITNO "00000000"
#define APPSECRET "42eded380f7f410e1556a1a94cfaa096"

/***********支付宝支付**************/
#define ALIAPPID "2017112900248589"
#define ALIPRIKEY "MIIEowIBAAKCAQEAzLS8xuKhj0eWHBcyvPEGLgGRvrU8RB/G1RUqi4gBRGThyF+t42V/psKiY/J7zQrGXgzkMXXyync03PSZnEnUH8rBrkCTgEohqxhwXt7+KrI+P4SBTgBXspYz5xsLbw0wdiOOHi+RtuaCgzszSbxzx/y7vNgWTlwcMlPhlVa+RyE0P4EBOvqfRVSsX4YLbynTFImXHD40CdrorlY16yozidtOOtwZ3mrox3XBfIXwKsG/KL+lHRbUHnBVCYI67SBicPGjmZ24ttrP595Dc5A6hGGXkV4YBm+D2qWMOa38y3UcMXbFBr4fZeGypUlr1sWWUVipy9K9rt5BkqEa5m0C2QIDAQABAoIBAGzUYdByyGW+L0X751Of6CMpXmy/Oa0WH8VFHXtU8SeFYqv+ghn06iqkK2VrkvntkoyJ2f+Zr1yIh6g4ujnm3kmQp+uWFVUNFF07/vHRJ9tXK9TlA0GO2RDu3eUt8aQ2pN8KBBJ0Q8mzPwu5u4imEf/uk3jQgPtj6PGwuquElGCHiRmPElXnPFNdTHkGxrka1BhU/XAOrBy6lkvhTqd1CTneFuYbRCT23B0OZmiGdWllI2ZmUeSzpt4AqZQgJHrqdU4PyEoj0SDepvQqGRO1upvce4uZkEGoVH0bmvvxCFt33rjcYtMfF+HO4hYUAeACCMYzzANDmdDDLYmwBrHnYEkCgYEA6zlcZFBA/kgJKyqI+hR4lbhSLF6YGHDJ6PVIBVpNqDm2yfiVFjRwaRvABc2dMulzbowsyKqai0RAuYOlsCUlzHWKMYZJ5OecR52YO0SIFOJJvYrEbGWLbw6V8bL4M0lcF38G8isfYiHKfVpcE6jkFNH9Em9z/OZ/x/ygU2gz7+cCgYEA3slVj/4vL7s6uW1ko6wewIS47MSznHZDgzHdfKLDHfJoqn5dGVKUnGojL0vFWroQi91o4ChS2R1f4iQN4hctcX2jFvPz+lWlQEdxxQPDm4xSbg/KiDRsa2X5h/wMf+XQ0iQjS330kJ3A3JZPsqQWX1SA05omfjZLWsszqjioHz8CgYBastVzQJau9avDXzVN6FLOuDHHNfpDgkn4b4uDtNw4v3hBzH3//9xQSwjxs+SnWujNeXNTNKU+ieTDI4Xhs5EtkyzkB7m6b6wM2TF3uOcX1mFEmps+iLEI84DRn4agj+rYjHxrKEGDny/qwcq7MeYwOP/x+dOpcl9XB4vyXT2ZhQKBgGxPye9ljp9QEW7RBlqkdZb3b8h8oS3yOURxW9Madm36ynQJV03WXjXlTBHUbrB7QUwearyAHjc8Fr+k87L1vbwtbrrcA8OzG9Iyadd0ywa2o+SqfLbXddaJD1XTbmhH3MwtkZNEO0CL0ZXs5fZd3BqgxLkbepdl4iDUJdweC26VAoGBANy8NcRMcZizq9sL1/rBuGvyrG6d9Fv54CE2YYK3HtjTo1rvqRxbQX0ZMDZOdeE3AoNldrtlCjaHPElykddixg3QGmQqG2i4wUYlSrhETQ/T3X6i5WPPzgbBtcwd103WOXmVBk+h/CjImO8RB/M6wKTJRZwBR04dS6G0PJcrLLg2"
#define ALIPUBKEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAlQStQeveZy9x87UEx35kZas6sew/tQw3xFtEsE9GiQAnza6GBRZElior8xL+uVTG21TfMbEW2DJDEYEB2od7qg9X4sRnp4/8gTM+eUHWwU5Ydt8Kj98EOnVGKvi/iAPsYMoyJUn98ElHO3ZcSvUMCy/hYi/XzVntP6YyCd0DhB7mtdIFQEjQt/bGWjM+vDUKFA+lAzQs1FM5+kzZM4uye93dmNsnrwD5trs24JZJVhngTPeblO8YkhAxeAVP7bnu9ge1zjLpX5WcLH4D39cXqdA0C7bz7cseXJc+KxwbBIWiWBhxKVY9prW0e+vxCLUjpXXEJhb4SpUubhaT0dFiIwIDAQAB"
#define ALIPAYURL "https://openapi.alipay.com/gateway.do"
#define CHARSET "utf-8"