#ifndef __ClientSocket_H__
#define __ClientSocket_H__

#ifdef WIN32
#include <thread>
#else
#include <pthread.h>
#endif

#include "stdafx.h"
#include "TcpSocket.h"
#include "EventDispatcher.h"

using namespace std;

class ClientSocket:public Object{
public:
    static ClientSocket *getIns();
private:
    static ClientSocket *m_ins;
private:
    ClientSocket();
    virtual ~ClientSocket();
public:
    int connect(const char* ip, unsigned short port);
    int close();
    int GetError();
	bool reConnect();
	void createTcp();
	void sendMsg(int cmd,const google::protobuf::Message* msg);
	void update(float dt);
	void setBegin(bool isbeign){
		m_isbegin = isbeign;
	}
private:
	static  void *threadHandler(void *arg);
	int chars2Int(char chars[]);
	void int2Chars(char *chars, int val, int start = 0);
	string int2String(unsigned int val);
	int Recv(char* buf, int len, int flags = 0);
	void DataIn(char* data, int size, int cmd);
	void getKey(int cmd, int stamp, int needKeylen, char buffer[]);
	string getReq(Head *h);
	int getCMD(Head *h);
	int getBodyLen(Head *h);
	int getStamp(Head *h);
public:
	bool m_isConnected;
	TcpSocket *m_tcpSocket;
	string m_ip;
	unsigned short m_port;
	int m_sendstamp;
	int m_recvstamp;
	bool m_isbegin;
};

#endif