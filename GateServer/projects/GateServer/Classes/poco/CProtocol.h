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
//命令号
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
/* 包头                                                                 */
/************************************************************************/
typedef struct _Head 
{
	char _backup[2];//备份
	char _serverdest;
	char _cmd;//1表示登录   2表示其他信息发送
	char _bodylen[2];
	char _code[4];//protocol 的code
}Head,*PHead;