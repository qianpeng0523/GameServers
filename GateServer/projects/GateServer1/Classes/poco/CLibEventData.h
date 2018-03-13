#pragma once
#include <event2/bufferevent.h>
#include <event2/bufferevent_compat.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer_compat.h>
#include <event2/http_struct.h>
#include <event2/bufferevent.h>
#include <event2/thread.h>

#include "CProtocol.h"

struct _Conn;
struct _Worker;

//服务器属性封装对象
struct _Server
{
	bool bStart;
	short nPort;
	short workernum;
	unsigned int connnum;
	volatile int nCurrentWorker;
	int read_timeout;
	int write_timeout;		
	struct evconnlistener *pListener;
	struct event_base *pBase;
	HANDLE hThread;
	_Worker *pWorker;		
};
//连接对象列表
struct _ConnList
{
	_ConnList()
	{
		head=NULL;
		tail=NULL;
		plistConn=NULL;
	}
	_Conn *head;
	_Conn *tail;
	_Conn *plistConn;
};
//连接对象
struct _Conn
{
	_Conn()
	{
		fd=NULL;
		bufev=NULL;
		index=-1;
		owner=NULL;
		next=NULL;
		stamp = 0;
	}
	~_Conn()
	{
		bufferevent_free(bufev);
	}
	struct bufferevent *bufev;
	evutil_socket_t fd;
	int index;
	_Worker *owner;
	_Conn *next;
	int stamp;
	
};
//工作线程封装对象.
struct _Worker
{
	_Worker()
	{
		pWokerbase=NULL;
		hThread=INVALID_HANDLE_VALUE;
		pListConn=NULL;
	}
	struct event_base *pWokerbase;
	HANDLE hThread;
	_ConnList *pListConn;
	inline _Conn* GetFreeConn()
	{
		_Conn*pItem=NULL;
		if(pListConn->head!=pListConn->tail)
		{
			pItem=pListConn->head;
			pListConn->head=pListConn->head->next;
		}
		return pItem;
	}
	inline void PutFreeConn(_Conn *pItem)
	{
		pListConn->tail->next=pItem;
		pListConn->tail=pItem;
	}
};

struct _ClientData
{
	_ClientData(){
		_fd = 0;
		_conn = NULL;
	}
	evutil_socket_t _fd;
	_Conn *_conn;
	string _sessionID;
};

typedef struct _Server Server;
typedef struct _Worker Worker;
typedef struct _Conn Conn;
typedef struct _ConnList ConnList;
typedef struct _ClientData ClientData;
