#ifndef __EventListen_H__
#define __EventListen_H__

#ifdef WIN32
#include <thread>
#else
#include <pthread.h>
#endif

#include <stdlib.h>
#include "EventDispatcher.h"


using namespace std;

class EventListen{
public:
    static EventListen *getIns();
private:
    static EventListen *m_ins;
private:
    EventListen();
    virtual ~EventListen();
public:
   
	void addDataPacketListener(int cmd, Object *target, EventHandler handler, string typeName = "");
	void removeDataPacketListener(int cmd, Object *target, EventHandler handler);
	void removeAllDataPacketListener();

};

#endif