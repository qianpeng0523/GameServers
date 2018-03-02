#ifndef __CSVBASEDATA_SCENE_H__
#define __CSVBASEDATA_SCENE_H__

#include "Object.h"
#include "XXIconv.h"

namespace CSV{
	//道具列表item
// 	struct CSVRoom:Object
// 	{
// 		int _id;
// 		int _round;//总共局数
// 		int _ante;//底分
// 		string _remark;
// 		int _numbers;//人数
// 		int _piao;
// 		bool _kan;
// 		int _max;
// 		int _ruletype;//
// 		int _zimo;//自摸
// 		int _sankezi;//三刻字
// 		int _bazhang;//8
// 		int _menqing;//门清
// 		int _du;//du
// 		int _jiang;//将
// 		
// 	};

	struct CSVHuTest:public Object
	{
		string _name;
		string _content;
		string _peng;
		string _minggang;
		string _angang;
		string _bugang;
		string _chi;
		std::vector<int> _handvalues;
		std::vector<int> _pengvalues;
		std::vector<int> _minggangvalues;
		std::vector<int> _angangvalues;
		std::vector<int> _bugangvalues;
		std::vector<std::vector<int>> _chivalues;

		void _copyData(){
			copyData(_content,_handvalues);
			copyData(_peng, _pengvalues);
			copyData(_minggang, _minggangvalues);
			copyData(_angang, _angangvalues);
			copyData(_bugang, _bugangvalues);
			copyData(_chi);
		}

		void copyData(string content, std::vector<int> &values){
			if (content.empty()){
				return;
			}
			
			content = content.substr(1, content.length());
			int index = content.find("|");
			if (index == -1){
				index = content.find("}");
				int t = atoi(content.substr(0, index).c_str());
				values.push_back(t);
			}
			else{
				while (index != -1){
					int t = atoi(content.substr(0, index).c_str());
					values.push_back(t);
					content = content.substr(index + 1, content.length());
					index = content.find("|");
				}
				index = content.find("}");
				int t = atoi(content.substr(0, index).c_str());
				values.push_back(t);
			}
		}
		
		void copyData(string content){
			if (content.empty()){
				return;
			}
			
			content = content.substr(1, content.length());
			int index = content.find("}");
			while (index != -1){
				string temp = content.substr(0, index+1);
				
				std::vector<int> vv;
				copyData(temp, vv);
				_chivalues.push_back(vv);

				content = content.substr(index + 2, content.length());
				
				index = content.find("}");
			}
			
		}
	};

	struct CSVConfig:public Object
	{
		string _server_ip;
		int _server_port;
		string _sql_ip;
		int _sql_port;
		int _log;
		int _robot;
		int _roomcount;
	};

	//胡牌番数列表item
	struct CSVHuFan :public Object
	{
		int _id;
		string _name;
		int _number;

	};

	struct FanItem :public Object
	{
		int _id;
		string _name;
		int _number;
		bool _add;
		bool _mutil;
	};

	struct OverFanItem :public Object
	{
		int _id;
		int _kind;
		int _oid;
	};

	struct GameOverData :public Object
	{
		int _id;
		string _name;
		int _count;
		string _type;
	};

	struct RobotData :public Object{
		int _id;
		string _name;
	};
}

#endif 
