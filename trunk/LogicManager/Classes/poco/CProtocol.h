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
	char _req[3];//服务器序列号
	char _stamp;
	char _bodylen[2];//长度
	char _cmd[4];//cmd
}Head,*PHead;