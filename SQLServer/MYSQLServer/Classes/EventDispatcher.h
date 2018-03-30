#ifndef __EventDispatcher_SCENE_H__
#define __EventDispatcher_SCENE_H__

#include "ccEvent.h"
#include <fstream>

class EventDispatcher : public Object
{
public:
	EventDispatcher();
	~EventDispatcher();
	
	static EventDispatcher *getIns();
	bool init();
	
	void registerProto(int cmd,string tname);
	string getProtoName(int cmd);
private:
	
private:
	static EventDispatcher* m_ins;
	map<int, string> m_protos;
private:

};

#endif 