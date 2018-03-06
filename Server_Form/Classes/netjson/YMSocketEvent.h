#ifndef __SOCKETYMEvent_H__
#define __SOCKETYMEvent_H__

#include "YMEvent.h"

class YMSocketEvent: public YMEvent {
public:
	/** 链接后事件，链接失败错误代码在m_errorCode **/
	const static string EVENT_SOCKET_CONNECT;
	/** 断开链接 **/
	const static string EVENT_SOCKET_CLOSE;
	/** socket错误 **/
	const static string EVENT_SOCKET_ERROR;

public:
	YMSocketEvent(string type, int errorCode);
	int getErrorCode();

private:
	int m_errorCode;
};

#endif