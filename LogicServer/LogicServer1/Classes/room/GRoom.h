
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
	UData(){
		_online = true;
		_gold = 0;
		_score = 0;
		init();
	}
	void init(){
		memset(_hand,0,sizeof(int)*14);
		_cpg.clear();
		_out.clear();
		_fa = 0;
		_ready = false;
	}
	string _uid;
	int _hand[14];
	vector<CPGCardData> _cpg;
	vector<int> _out;
	int _fa;
	int _gold;
	int _score;
	bool _ready;
	bool _online;
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
	void PopUData(UData *ud);
	UData *getUData(string uid);
	UData *getUData(int pos);
	UData **getUDatas(){
		return m_udata;
	}
	
	void selectZhuang();
	void SendDice(string uid);
	int getZhuang();

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
	void Ready(string uid,bool ready);
	void Leave(string uid,bool zhudong);
	void onLine(string uid,bool online);
	bool DissolveRoom(string uid);
	bool Agree(string uid,bool isagree);
	void VoteResult(bool isDissolve);
	int getPosition(string uid);
	bool isBegin(){
		return m_isbegin;
	}
	//
	void HandCardCallBack(float dt);
	void setFangZhuUid(string uid){
		m_fangzhuuid = uid;
	}
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

	void update(float dt);
	void openUpdate(bool isopen);
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
	string m_dissoveuid;
	map<string,int>m_voteuids;
	RoomLogicInfo *m_pRoomLogicInfo;
	RoomInfo *m_pRoomInfo;
	bool m_isbegin;
	time_t m_lasttime;
	bool m_isopen;
	string m_tipuid;
};

#endif 
