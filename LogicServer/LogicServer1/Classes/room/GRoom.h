
#ifndef __GRoom__
#define __GRoom__

#include "stdafx.h"

struct UData
{
	void init(){
		memset(_hand,0,sizeof(int)*14);
		_cpg.clear();
		_out.clear();
		_fa;
	}
	string _uid;
	int _hand[14];
	vector<CPGCardData> _cpg;
	vector<int> _out;
	int _fa;
};

class GRoom:public Object
{
public:
	GRoom();
	~GRoom();
	bool init();


	RoomData getRoomData(){
		return m_roomdata;
	}
	void reset();
private:
	int m_maxcount;
	RoomData m_roomdata;
	int m_curbao;
	UData m_udata[4];
};

#endif 
