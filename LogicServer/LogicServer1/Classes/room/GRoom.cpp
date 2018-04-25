#include "GRoom.h"
#include "ClientSocket.h"
#include "EventListen.h"
#include "EventDispatcher.h"
#include "HttpLogic.h"
#include "ConfigData.h"

GRoom::GRoom():
m_curbao(0),
m_maxcount(4)
{
	
}

GRoom::~GRoom(){
	
}

bool GRoom::init()
{
	reset();
    return true;
}

void GRoom::reset(){
	m_curbao=0;
	for (int i = 0; i < 4; i++){
		m_udata[i].init();
	}
	m_curchu=0;
	m_isgang=false;
	m_curdraw=0;
	m_curdir=-1;
	m_optype=DRAW_TYPE;
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
	UData ud = m_udata[pos - 1];
	int *pai = ud._hand;
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
	UData ud = m_udata[pos - 1];
	//手牌
	int *pai = ud._hand;
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
	auto itr = ud._cpg.begin();
	
	for (itr; itr != ud._cpg.end(); itr++){
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
	UData ud = m_udata[pos - 1];
	int *pai = ud._hand;
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
	if (!ud._cpg.empty()){
		auto itr = ud._cpg.begin();
		for (itr; itr != ud._cpg.end(); itr++){
			CPGCardData cpgd = *itr;
			int va = cpgd.cards()[0];
			if (cpgd.type() == 2 && va == card){
				return true;
			}
		}
	}
	return false;
}

bool GRoom::isChi(int pos, int card){
	if (card < 0x31){
		UData ud = m_udata[pos - 1];
		int *pai = ud._hand;
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
	
	return false;
}

bool GRoom::isHu(int pos, int card){
	UData ud = m_udata[pos - 1];
	int pai[14];
	for (int i = 0; i < 14; i++){
		int va = ud._hand[i];
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

void GRoom::SendDraw(int pos, int card){

}

void GRoom::SendDiscard(int pos, int card){

}

void GRoom::SendChi(int pos, int *chi){

}

void GRoom::SendPeng(int pos, int card){

}

void GRoom::SendMingGang(int pos, int card){

}

void GRoom::SendAnGang(int pos, int card){

}

void GRoom::SendHu(int pos){

}

void GRoom::NextStep(){

}