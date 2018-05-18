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
		GateData(){
			_use = false;
			_type = 0;
		}
		int _id;
		string _name;
		string _ip;
		int _port;
		bool _use;
		int _type;
	};
	/*struct CSVHuItem :public Object
	{
		string _key;
		int _value;
	};*/
	struct REDISDBName :public Object
	{
		string _name;
		int _dbindex;
	};
}

#endif 
