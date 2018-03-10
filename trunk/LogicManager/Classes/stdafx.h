// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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
#include "YMSocketData.h"
#include "ccEvent.h"
#include "LogicServer.h"
using namespace std;
using namespace protocol;


enum ERR_CODE{
	ERR_OK,//����

	UNKOWN,//δ֪����

	LOGIN_PASS_ERR=100,//�˻����������
	LOGIN_TOKEN=101,//token��ʧЧ

	ROOM_OVER=201,//��������
	ROOM_AREDY_IN ,//�Ѿ��ڷ���
	ROOM_NOT_IN,//���ڷ���
	ROOM_NOT_EXIT,//����Ų�����
	ROOM_HAVE_OVER,//������������
	
	OPERATE_NOT_DISCARD=301,//���������
	OPERATE_NOT_OP,//��û���� �� ����Щ����

	REQUST_NOT_EXIT=401,//��������ݸ�ʽ�д�

	CARD_NOT_ENOUCH=501,//��������
};


static int SERVER_PORT = 6000;
static string SERVER_CODE = "lm1";
#define HEADLEN 10
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"./Debug/libiconv.lib") 
#pragma comment(lib,"./Debug/libprotobuf.lib")
#pragma comment(lib,"./Debug/libMessagePack.lib")
#endif

#define CC_HTTPFUNC_SELECTOR(_SELECTOR) static_cast<Http_CallFun>(&_SELECTOR)
#define http_handler(_SELECTOR) CC_HTTPFUNC_SELECTOR(_SELECTOR)
typedef void (Object::*Http_CallFun)(YMSocketData );

#define sqlhttp "http://192.168.1.103:8080/"
#define REPLACESTRR "||||&***#@"
#define REPLACESTRN "%^&*():"
#define LOGIC_TOKEN "kashdkiuySKJHFKS312skfhksuiie2342"