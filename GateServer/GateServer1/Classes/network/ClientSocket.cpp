#include "ClientSocket.h"
#include "LogicServerInfo.h"
#include "HttpLogic.h"

/**********消息头********
0		服务器序列号
1		stamp
2-5		长度len
6-9		cmd
**********总长度为10****/


ClientSocket *ClientSocket::m_ins=NULL;
ClientSocket *ClientSocket::getIns() {
	if(!m_ins){
		m_ins=new ClientSocket();
	}
    return m_ins;
}

ClientSocket::ClientSocket(){
	m_sendstamp = 0;
	m_recvstamp = 0;
	createTcp();
}

void ClientSocket::createTcp(){
	m_tcpSocket = new TcpSocket();
	m_isConnected = false;
	TcpSocket::Init();
	int ret = m_tcpSocket->Create(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (ret) {
		printf("Create socket:success,ret:%d\n", ret);
	} else {
		printf("Create socket:fail,ret:%d", ret);
	}
}

ClientSocket::~ClientSocket() {
	
}

string ClientSocket::int2String(unsigned int val) {
    char buff[25];
    sprintf(buff, "0x%04X", val);
    return std::string(buff);
}

int ClientSocket::chars2Int(char chars[]) {
	int iVal = 0;
	for (int i = 0; i < 4; i++)
	{
		iVal += (chars[i] & 0xFF) << ((3 - i) * 8);
	}
	return iVal;
}

void ClientSocket::int2Chars(char *chars, int val, int start) {
	chars[start++] = (char)((val >> 24) & 0xFF);
	chars[start++] = (char)((val >> 16) & 0xFF);
	chars[start++] = (char)((val >> 8) & 0xFF);
	chars[start++] = (char)((val >> 0) & 0xFF);
}

int ClientSocket::connect(const char* ip, unsigned short port) {
	m_sendstamp = 0;
	m_recvstamp = 0;
	m_ip = ip;
	m_port = port;
	int connectFlag = m_tcpSocket->Connect(ip, port);
    if (connectFlag != SOCKET_ERROR) {
		std::thread t1(&ClientSocket::threadHandler, this);//创建一个分支线程，回调到myThread函数里
		t1.detach();
        m_isConnected = true;
		LogicServerInfo::getIns()->SendCLogicLogin();
	}
    return connectFlag;
}

int ClientSocket::close() {
	int state = 0;
	m_isConnected = false;
	
	if (m_tcpSocket != NULL)
	{
		state = m_tcpSocket->Close();
		m_sendstamp = 0;
		m_recvstamp = 0;
	}
	return state;
}

int ClientSocket::Recv(char* buf, int len, int flags) {
	return m_tcpSocket->Recv(buf, len, flags);
}

int ClientSocket::GetError() {
	return m_tcpSocket->GetError();
}

void ClientSocket::sendMsg(int cmd,const google::protobuf::Message *msg){
	m_sendstamp = (m_sendstamp + 1) % MAXSTAMP;
	int len = msg->ByteSize();
	char *buffer = new char[HEADLEN + len];
	memset(buffer, 0, HEADLEN + len);

	//服务器编号
	memcpy(buffer, HttpLogic::getIns()->getServerName() .c_str(), 3);

	//消息序列号
	buffer[3] = m_sendstamp;
	//bodylen
	char * clen = (char *)&len;
	for (int i = 0; i < 2; i++){
		buffer[4 + i] = *(clen + i);
	}
	//cmd
	char *ccmd = (char *)&cmd;
	for (int i = 0; i < 4; i++){
		buffer[6 + i] = *(ccmd + i);
	}

	char* sm=new char[len];
	msg->SerializePartialToArray(sm,len);
	char *data = new char[len+1];
	HttpLogic::getIns()->aes_encrypt(sm, len, data);
	delete sm;
	for (int i = HEADLEN; i < HEADLEN + len; i++){
		buffer[i] = data[i - HEADLEN];
	}
	delete data;
	printf("sendmsg:body:%s",msg->DebugString().c_str());
	
	if (m_tcpSocket){
		m_tcpSocket->Send(buffer, HEADLEN + len);
	}
	
}


void ClientSocket::DataIn(char* data, int size,int cmd){
	//数据不能用string  只能用char*
	printf("datain size:%d cmd:%d", size, cmd);
	ccEvent *sEvent = new ccEvent(cmd, data, size,1);
	EventDispatcher::getIns()->disEventDispatcher(sEvent);
}

void *ClientSocket::threadHandler(void *arg) {
    char buff[10];
    int len;
	ClientSocket *p = ClientSocket::getIns();
    while (1) {
		len = p->Recv(buff, HEADLEN, 0);
        if (len > 0){
			Head *head = (Head*)buff;
			string servercode = p->getReq(head);
			int stamp = p->getStamp(head);
			len = p->getBodyLen(head);
			int cmd=p->getCMD(head);
			
			char *temp = new char[len];
			p->Recv(temp, len, 0);

			p->m_recvstamp = (p->m_recvstamp + 1) % MAXSTAMP;
			if (stamp == p->m_recvstamp){
				char *out = new char[len+1];
				HttpLogic::getIns()->aes_decrypt(temp, len, out);
				delete temp;
				p->DataIn(out, len, cmd);
			}
			else{
				printf("数据不合法\n");
				delete temp;
			}

        }else{
			printf("%s", "==== connect break up ====");
            //服务端断开
            p->close();
			//断开线程
            break;
        }
    }
    return 0;
}

void ClientSocket::reConnect(){
	if (m_tcpSocket){
		m_tcpSocket->Connect(m_ip.c_str(), m_port);
	}
}

void ClientSocket::update(float dt){
	
}


string ClientSocket::getReq(Head *h){
	char buff[10];
	memset(buff, 0, 10);
	memcpy(buff, h->_req, 3);
	return buff;
}

int ClientSocket::getCMD(Head *h){
	int cmd = 0;
	memcpy(&cmd, h->_cmd, 4);
	return cmd;
}

int ClientSocket::getBodyLen(Head *h){
	int len = 0;
	memcpy(&len, h->_bodylen, 2);
	return len;
}

int ClientSocket::getStamp(Head *h){
	int stamp = h->_stamp;
	return stamp;
}
