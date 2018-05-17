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
			memset(_ip, 0, 48);
		}
		~GateData(){

		}
		int _id;
		int _port;
		int _type;
		char _name[8];
		char _ip[48];
		void DebugPrintf(){
			CLog::log("%d %d %d %s %s\n",_id,_port,_type,_name,_ip);
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
			for (int i = 0; i < 48; i++){
				buffer[len+i] = _ip[i];
			}

			return buffer;
		}
	};
	//奖励
	struct CSVReward :public Object
	{
		int _rid;
		int _pid;
		int _pnumber;
	};
	//商城
	struct CSVShopItem:public Object
	{
		int _sid;
		int _rid;
		int _conid;
		int _giveid;
		bool _hot;
	};

	//任务和免费
	struct CSVTaskItem:public Object
	{
		int _tid;
		string _title;
		string _content;
		int _count;
		int _type;
		vector<int > _rewardid;
	};

	struct CSVSignItem:public Object
	{
		int _id;
		int _day;
		int _rid;//rewardid
	};
	struct CSVSignZhuan :public Object
	{
		int _index;
		int _rid;
	};
	struct CSVExchangeAward :public Object
	{
		int _eid;
		string _title;
		int _rid;
		int _brid;
	};
	struct CSVActive :public Object
	{
		int _id;
		int _type;
		string _url;
	};
	struct CSVProp :public Object
	{
		int _pid;
		string _name;
	};
	//任务和免费
	struct CSVFree :public Object
	{
		int _tid;
		string _title;
		string _content;
		int _count;
		vector<int > _rewardid;
	};

	//首充
	struct CSVFirstBuyItem :public Object
	{
		int _sid;
		string _rid;
		string _conid;
		string _giveid;
	};

	//首充
	struct CSVExchangeCode :public Object
	{
		int _id;
		string _rewardid;
		string _code;
	};

	struct REDISDBName:public Object
	{
		string _name;
		int _dbindex;
	};
}

#endif 
