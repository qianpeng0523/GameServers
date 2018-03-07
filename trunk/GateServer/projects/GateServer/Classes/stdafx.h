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
using namespace std;
using namespace protocol;
#define CC_REPEAT_FOREVER (UINT_MAX -1)
#define kRepeatForever CC_REPEAT_FOREVER
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
#define CC_DEPRECATED_ATTRIBUTE
#endif 

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam


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

#define SERVER_PORT 5600

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../../Debug/libiconv.lib") 
#pragma comment(lib,"../../Debug/libprotobuf.lib")
#pragma comment(lib,"../../Debug/libMessagePack.lib")
#endif