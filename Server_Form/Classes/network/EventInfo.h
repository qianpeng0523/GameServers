#ifndef __EventInfo_SCENE_H__
#define __EventInfo_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"


using namespace cocos2d;
using namespace ui;
using namespace std;


class EventInfo : public Object
{
public:
	EventInfo();
	~EventInfo();
    virtual bool init();
	static EventInfo* getIns();
	void pushCMD(int cmd, std::string type_name);
	std::string getTypeName(int cmd);
private:
	static EventInfo *m_ins;
	std::map<int, std::string>m_cmds;
};

#endif // __EventInfo_SCENE_H__
