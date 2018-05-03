#ifndef __EventDispatcher_SCENE_H__
#define __EventDispatcher_SCENE_H__

#include "ccEvent.h"
#include <fstream>


typedef void (Object::*EventHandler)(ccEvent*);
#define Event_Handler(_SELECTOR)(EventHandler)(&_SELECTOR)

class CallList{
public:
	int cmd;
	Object *obj;
	SERVERTYPE type;
	EventHandler handler;
};

typedef std::vector<CallList *> CallList_Vec;

class EventDispatcher : public Object
{
public:
	EventDispatcher();
	~EventDispatcher();
	
	static EventDispatcher *getIns();
	bool init();
	
	void addListener(int cmd, Object *target, EventHandler handler,SERVERTYPE type);//添加监听
	void removeListener(int cmd, Object *target, EventHandler handler, SERVERTYPE type);//移除监听
	void removeAllListener();
	void disEventDispatcher(ccEvent *event);
	
	void registerProto(int cmd, string tname, SERVERTYPE type);
	string getProtoName(int cmd, SERVERTYPE type);

private:
	void EventPathch(std::vector<ccEvent *> &ep);
private:
	static EventDispatcher* m_ins;
	map<SERVERTYPE, map<int, CallList_Vec>> m_eventLists;
	vector<ccEvent *> m_Events;
	map<SERVERTYPE, map<int, string>> m_protos;
private:

};

#endif 