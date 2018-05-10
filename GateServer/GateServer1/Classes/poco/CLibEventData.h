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
#include "StatTimer.h"
#include "CProtocol.h"
#include "Common.h"
#include "LoginInfo.h"

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
		m_sendstamp = 0;
		m_recvstamp = 0;
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
	int m_sendstamp;
	int m_recvstamp;
	
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

struct _ClientData:public Object
{
	_ClientData(){
		_fd = 0;
		_conn = NULL;
		m_lasttime = 0;
		StatTimer::getIns()->scheduleSelector(this, schedule_selector(_ClientData::update), 1.0);
	}
	~_ClientData(){
		StatTimer::getIns()->unscheduleSelector(this, schedule_selector(_ClientData::update));
	}
	void update(float dt){
		time_t tt = Common::getTime();
		if (m_lasttime>0&&tt - m_lasttime > 2 * 60){
			CLog::log("lasttime[%ld]---tt[%ld]--sub[%ld]\n",m_lasttime,tt,tt-m_lasttime);
			CLog::log("%s\n","主动断开");
			LoginInfo::getIns()->eraseClientData(_fd);
		}
	}
	evutil_socket_t _fd;
	_Conn *_conn;
	string _sessionID;
	string _ip;
	string _uid;
	string _openid;
	time_t m_lasttime;
};

typedef struct _Server Server;
typedef struct _Worker Worker;
typedef struct _Conn Conn;
typedef struct _ConnList ConnList;
typedef struct _ClientData ClientData;
