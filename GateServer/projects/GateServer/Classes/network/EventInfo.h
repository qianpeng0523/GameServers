#ifndef __EventInfo_SCENE_H__
#define __EventInfo_SCENE_H__

#include "stdafx.h"


using namespace std;


class EventInfo : public Object
{
public:
	EventInfo();
	~EventInfo();
    virtual bool init();
	static EventInfo* getIns();
	void pushCMD(int cmd, string type_name);
	string getTypeName(int cmd);
private:
	static EventInfo *m_ins;
	map<int, string>m_cmds;
};

#endif // __EventInfo_SCENE_H__
