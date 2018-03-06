#ifndef __YSEventDispatcher_SCENE_H__
#define __YSEventDispatcher_SCENE_H__

#include "cocos2d.h"
#include "DBProto.h"
#include "EventInfo.h"
#include "Event.h"
#include <fstream>
using namespace cocos2d;

class YSEvent{
public:
	YSEvent(int pcmd, const char* data,int size):
	msg(NULL){
		cmd = pcmd;
		parse(data,size);
	}
	YSEvent(){
		cmd = 0;
		msg = NULL;
	}
	YSEvent* Copy(YSEvent *evt){
		cmd = evt->cmd;
		msg=evt->msg;
		return this;
	}
	int cmd;
	::google::protobuf::Message* msg;
private:
	void parse(const char* data,int size){

		std::string type_name = EventInfo::getIns()->getTypeName(cmd);
		if (!type_name.empty()){
			msg = create_message(type_name);
			msg->ParseFromArray(data,size);

			
			string ss = msg->DebugString();
			ss = ss.substr(0,1024*16);
			log("name:%s,YSEvent:%s  ", type_name.c_str(), ss.c_str());
			
		}
		delete data;
	}
public:
	static google::protobuf::Message* create_message(const std::string& type_name)
	{
		google::protobuf::Message* message = NULL;
		const google::protobuf::Descriptor* descriptor = \
			google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
		if (descriptor)
		{
			const google::protobuf::Message* prototype =
				google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
			if (prototype)
			{
				message = prototype->New();
			}
		}
		return message;
	}
	
};

typedef void (Object::*EventHandler)(YSEvent*);
#define Event_Handler(_SELECTOR)(EventHandler)(&_SELECTOR)

class CallList{
public:
	CallList(){
		evt = new YSEvent();
	}
	~CallList(){
		CC_SAFE_DELETE(evt);
	}
	void Copy(CallList *p){
		evt->Copy(evt);
		cmd = p->cmd;
		obj = p->obj;
		handler = p->handler;
	}
	int cmd;
	Object *obj;
	EventHandler handler;
	YSEvent *evt;
};

typedef std::map<int,CallList *> CallList_Vec;

class YSEventDispatcher : public Object
{
public:
	YSEventDispatcher();
	~YSEventDispatcher();
	
	static YSEventDispatcher *getIns();
	bool init();
	
	void addListener(int cmd, Object *target, EventHandler handler);//添加监听
	void removeListener(int cmd, Object *target, EventHandler handler);//移除监听
	void removeAllKistener();
	void disEventDispatcher(YSEvent *event);
	void EventPathch();

	void update(float dt);
	void CallBack(float dt);
private:
	static YSEventDispatcher* m_ins;
	std::map<int, CallList_Vec> m_eventLists;

	std::vector<YSEvent *> m_Events;
	std::vector<CallList *>m_pCallListVecs;
	bool m_eventLock;
	bool m_islock;
	CallList *m_curClist;
	
private:

};

#endif 