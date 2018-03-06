#ifndef __SOCKETYMEvent_H__
#define __SOCKETYMEvent_H__

#include "YMEvent.h"

class YMSocketEvent: public YMEvent {
public:
	/** ���Ӻ��¼�������ʧ�ܴ��������m_errorCode **/
	const static string EVENT_SOCKET_CONNECT;
	/** �Ͽ����� **/
	const static string EVENT_SOCKET_CLOSE;
	/** socket���� **/
	const static string EVENT_SOCKET_ERROR;

public:
	YMSocketEvent(string type, int errorCode);
	int getErrorCode();

private:
	int m_errorCode;
};

#endif