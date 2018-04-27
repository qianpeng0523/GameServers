
#ifndef __GRoom__
#define __GRoom__

#include "stdafx.h"
#include "RoomLogicInfo.h"
#include "RoomInfo.h"
#include "ConfigData.h"

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
		m_fangzhuuid = rd.uid();
		m_leftround = rd.round();
	}

	void reset();
	void PushUData(UData *ud);
	UData *getUData(string uid);
	UData *getUData(int pos);

	
	void selectZhuang();
	void SendDice(string uid);

	//oprate
	void SendDraw(int pos,int card);
	void SendDiscard(int pos,int card);
	void SendChi(int pos,int *chi);
	void SendPeng(int pos,int card);
	void SendMingGang(int pos,int card);
	void SendAnGang(int pos,int card);
	void SendHu(int pos);
	void SendZhuangHandCards();
	void SendHandCards();

	void NextFrontDraw(float dt);
	void NextBackDraw(float dt);

	void Begin(string uid, int type);

	//
	void HandCardCallBack(float dt);
private:
	vector<CPGCardData> isPeng(int pos);
	vector<CPGCardData> isMoGang(int pos);
	vector<CPGCardData> isChuGang(int pos);
	vector<CPGCardData> isChi(int pos);
	HuItem isHu(int pos);

	vector<CPGCardData> isPeng(int pos, int card);
	vector<CPGCardData> isMoGang(int pos, int card);
	vector<CPGCardData> isChuGang(int pos, int card);
	vector<CPGCardData> isChi(int pos, int card);
	HuItem isHu(int pos, int card);

	void initMJ();
	int getMJ();
	int getMJ(int index);

	int getPosition(string uid);
private:
	int m_maxcount;
	RoomData m_roomdata;
	int m_curbao;
	int m_curbaoniang;
	UData *m_udata[4];
	int m_curchu;
	bool m_isgang;
	int m_curdraw;
	int m_curdir;
	int m_curcpgcard;
	int m_zhuangpos;
	OpTypeEnum m_optype;
	int m_leftround;
	vector<int >m_cards;
	int m_index;
	string m_fangzhuuid;
	RoomLogicInfo *m_pRoomLogicInfo;
	RoomInfo *m_pRoomInfo;
};

#endif 
