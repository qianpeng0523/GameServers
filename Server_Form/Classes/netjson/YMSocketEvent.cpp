#include "YMSocketEvent.h"

const string YMSocketEvent::EVENT_SOCKET_CONNECT = "event_socket_connect";
const string YMSocketEvent::EVENT_SOCKET_CLOSE = "event_socket_close";
const string YMSocketEvent::EVENT_SOCKET_ERROR = "event_socket_error";

YMSocketEvent::YMSocketEvent(string type, int errorCode): YMEvent(type) {
	m_errorCode = errorCode;
}

int YMSocketEvent::getErrorCode() {
	return m_errorCode;
}