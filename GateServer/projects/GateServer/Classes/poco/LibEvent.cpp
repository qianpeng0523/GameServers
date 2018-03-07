#include "LibEvent.h"


LibEvent *LibEvent::m_ins = NULL;
LibEvent::LibEvent()
{
	
	ZeroMemory(&m_Server, sizeof(m_Server));
	WSADATA WSAData;
	WSAStartup(0x0201, &WSAData);
		
}

LibEvent::~LibEvent()
{
	WSACleanup();
}

LibEvent *LibEvent::getIns(){
	if (!m_ins){
		m_ins = new LibEvent();
	}
	return m_ins;
}

bool LibEvent::StartServer(int port, short workernum, unsigned int connnum, int read_timeout, int write_timeout)
{

	m_Server.bStart = false;
	m_Server.nCurrentWorker = 0;
	m_Server.nPort = port;
	m_Server.workernum = workernum;
	m_Server.connnum = connnum;
	m_Server.read_timeout = read_timeout;
	m_Server.write_timeout = write_timeout;
	evthread_use_windows_threads();
	m_Server.pBase = event_base_new();
	if (m_Server.pBase == NULL)
	{
		return false;
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_Server.nPort);
	m_Server.pListener = evconnlistener_new_bind(m_Server.pBase, DoAccept, (void*)&m_Server, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sin, sizeof(sin));
	if (m_Server.pListener == NULL)
	{
		return false;
	}

	m_Server.pWorker = new Worker[workernum];
	for (int i = 0; i < workernum; i++)
	{
		m_Server.pWorker[i].pWokerbase = event_base_new();
		if (m_Server.pWorker[i].pWokerbase == NULL)
		{
			delete[]m_Server.pWorker;
			return false;
		}
		//初始化连接对象
		{
			m_Server.pWorker[i].pListConn = new ConnList();
			if (m_Server.pWorker[i].pListConn == NULL)
			{
				return false;
			}
			m_Server.pWorker[i].pListConn->plistConn = new Conn[m_Server.connnum + 1];
			m_Server.pWorker[i].pListConn->head = &m_Server.pWorker[i].pListConn->plistConn[0];
			m_Server.pWorker[i].pListConn->tail = &m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum];
			for (int j = 0; j < m_Server.connnum; j++) {
				m_Server.pWorker[i].pListConn->plistConn[j].index = j;
				m_Server.pWorker[i].pListConn->plistConn[j].next = &m_Server.pWorker[i].pListConn->plistConn[j + 1];
			}
			m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum].index = m_Server.connnum;
			m_Server.pWorker[i].pListConn->plistConn[m_Server.connnum].next = NULL;
			//设置当前事件
			Conn *p = m_Server.pWorker[i].pListConn->head;
			while (p != NULL)
			{
				p->bufev = bufferevent_socket_new(m_Server.pWorker[i].pWokerbase, -1, BEV_OPT_CLOSE_ON_FREE);
				if (p->bufev == NULL)
				{
					return false;
				}
				bufferevent_setcb(p->bufev, DoRead, NULL, DoError, p);
				bufferevent_setwatermark(p->bufev, EV_READ, 0, emMaxBuffLen);
				bufferevent_enable(p->bufev, EV_READ | EV_WRITE);
				struct timeval delayWriteTimeout;
				delayWriteTimeout.tv_sec = m_Server.write_timeout;
				delayWriteTimeout.tv_usec = 0;
				struct timeval delayReadTimeout;
				delayReadTimeout.tv_sec = m_Server.read_timeout;
				delayReadTimeout.tv_usec = 0;
				//bufferevent_set_timeouts(p->bufev, &delayReadTimeout, &delayWriteTimeout);
				p->owner = &m_Server.pWorker[i];
				p = p->next;
			}
		}
		m_Server.pWorker[i].hThread = CreateThread(NULL, 0, ThreadWorkers, &m_Server.pWorker[i], 0, NULL);
	}
	m_Server.hThread = CreateThread(NULL, 0, ThreadServer, &m_Server, 0, NULL);
	if (m_Server.hThread == NULL)
	{
		return false;
	}
	m_Server.bStart = true;

	CreateThread(NULL, 0, ThreadHttp, NULL, 0, NULL);
		
	return true;
}

void LibEvent::StopServer()
{
	if (m_Server.bStart)
	{
		struct timeval delay = { 2, 0 };
		event_base_loopexit(m_Server.pBase, &delay);
		WaitForSingleObject(m_Server.hThread, INFINITE);
		if (m_Server.pWorker)
		{
			for (int i = 0; i < m_Server.workernum; i++)
			{
				event_base_loopexit(m_Server.pWorker[i].pWokerbase, &delay);
				WaitForSingleObject(m_Server.pWorker[i].hThread, INFINITE);
			}
			for (int i = 0; i < m_Server.workernum; i++)
			{
				if (m_Server.pWorker[i].pListConn)
				{
					delete[]m_Server.pWorker[i].pListConn->plistConn;
					delete m_Server.pWorker[i].pListConn;
					m_Server.pWorker[i].pListConn = NULL;
				}
				event_base_free(m_Server.pWorker[i].pWokerbase);
			}
			delete[]m_Server.pWorker;
			m_Server.pWorker = NULL;
		}
		evconnlistener_free(m_Server.pListener);
		event_base_free(m_Server.pBase);
	}
	m_Server.bStart = false;
}

void LibEvent::DoRead(struct bufferevent *bev, void *ctx)
{

	char headchar[10];
	Conn *c = (Conn *)ctx;
	size_t len = bufferevent_read(bev, headchar, 10);
	if (len >= 0){
		Head *testhead = (Head*)headchar;
		int serverdest = LibEvent::getIns()->getBodyLen(testhead);
		int cmd = LibEvent::getIns()->getCMD(testhead);
		int bodylen = LibEvent::getIns()->getBodyLen(testhead);
		int code = LibEvent::getIns()->getCode(testhead);
		printf("sercverdest:%d,cmd:%d,bodylen:%d,code:%d\n", serverdest, cmd, bodylen, code);

		char *buffer = new char[bodylen];
		len = bufferevent_read(bev, buffer, bodylen);
		if (len == bodylen){
			ccEvent *cce = new ccEvent(code, buffer, bodylen, c->fd);
			EventDispatcher::getIns()->disEventDispatcher(cce);
		}
		else{
			printf("数据不合法！！！！！！！！\n");
		}
	}
	else{
		printf("请求包不合法\n");
		LibEvent::getIns()->CloseConn(c);
	}
}

void LibEvent::DoWrite(Conn *pconn){
	char msg[4096];
	size_t len = bufferevent_read(pconn->bufev, msg, sizeof(msg)-1);

	msg[len] = '\0';
	printf("send data: %s\n", msg);

	bufferevent_write(pconn->bufev, pconn->in_buf, pconn->in_buf_len);

}

void LibEvent::SendData(int code, const google::protobuf::Message *msg, string recv_type_name, int fd){
		
	Conn *pconn=NULL ;

	if (pconn){
		string sm;
		msg->SerializePartialToString(&sm);

		string ss = msg->DebugString();
		printf("SendData:%s  name:%s\n", ss.c_str(), recv_type_name.c_str());

		//delete msg;
		//msg = NULL;
		int len = sm.length();
		int headlen = sizeof(Head);
		char *buffer = new char[len + headlen];
		//备用
		buffer[0] = 1;
		buffer[1] = 1;
		//serverdest
		buffer[2] = 2;

		//cmd
		buffer[3] = 2;

		//bodylen
		int  blen = len;
		char * b = (char *)&blen;
		char ble[2] = { *(b + 1), *b };
		for (int i = 0; i < 2; i++){
			buffer[4 + i] = ble[i];
		}

		//code
		char * a = (char *)&code;
		//char bf[4] = { *(a ), *(a + 1), *(a + 2), *(a+3) };
		char bf[4] = { *(a + 3), *(a + 2), *(a + 1), *(a) };
		for (int i = 0; i < 4; i++){
			buffer[6 + i] = bf[i];
		}

		for (int i = headlen; i < len + headlen; i++){
			buffer[i] = sm[i - headlen];
		}
		// 		for (int i = 0; i < len + headlen; i++){
		// 			printf("buffer[%d]:%c\n",i,buffer[i]);
		// 		}

		pconn->in_buf = buffer;
		pconn->in_buf_len = len + headlen;

		DoWrite(pconn);
		delete buffer;
	}
}

void LibEvent::CloseConn(Conn *pConn, int nFunID)
{
	int fd = pConn->fd;
	if (fd > 0){
			
		pConn->in_buf_len = 0;
		bufferevent_disable(pConn->bufev, EV_READ | EV_WRITE);
		evutil_closesocket(fd);
		pConn->owner->PutFreeConn(pConn);
	}
}

void LibEvent::CloseConn(Conn *pConn)
{
	CloseConn(pConn, emFunClosed);
		
}

void LibEvent::DoError(struct bufferevent *bev, short error, void *ctx)
{
	Conn *c = (Conn*)ctx;
	emFunID id = emFunNull;
	if (error&EVBUFFER_TIMEOUT)
	{
		id = emFunTimeout;
	}
	else if (error&EVBUFFER_EOF)
	{
		id = emFunClosed;
	}
	else if (error&EVBUFFER_ERROR)
	{
		id = emFunError;
	}
	LibEvent::getIns()->CloseConn(c, id);
}

void LibEvent::DoAccept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	//此处为监听线程的event.不做处理.
	Server *pServer = (Server *)user_data;
	//主线程处做任务分发.
	int nCurrent = pServer->nCurrentWorker++%pServer->workernum;
	//当前线程所在ID号
	Worker &pWorker = pServer->pWorker[nCurrent];
	//通知线程开始读取数据,用于分配哪一个线程来处理此处的event事件
	Conn *pConn = pWorker.GetFreeConn();
	if (pConn == NULL)
	{
		return;
	}
	struct sockaddr_in * in = (struct sockaddr_in *)sa;
	string ip = inet_ntoa(in->sin_addr);
	printf("accept IP:%s\n", ip.c_str());
	pConn->fd = fd;



	evutil_make_socket_nonblocking(pConn->fd);
	bufferevent_setfd(pConn->bufev, pConn->fd);
	//转发发送事件
	//LibEventFunction::DispatchFunction(emFunConnected,pConn);
	bufferevent_enable(pConn->bufev, EV_READ | EV_WRITE);
}

DWORD WINAPI LibEvent::ThreadAI(LPVOID lPVOID){
		
	return GetCurrentThreadId();
}

DWORD WINAPI LibEvent::ThreadHttp(LPVOID lPVOID){
	HttpEvent::getIns()->init();

	return GetCurrentThreadId();
}

DWORD WINAPI LibEvent::ThreadServer(LPVOID lPVOID)
{
	Server * pServer = reinterpret_cast<Server *>(lPVOID);
	if (pServer == NULL)
	{
		return -1;
	}
	event_base_dispatch(pServer->pBase);
	return GetCurrentThreadId();
}

DWORD WINAPI LibEvent::ThreadWorkers(LPVOID lPVOID)
{
	Worker *pWorker = reinterpret_cast<Worker *>(lPVOID);
	if (pWorker == NULL)
	{
		return -1;
	}
	event_base_dispatch(pWorker->pWokerbase);
	return GetCurrentThreadId();
}

int LibEvent::getServerDest(Head *h){
	int sd = h->_serverdest;
	return sd;
}

int LibEvent::getCMD(Head *h){
	int cmd = h->_cmd;
	return cmd;
}

int LibEvent::getBodyLen(Head *h){
	int len = 0;
	char buf[2];
	buf[1] = h->_bodylen[0];
	buf[0] = h->_bodylen[1];
	memcpy(&len, buf, 2);
	return len;
}

int LibEvent::getCode(Head *h){
	char buf1[4];
	buf1[0] = h->_code[3];
	buf1[1] = h->_code[2];
	buf1[2] = h->_code[1];
	buf1[3] = h->_code[0];
	int code = 0;
	memcpy(&code, buf1, 4);
	return code;
}