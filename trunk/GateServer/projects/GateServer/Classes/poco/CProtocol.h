#pragma once

typedef enum HeadStx
{
	emStx=860805
};
typedef enum emConfig
{
	emMaxBuffLen=4096,
	emMaxPackageType=65535
};
//�����
typedef enum emFunID
{
	emFunNull=-1,
	emFunClosed=0,
	emFunTimeout=1,
	emFunError=2,
	emFunConnected=3,
};
typedef enum emFunReturn
{
	emFunReturClose=1,
	emFunReturnSend,
	emFunReturnRecv
};
/************************************************************************/
/* ��ͷ                                                                 */
/************************************************************************/
typedef struct _Head 
{
	char _backup[2];//����
	char _serverdest;
	char _cmd;//1��ʾ��¼   2��ʾ������Ϣ����
	char _bodylen[2];
	char _code[4];//protocol ��code
}Head,*PHead;