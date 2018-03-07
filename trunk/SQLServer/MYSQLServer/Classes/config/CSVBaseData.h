#ifndef __CSVBASEDATA_SCENE_H__
#define __CSVBASEDATA_SCENE_H__

#include "Object.h"
#include "XXIconv.h"

namespace CSV{
	struct RobotData :public Object{
		int _id;
		string _name;
	};
	struct GateData :public Object{
		int _id;
		string _name;
		string _ip;
		int _port;
	};
}

#endif 
