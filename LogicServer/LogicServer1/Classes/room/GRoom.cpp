#include "GRoom.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "HttpLogic.h"
#include "ConfigData.h"
#include "StatTimer.h"

GRoom::GRoom():
m_curbao(0),
m_maxcount(4)
{
	memset(m_udata, NULL, sizeof(UData *)*4);
}

GRoom::~GRoom(){
	for (int i = 0; i < 4; i++){
		if (m_udata[i]){
			delete m_udata[i];
			m_udata[i] = NULL;
		}
	}
}

bool GRoom::init()
{
	reset();
    return true;
}

void GRoom::reset(){
	m_curbao=0;
	m_index = 0;
	m_cards.clear();
	for (int i = 0; i < 4; i++){
		if (m_udata[i]){
			m_udata[i]->init();
		}
	}
	m_curchu=0;
	m_isgang=false;
	m_curdraw=0;
	m_curdir=-1;
	m_optype=DRAW_TYPE;
}

void GRoom::PushUData(UData *ud){
	for (int i = 0; i < 4; i++){
		if (!m_udata[i]){
			m_udata[i] = ud;
			break;
		}
	}
}

UData *GRoom::getUData(string uid){
	for (int i = 0; i < 4; i++){
		UData *u = m_udata[i];
		if (u&& u->_uid.compare(uid)==0){
			return u;
		}
	}
	return NULL;
}

UData *GRoom::getUData(int pos){
	return m_udata[pos - 1];
}

bool GRoom::isPeng(int pos){
	return isPeng(pos, m_curchu);
}

bool GRoom::isMoGang(int pos){
	return isMoGang(pos,m_curdraw);
}

bool GRoom::isChuGang(int pos){
	return isChuGang(pos, m_curchu);
}

bool GRoom::isChi(int pos){
	return isChi(pos, m_curchu);
}

bool GRoom::isHu(int pos){
	return isHu(pos, m_optype == DRAW_TYPE ? m_curdraw : m_curchu);
}

bool GRoom::isPeng(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	int count = 0;
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		if (va>0 && va == card){
			count++;
			if (count >= 2){
				return true;
			}
		}
	}
	return false;
}

bool GRoom::isMoGang(int pos, int card){
	UData *ud = m_udata[pos - 1];
	//手牌
	int *pai = ud->_hand;
	int count = 0;
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		if (va>0 && va == card){
			count++;
			if (count == 3){
				return true;
			}
		}
	}
	vector<int>vec;
	auto itr = ud->_cpg.begin();
	
	for (itr; itr != ud->_cpg.end(); itr++){
		CPGCardData cpgd = *itr;
		int va = cpgd.cards()[0];
		if (cpgd.type() == 2){
			vec.push_back(va);
		}
	}
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		for (int j = 0; j<vec.size(); j++){
			if (va>0 && vec.at(j) == va){
				return true;
			}
		}
	}
	return false;
}

bool GRoom::isChuGang(int pos, int card){
	UData *ud = m_udata[pos - 1];
	if (ud){
		int *pai = ud->_hand;
		int count = 0;
		for (int i = 0; i < 14; i++){
			int va = pai[i];
			if (va>0 && va == card){
				count++;
				if (count == 3){
					return true;
				}
			}
		}
		//碰
		if (!ud->_cpg.empty()){
			auto itr = ud->_cpg.begin();
			for (itr; itr != ud->_cpg.end(); itr++){
				CPGCardData cpgd = *itr;
				int va = cpgd.cards()[0];
				if (cpgd.type() == 2 && va == card){
					return true;
				}
			}
		}
	}
	return false;
}

bool GRoom::isChi(int pos, int card){
	if (CHI_DEFINE == 1){
		if (card < 0x31){
			UData *ud = m_udata[pos - 1];
			int *pai = ud->_hand;
			vector<int *> vecs;
			int v1 = card - 2;
			int v2 = card - 1;
			int v3 = card + 1;
			int v4 = card + 2;
			int a1[2];
			a1[0] = v1;
			a1[1] = v2;
			vecs.push_back(a1);
			int a2[2];
			a2[0] = v2;
			a2[1] = v3;
			vecs.push_back(a2);
			int a3[2];
			a3[0] = v3;
			a3[1] = v4;
			vecs.push_back(a3);
			for (int i = 0; i < vecs.size(); i++){
				int *a = vecs.at(i);
				for (int k = 0; k < 2; k++){
					for (int j = 0; j < 14; j++){
						int va = pai[j];
						if (va>0 && va == a[k]){
							a[k] = -1;
							break;
						}
					}
				}
				if (a[0] == -1 && a[1] == -1){
					return true;
				}
			}
		}
	}
	return false;
}

bool GRoom::isHu(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int pai[14];
	memcpy(pai,ud->_hand,sizeof(int )*14);
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		if (m_optype == DRAW_TYPE&&va == 0){
			pai[i] = card;
		}
		else{
			pai[i] = va;
		}
	}
	HuItem item = ConfigData::getIns()->isHu(pai, m_isgang, m_curbao);
	return item._hutype!=None;
}

void GRoom::SendZhuangHandCards(int pos){
	int pai[14] = { 0 };
	for (int i = 0; i < 14; i++){
		pai[i] = getMJ();
	}
	memcpy(m_udata[pos - 1]->_hand,pai,sizeof(int)*14);
	UData *ud = m_udata[pos - 1];
	//推送给玩家
	
	//test
	printf("zhuanghand:");
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		printf("[%d]", va);
	}
	printf("\n");
	m_zhuangpos = pos;
	
	StatTimer::getIns()->scheduleOnce(this, schedule_selector(GRoom::ZhuangChu), 5.0);
}

void GRoom::ZhuangChu(float dt){
	StatTimer::getIns()->unscheduleSelector(this, schedule_selector(GRoom::ZhuangChu));
	UData *ud = m_udata[m_zhuangpos-1];
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		if (va>0){
			SendDiscard(m_zhuangpos, va);
			break;
		}
	}
}

void GRoom::SendHandCards(int pos){
	int pai[14] = { 0 };
	for (int i = 0; i < 13; i++){
		pai[i] = getMJ();
	}
	memcpy(m_udata[pos - 1]->_hand, pai, sizeof(int)* 14);
	UData *ud = m_udata[pos - 1];
	//推送给玩家

	//test
	printf("otherhand:");
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		printf("[%d]", va);
	}
	printf("\n");
}

void GRoom::SendDraw(int pos, int card){
	pos = (pos%5==0)?1:pos;
	UData *ud = m_udata[pos - 1];
	int *pai=ud->_hand;
	for (int i = 13; i >= 0; i--){
		int va = pai[i];
		if (va==0){
			ud->_hand[i] = card;
			break;
		}
	}
	printf("draw:[%d]->[%d]\n", pos, card);
	m_optype = DRAW_TYPE;
	m_curdir = pos;
	if (isHu(pos)){
		//提示玩家可以胡牌
		//test
		SendHu(pos);
	}
	else{
		//test
		for (int i = 0; i < 14; i++){
			int va = ud->_hand[i];
			if (va>0){
				SendDiscard(pos, va);
				break;
			}
		}
	}
}

void GRoom::SendDiscard(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	for (int i = 13; i >= 0; i--){
		int va = pai[i];
		if (va>0&&va == card){
			ud->_hand[i] = 0;
			break;
		}
	}
	printf("discard:[%d]->[%d]\n", pos, card);
	m_optype = DISCARD_TYPE;
	m_curcpgcard = 0;
	m_isgang = false;
	m_curdir = pos;
	bool ist = false;
	for (int i = 1; i <= 4; i++){
		if (pos != i){
			if (isChi(i, card)){
				ist = true;
			}
			if (isPeng(i, card)){
				ist = true;
				//test
				SendPeng(i, card);
			}
			if (isChuGang(i, card)){
				ist = true;
				//test
				SendMingGang(i, card);
			}
			if (isHu(i, card)){
				ist = true;
				//test
				SendHu(i);
			}
		}
	}
	if (!ist){
		NextStep(FRONT_DRAW);
	}
}

void GRoom::SendChi(int pos, int *chi){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	int ch[2];
	memcpy(ch,chi,sizeof(int)*2);
	
	for (int j = 0; j < 2; j++){
		int card = ch[j];
		for (int i = 13; i >= 0; i--){
			int va = pai[i];
			if (va>0 && va == card){
				ud->_hand[i] = 0;
				pai[i] = 0;
				ch[j] = 0;
				break;
			}
		}
	}
	m_curcpgcard = chi[2];
	m_isgang = false;
	m_curdir = pos;
	//test
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		if (va>0){
			SendDiscard(pos, va);
			break;
		}
	}
}

void GRoom::SendPeng(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	int ch[2] = {card,card};
	for (int j = 0; j < 2; j++){
		int card1 = ch[j];
		for (int i = 13; i >= 0; i--){
			int va = pai[i];
			if (va>0 && va == card1){
				ud->_hand[i] = 0;
				pai[i] = 0;
				ch[j] = 0;
				break;
			}
		}
	}
	printf("peng:[%d]->[%d]\n", pos, card);
	m_curcpgcard = card;
	m_isgang = false;
	m_curdir = pos;

	//test
	for (int i = 0; i < 14; i++){
		int va = ud->_hand[i];
		if (va>0){
			SendDiscard(pos, va);
			break;
		}
	}
}

void GRoom::SendMingGang(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	int ch[3] = { card, card ,card};
	for (int j = 0; j < 3; j++){
		int card1 = ch[j];
		for (int i = 13; i >= 0; i--){
			int va = pai[i];
			if (va>0 && va == card1){
				ud->_hand[i] = 0;
				pai[i] = 0;
				ch[j] = 0;
				break;
			}
		}
	}
	printf("minggang:[%d]->[%d]\n", pos, card);
	m_curcpgcard = card;
	m_isgang = true;
	m_curdir = pos;
	vector<int> vec;
	for (int i = 1; i <= 4; i++){
		if (pos!=i && isHu(pos, card)){
			vec.push_back(i);
		}
	}
	if (vec.empty()){
		NextStep(BACK_DRAW);
	}
	else{
		//提示这些玩家可以胡
		//test
		for (int i = 0; i < vec.size(); i++){
			SendHu(i);
		}
	}
}

void GRoom::SendAnGang(int pos, int card){
	UData *ud = m_udata[pos - 1];
	int *pai = ud->_hand;
	int ch[4] = { card, card, card ,card};
	for (int j = 0; j < 4; j++){
		int card1 = ch[j];
		for (int i = 13; i >= 0; i--){
			int va = pai[i];
			if (va>0 && va == card1){
				ud->_hand[i] = 0;
				pai[i] = 0;
				ch[j] = 0;
				break;
			}
		}
	}
	printf("angang:[%d]->[%d]\n",pos,card);
	m_curcpgcard = card;
	m_isgang = true;
	m_curdir = pos;
	NextStep(BACK_DRAW);
}

void GRoom::SendHu(int pos){
	if (isHu(pos)){
		//可以胡
		printf("hu hu hu\n");
	}
}

void GRoom::NextStep(NEXTSTEPTYPE step){
	int card = 0;
	switch (step)
	{
	case FRONT_DRAW:
		card = getMJ();
		if (card > 0){
			SendDraw(m_curdir + 1, card);
		}
		else{
			printf("end!!!\n");
		}
		break;
	case BACK_DRAW:
		card = getMJ();
		if (card > 0){
			SendDraw(m_curdir, card);
		}
		else{
			printf("end!!!\n");
		}
		break;
	case DISCARD:
		
		break;
	default:
		break;
	}
}

void GRoom::Begin(){
	reset();
	initMJ();

	//test
	SendZhuangHandCards(1);
	SendHandCards(2);
	SendHandCards(3);
	SendHandCards(4);
}

void GRoom::initMJ(){
	m_cards.clear();
	for (int i = 0; i < CARDNUMBER; i++){
		m_cards.push_back(g_all_mj[i]);
	}
	random_shuffle(m_cards.begin(), m_cards.end());
}

int GRoom::getMJ(){
	if (m_index >= m_cards.size()){
		return -1;
	}
	return m_cards.at(m_index++);
}

int GRoom::getMJ(int index){
	if (index<m_cards.size()){
		return m_cards.at(index);
	}
	return -1;
}