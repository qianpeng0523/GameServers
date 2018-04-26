
#ifndef __GRoom__
#define __GRoom__

#include "stdafx.h"

enum OpTypeEnum{
	DRAW_TYPE=0,
	DISCARD_TYPE,
};

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
	int _gold;
	int _score;
};

enum NEXTSTEPTYPE{
	FRONT_DRAW=0,
	BACK_DRAW,
	DISCARD,

};

class GRoom:public Object
{
public:
	GRoom();
	~GRoom();
	bool init();

	void Comein();

	RoomData getRoomData(){
		return m_roomdata;
	}
	void setRoomData(RoomData rd){
		m_roomdata.CopyFrom(rd);
	}

	void reset();
	void PushUData(UData *ud);
	UData *getUData(string uid);
	UData *getUData(int pos);

	//oprate
	void SendDraw(int pos,int card);
	void SendDiscard(int pos,int card);
	void SendChi(int pos,int *chi);
	void SendPeng(int pos,int card);
	void SendMingGang(int pos,int card);
	void SendAnGang(int pos,int card);
	void SendHu(int pos);
	void SendZhuangHandCards(int pos);
	void SendHandCards(int pos);

	void NextStep(NEXTSTEPTYPE step);
	void Begin();

	//test
	void ZhuangChu(float dt);
private:
	bool isPeng(int pos);
	bool isMoGang(int pos);
	bool isChuGang(int pos);
	bool isChi(int pos);
	bool isHu(int pos);

	bool isPeng(int pos,int card);
	bool isMoGang(int pos, int card);
	bool isChuGang(int pos, int card);
	bool isChi(int pos, int card);
	bool isHu(int pos, int card);

	void initMJ();
	int getMJ();
	int getMJ(int index);
private:
	int m_maxcount;
	RoomData m_roomdata;
	int m_curbao;
	UData *m_udata[4];
	int m_curchu;
	bool m_isgang;
	int m_curdraw;
	int m_curdir;
	int m_curcpgcard;
	int m_zhuangpos;
	OpTypeEnum m_optype;

	vector<int >m_cards;
	int m_index;
};

#endif 
