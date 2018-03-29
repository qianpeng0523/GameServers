#include "fmacros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#include <assert.h>
#include <fcntl.h>
#include <limits.h>

#ifdef _WIN32
#include <fcntl.h>
#ifndef FD_SETSIZE
#define FD_SETSIZE 16000
#endif
#ifndef STDIN_FILENO
  #define STDIN_FILENO (_fileno(stdin))
#endif
#include <winsock2.h>
#include <windows.h>
#include "win32fixes.h"
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoull _strtoui64
#endif

#ifdef _WIN32
#include <fcntl.h>
#ifndef FD_SETSIZE
#define FD_SETSIZE 16000
#endif
#ifndef STDIN_FILENO
  #define STDIN_FILENO (_fileno(stdin))
#endif
#include <winsock2.h>
#include <windows.h>
#include "win32fixes.h"
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoull _strtoui64
#endif

#include "hiredis.h"
#include "zmalloc.h"
#include "help.h"
#include "anet.h"
#include "ae.h"


void doTest()
{

	//redis默认监听端口为6387 可以再配置文件中修改
	redisContext* c = redisConnect("127.0.0.1", 6379);
	if ( c->err)
	{
		redisFree(c);
		printf("Connect to redisServer faile\n");
		return ;
	}
	printf("Connect to redisServer Success\n");
	
	const char* command1 = "set stest1 value1";
	redisReply* r = (redisReply*)redisCommand(c, command1);
	
	if( NULL == r)
	{
		printf("Execut command1 failure\n");
		redisFree(c);
		return;
	}
	if( !(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str,"OK")==0))
	{
		printf("Failed to execute command[%s]\n",command1);
		freeReplyObject(r);
		redisFree(c);
		return;
	}	
	freeReplyObject(r);
	printf("Succeed to execute command[%s]\n", command1);
	
	const char* command2 = "strlen stest1";
	r = (redisReply*)redisCommand(c, command2);
	if ( r->type != REDIS_REPLY_INTEGER)
	{
		printf("Failed to execute command[%s]\n",command2);
		freeReplyObject(r);
		redisFree(c);
		return;
	}
	int length =  r->integer;
	freeReplyObject(r);
	printf("The length of 'stest1' is %d.\n", length);
	printf("Succeed to execute command[%s]\n", command2);
	
	
	const char* command3 = "get stest1";
	r = (redisReply*)redisCommand(c, command3);
	if ( r->type != REDIS_REPLY_STRING)
	{
		printf("Failed to execute command[%s]\n",command3);
		freeReplyObject(r);
		redisFree(c);
		return;
	}
	printf("The value of 'stest1' is %s\n", r->str);
	freeReplyObject(r);
	printf("Succeed to execute command[%s]\n", command3);
	
	const char* command4 = "get stest2";
	r = (redisReply*)redisCommand(c, command4);
	if ( r->type != REDIS_REPLY_NIL)
	{
		printf("Failed to execute command[%s]\n",command4);
		freeReplyObject(r);
		redisFree(c);
		return;
	}
	freeReplyObject(r);
	printf("Succeed to execute command[%s]\n", command4);	
	
	
	redisFree(c);
	
}


int main(int argc, char **argv) {
	printf("test ok!");
	//初始化ws2_32库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,1), &wsaData);
	doTest();
	getchar();
	return 0;
}
