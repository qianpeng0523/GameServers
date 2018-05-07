#pragma once
#include "stdafx.h"
#include <signal.h>
#include <WinSock2.h>
#include "EventDispatcher.h"

#include "CSVDataInfo.h"
#include "CLibEventData.h"

class LibEvent :public Object
{
public:
	LibEvent();
	~LibEvent();

	static LibEvent *getIns();
private:
	//当前服务器对象
	Server m_Server;
public:
	bool StartServer(int port, short workernum, unsigned int connnum, int read_timeout, int write_timeout);
	void StopServer();

	void SendData(int cmd, const google::protobuf::Message *msg, int fd);
		
	static void DoAccept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data);
	Server getServer(){
		return m_Server;
	}
	ClientData * getClientData1(string servername);
	ClientData * getClientData(int fd);
	void eraseClientData(int fd);
private:
	static void DoError(struct bufferevent *bev, short error, void *ctx);
	void CloseConn(Conn *pConn, int nFunID);
	void CloseConn(Conn *pConn);
	static void DoRead(struct bufferevent *bev, void *ctx);
	static void DoWrite(Conn *pconn);
	static DWORD WINAPI ThreadServer(LPVOID lPVOID);
	static DWORD WINAPI ThreadWorkers(LPVOID lPVOID);
	static DWORD WINAPI ThreadHttp(LPVOID lPVOID);
	static DWORD WINAPI ThreadAI(LPVOID lPVOID);
	string getReq(Head *h);
	int getCMD(Head *h);
	int getBodyLen(Head *h);
	int getStamp(Head *h);
	
	void inserClientData(int fd,ClientData *data);
	
	ClientData * getClientData(string sessionid);
	
	
	void eraseClientData(string seesionid);
	void resetConn(Conn *pConn);
private:
	map<int ,ClientData *>m_ClientDatas;
	int m_stamp;
	static LibEvent *m_ins;
};