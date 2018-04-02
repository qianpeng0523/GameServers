#ifndef __CSVBASEDATA_SCENE_H__
#define __CSVBASEDATA_SCENE_H__

#include "Object.h"
#include "XXIconv.h"
#include "Common.h"
#include "redis.h"

namespace CSV{
	struct RobotData :public Object{
		int _id;
		string _name;
	};

	struct GateData :public Object{
		GateData(){
			_type = 0;
			memset(_name,0,8);
			memset(_ip, 0, 16);
		}
		~GateData(){

		}
		int _id;
		int _port;
		int _type;
		char _name[8];
		char _ip[16];
		void DebugPrintf(){
			printf("%d %d %d %s %s\n",_id,_port,_type,_name,_ip);
		}
		char *getData(){
			int sz = sizeof(GateData);
			char *buffer = new char[sz];
			memset(buffer, 0, sz);
			int idlen = sizeof(_id);
			char *idstr = (char *)&_id;
			int len = sizeof(Object);//涉及到继承结构体字节对齐
			for (int i = 0; i < idlen; i++){
				buffer[len + i] = *(idstr + i);
			}
			len += idlen;
			int portlen = sizeof(_port);
			char *portstr = (char *)&_port;
			for (int i = 0; i < portlen; i++){
				buffer[len + i] = *(portstr + i);
			}
			len += portlen;
			int typelen = sizeof(_type);
			char *typestr = (char *)&_type;
			for (int i = 0; i < typelen; i++){
				buffer[len + i] = *(typestr + i);
			}
			len += typelen;
			for (int i = 0; i < 8; i++){
				buffer[len+i] = _name[i];
			}
			len += 8;
			for (int i = 0; i < 16; i++){
				buffer[len+i] = _ip[i];
			}

			redis::getIns()->ZeroChange(buffer,sz);

			return buffer;
		}
	};
}

#endif 
