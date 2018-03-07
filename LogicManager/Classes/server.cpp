// testlibeventSrv.cpp : 定义控制台应用程序的入口点。
//

#ifndef __SERVER_H__
#define __SERVER_H__

#include "stdafx.h"
#include <event.h>
#include <WinSock2.h>
#include <string>
#include "Schedul.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"

using namespace QPServer;
using namespace Poco;

#pragma comment(lib,"../../lib/PocoFoundationd.lib")
// #pragma comment(lib,"../../bin/PocoFoundationd.dll")
// #pragma comment(lib,"../../bin/PocoUtil.dll")
// #pragma comment(lib,"../../bin/PocoNet.dll")
// #pragma comment(lib,"../../bin/PocoNetd.dll")
// #pragma comment(lib,"../../bin/PocoUtild.dll")  

#define LINSTEN_PORT	5001
#define LISTEN_MAX		5
#define BUFFER_SIZE		1024
struct event_base *base = NULL;

struct sock_ev
{
	struct event *read_ev;
	struct event *write_ev;
	char *buf;
	sock_ev()
	{
		read_ev = NULL;
		write_ev = NULL;
		buf = new char[BUFFER_SIZE];
		memset(buf, 0, BUFFER_SIZE);
	}

	~sock_ev()
	{
		delete[] buf;
	}
};

void release_sock_cv(sock_ev *ev)
{
	if (ev->read_ev)
	{
		if( 0 != event_del(ev->read_ev))
		{
			printf("del err\n");
		}
		event_free(ev->read_ev);
		ev->read_ev = NULL;
	}
	if (ev->write_ev)
	{
		event_del(ev->write_ev);
		event_free(ev->write_ev);
		ev->write_ev = NULL;
	}
	free(ev);
}

void on_read(evutil_socket_t sock, short eventRead, void *arg)
{
	sock_ev *ev = (sock_ev*)arg;
	int nSize = recv(sock, (char*)ev->buf, BUFFER_SIZE, 0);
	if (nSize == SOCKET_ERROR )
	{
		printf("client socket closed\n");
		release_sock_cv(ev);
		closesocket(sock);
		return;
	}
	printf("%s:eventRead:%d\n", ev->buf, eventRead);
	char *p = "recv hello world!";
	//保存ip  句柄  同一个ip地址的用户怎么获取信息(通过uid来确定，因为在没有通过uid连接的时候不能发送数据,如果能发则怎么处理)
	send(sock, p, strlen(p), 0);
}

void on_write(evutil_socket_t sock, short eventWrite, void *arg)
{
	sock_ev *ev = (sock_ev*)arg;
	printf("%s\n", (char*)ev->buf);
}

void on_accept(int sock, short eventAccept, void *arg)
{	
	struct sockaddr_in cliAddr;
	int nSize = sizeof(sockaddr_in);
	SOCKET cliSock = accept(sock, (sockaddr*)&cliAddr, &nSize);
	sock_ev *ev = new sock_ev;
	std::string ip = inet_ntoa(cliAddr.sin_addr);//记录了ip
	printf("ip:%s\n", ip.c_str());
	struct event *eventRead = event_new(base, cliSock, EV_READ | EV_PERSIST, on_read, ev);
	struct event *eventWrite = event_new(base, cliSock, EV_WRITE | EV_PERSIST, on_write, ev);
	ev->read_ev = eventRead;
	ev->write_ev = eventWrite;
	event_add(eventRead, NULL);
	event_add(eventWrite, NULL);
	Schedul::getIns()->scheduleUpdate(Schedul::getIns());
	
}

int _tmain(int argc, _TCHAR* argv[])
{

	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	SOCKET sockListen = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in srvAddr;
	srvAddr.sin_addr.s_addr = INADDR_ANY;
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(LINSTEN_PORT);

	if (bind(sockListen, (sockaddr*)&srvAddr, sizeof(sockaddr)) != 0)
	{
		printf("bind err = %d\n", WSAGetLastError());
		return 0;
	}
	if(0 !=  listen(sockListen, LISTEN_MAX))
	{
		printf("listen err = %d\n", WSAGetLastError());
		return 0;
	}
	
	base = event_base_new();
	struct event *eventListen = event_new(base, sockListen, EV_READ | EV_PERSIST, on_accept, NULL);
	event_add(eventListen, NULL);
	event_base_dispatch(base);	

	closesocket(sockListen);
	event_base_free(base);
	WSACleanup();

	
	return 0;
}

#endif