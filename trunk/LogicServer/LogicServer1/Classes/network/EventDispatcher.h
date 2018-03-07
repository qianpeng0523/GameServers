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
	
	void addListener(int cmd, Object *target, EventHandler handler);//添加监听
	void removeListener(int cmd, Object *target, EventHandler handler);//移除监听
	void removeAllKistener();
	void disEventDispatcher(ccEvent *event);
	
	void registerProto(int cmd,string tname);
	string getProtoName(int cmd);
private:
	void EventPathch(std::vector<ccEvent *> &ep);
private:
	static EventDispatcher* m_ins;
	std::map<int, CallList_Vec> m_eventLists;
	std::vector<ccEvent *> m_Events;
	map<int, string> m_protos;
private:

};

#endif 