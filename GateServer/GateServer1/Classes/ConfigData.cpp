
#include "ConfigData.h"
#include "time.h"
#include "Common.h"
#include "CSVDataInfo.h"
#include "RedisGet.h"
#include "RedisPut.h"

bool compare(int a, int b){
	if (a < b){
		return true;
	}
	else{
		return false;
	}
}


ConfigData *ConfigData::m_ins=NULL;
ConfigData::ConfigData(){
	m_index = 0;
	srand((unsigned int)time(NULL));
}

ConfigData::~ConfigData(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

ConfigData *ConfigData::getIns(){
	if (!m_ins){
		m_ins = new ConfigData();
		m_ins->init();
	}
	return m_ins;
}

int ConfigData::getMJ(){
	if (m_index >= m_cards.size()){
		return -1;
	}
	int va = m_cards.at(m_index++);
#ifndef DONGXINANBEI
	if (va >= 0x31 && va <= 0x34){
		return getMJ();
	}
#else

#endif
	return va;
}

int ConfigData::getMJ(int index){
	if (index >= m_cards.size()){
		return -1;
	}
	return m_cards.at(index);
}

void ConfigData::initMJ(){
	for (int i = 0; i < CARDNUMBER; i++){
		m_cards.push_back(g_all_mj[i]);
	}
	random_shuffle(m_cards.begin(), m_cards.end());
}

uint64 ConfigData::getRedisLastIndex(string key){
	char *dd = m_predis->getLastList(key);
	if (dd == NULL){
		return 0;
	}
	string tt = dd;
	CLog::log("tt:%s,dd:%s",tt.c_str(),dd);
	delete dd;
	return atoi(tt.c_str());
}

void ConfigData::init(){
	
	int64_t t = Common::getCurrentTime();
	RedisGet::getIns()->getHuList(m_lianke,1);
	RedisGet::getIns()->getHuList(m_liankebao,2);
	if (m_lianke.empty()){
		setKezi();
		setShunzi();
		for (int i = 1; i <= 4; i++){
			for (int j = 0; j <= i; j++){
				init3P(i, j);
			}
		}
		RedisPut::getIns()->PushHus(m_lianke,1);
	}
	char buff[200];
	if (m_liankebao.empty()){
		for (int i = 1; i <= 4; i++){
			setLiankeBao(i);
		}
		RedisPut::getIns()->PushHus(m_liankebao,2);
	}
	
	
	
	int64_t t2 = Common::getCurrentTime();
	uint64_t tt = t2 - t;
	CLog::log("******use time:%fs******\n", tt / 1000.0 / 1000);



	//test();
	
}
#define  TESTCOUNT 50
void ConfigData::test(){
	
	int a[TESTCOUNT][14] = { 0 };
	int b[TESTCOUNT][14] = { 0 };
	int index = 0;
	int64_t t = Common::getCurrentTime();
	while (index < TESTCOUNT){
		//胡牌和出牌听牌测试
		for (int i = 2; i <= 14;i+=3){
			initMJ();
			for (int j = 0; j < i; j++){
				a[index][j] = getMJ();
			}
		}
		quickSort(a[index], 0, 13);
		//听牌随机牌测试
		for (int i = 1; i <= 13; i += 3){
			initMJ();
			for (int j = 0; j < i; j++){
				b[index][j] = getMJ();
			}
		}
		quickSort(b[index], 0, 13);
		index++;
	}
	int64_t ttt = Common::getCurrentTime();
	int64_t tt = ttt - t;
	CLog::log("******rand.use time:%fs******\n", tt / 1000.0 / 1000);


	index = 0;
	string tp;
	while (index < TESTCOUNT){
		HuItem item = isHu(a[index], false,1);
		
		HuTypeEnum type = item._hutype;
		switch (type)
		{
		case None:
			tp = "None";
			break;
		case PI:
			tp = "PI";
			break;
		case PENGPENG:
			tp = "PENGPENG";
			break;
		case QIDUI:
			tp = "QIDUI";
			break;
		case QINGYISE:
			tp = "QINGYISE";
			break;
		case QYSPENG:
			tp = "QYSPENG";
			break;
		case QYSQD:
			tp = "QYSQD";
			break;
		default:
			break;
		}
		CLog::log("[%d]{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}:%s-%s",index+1,
			a[index][0], a[index][1], a[index][2], a[index][3], a[index][4], a[index][5], a[index][6],
			a[index][7], a[index][8], a[index][9], a[index][10], a[index][11], a[index][12], a[index][13],
			tp.c_str(), item._hy == HEI ? "HEI" : "RUAN"
			);
		index++;
	}
	int64_t t3 = Common::getCurrentTime();
	tt = t3 - ttt;
	CLog::log("******Hu.use time:%fs******\n", tt / 1000.0 / 1000);

	index = 0;
	char buff[30];
	
	while (index < TESTCOUNT){
		tp = "";
		map<int, int> vec;
		isTing(b[index], 1,vec);
		auto itr = vec.begin();
		for (itr; itr != vec.end();itr++){
			int va = itr->first;
			sprintf(buff, "0x%02X",va);
			if (itr != vec.begin()){
				tp += ",";
			}
			tp += buff;
		}
		CLog::log("Ting.[%d]{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}:[%s]", index + 1,
			b[index][0], b[index][1], b[index][2], b[index][3], b[index][4], b[index][5], b[index][6],
			b[index][7], b[index][8], b[index][9], b[index][10], b[index][11], b[index][12], b[index][13],
			tp.c_str()
			);

		index++;
		
	}
	int64_t t2 = Common::getCurrentTime();
	tt = t2 - t3;
	CLog::log("******Ting.use time:%fs******\n", tt / 1000.0 / 1000);

	index = 0;
	while (index < TESTCOUNT){
		map<int, map<int, int>> vec1 = chuTing(a[index], 1);
		CLog::log("ChuTing.[%d]{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}", index + 1,
			a[index][0], a[index][1], a[index][2], a[index][3], a[index][4], a[index][5], a[index][6],
			a[index][7], a[index][8], a[index][9], a[index][10], a[index][11], a[index][12], a[index][13]
			);
		auto itr1 = vec1.begin();
		for (itr1; itr1 != vec1.end(); itr1++){
			auto vec = &itr1->second;
			auto itr = vec->begin();
			tp = "";
			for (itr; itr != vec->end(); itr++){
				int va = itr->first;
				sprintf(buff, "0x%02X", va);
				if (itr != vec->begin()){
					tp += ",";
				}
				tp += buff;
			}
			CLog::log("%d.CHU[0x%02X]-Ting[%s]",index+1,itr1->first,tp.c_str());
		}
		index++;
	}
	int64_t t1 = Common::getCurrentTime();
	tt = t1 - t2;
	CLog::log("******ChuTing.use time:%fs******\n", tt/1000.0/1000);
	
// 	int a[14] = { 0x03, 0x03, 0x05, 0x04, 0x04,0x05,0x06,0x07,0x06,0x07,0x08,0x08,0x09,0x09};
// 	map<int, int> vec;
// 	HuItem item= isHu(a, false,1);
// 
// 	int a1[14] = { 0x01, 0x03, 0x05, 0x04, 0x04, 0x05, 0x06, 0x07, 0x06, 0x07, 0x08, 0x08, 0x09, 0x09 };
// 	HuItem item1 = isHu(a1, false, 1);

	CLog::log("1111\n");
}

void ConfigData::quickSort(int *s, int l, int r)
{
	if (l < r)
	{
		int i = l, j = r, x = s[l];
		while (i < j)
		{
			while (i < j && s[j] >= x) // 从右向左找第一个小于x的数  
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && s[i] < x) // 从左向右找第一个大于等于x的数  
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // 递归调用  
		quickSort(s, i + 1, r);
	}
}

void ConfigData::setKezi(){
	for (int i = 0; i < 9;i++){
		vector<int >vec;
		for (int j = 0; j < 3; j++){
			int v = i+1;
			vec.push_back(v);
		}
		m_kezi.push_back(vec);
	}
}

void ConfigData::setShunzi(){
	for (int i = 0; i < 7; i++){
		int v1 = i+1;
		int v2 = i+2;
		int v3 = i+3;
		vector<int >vec;
		vec.push_back(v1);
		vec.push_back(v2);
		vec.push_back(v3);
		m_shunzi.push_back(vec);
	}
}

void ConfigData::init3P(int index, int kenum){
	map<string, int> maps;
	int kzsz = m_kezi.size();
	char buff[200];
	memset(buff, 0, 200);
	int shunnum = index - kenum;
	if (kenum > 0){
		for (int i = 0; i < kzsz; i++){
			if (kenum>1){
				for (int j = 0; j != i&& j<kzsz; j++){
					if (kenum > 2){
						for (int m = 0; m != j&& m != i&& m<kzsz; m++){
							if (kenum > 3){
								for (int n = 0; n != m&& n != j&& n != i&& n < kzsz; n++){
									vector<int >*vec = &m_kezi.at(i);
									vector<int >*vec1 = &m_kezi.at(j);
									vector<int >*vec2 = &m_kezi.at(m);
									vector<int >*vec3 = &m_kezi.at(n);
									vector<int>vvvvv;
									for (int kk = 0; kk < vec->size(); kk++){
										vvvvv.push_back(vec->at(kk));
										vvvvv.push_back(vec1->at(kk));
										vvvvv.push_back(vec2->at(kk));
										vvvvv.push_back(vec3->at(kk));
									}
									
									init3L(shunnum, index, vvvvv);
								}
							}
							else{
								vector<int >vec = m_kezi.at(i);
								vector<int >vec1 = m_kezi.at(j);
								vector<int >vec2 = m_kezi.at(m);
								vector<int>vvvvv;
								for (int kk = 0; kk < vec.size(); kk++){
									vvvvv.push_back(vec.at(kk));
									vvvvv.push_back(vec1.at(kk));
									vvvvv.push_back(vec2.at(kk));
								}
								init3L(shunnum, index, vvvvv);
							}
						}
					}
					else{
						vector<int >vec = m_kezi.at(i);
						vector<int >vec1 = m_kezi.at(j);
						vector<int>vvvvv;
						for (int kk = 0; kk < vec.size(); kk++){
							vvvvv.push_back(vec.at(kk));
							vvvvv.push_back(vec1.at(kk));
						}
						init3L(shunnum, index, vvvvv);
					}
				}
			}
			else{
				vector<int >vec = m_kezi.at(i);
				init3L(shunnum,index,vec);
			}
		}
	}
	else{
		vector<int >vec;
		init3L(shunnum, index, vec);
	}
	int co = index * 3 + 2;
	if (shunnum == 0){
		co = co * 10 + 2;
	}
	CLog::log("*********%d**********\n", co);
	
}

void ConfigData::init3L(int shunnum, int index, vector<int>&ww){
	char buff[300];
	int co = index * 3 + 2;
	if (shunnum == 0){
		co = co*10+2;
	}
	
	int szsz = m_shunzi.size();
	map<uint64,int> maps;
	if (shunnum > 0){
		for (int i = 0; i < szsz; i++){
			if (shunnum>1){
				for (int j = 0; j<szsz; j++){
					if (shunnum > 2){
						for (int m = 0; m<szsz; m++){
							if (shunnum > 3){
								for (int n = 0; n < szsz; n++){
									vector<int >vec = m_shunzi.at(i);
									vector<int >vec1 = m_shunzi.at(j);
									vector<int >vec2 = m_shunzi.at(m);
									vector<int >vec3 = m_shunzi.at(n);
									vector<int>vvvvv;
									for (int kk = 0; kk < vec.size(); kk++){
										vvvvv.push_back(vec.at(kk));
										vvvvv.push_back(vec1.at(kk));
										vvvvv.push_back(vec2.at(kk));
										vvvvv.push_back(vec3.at(kk));
									}
									for (int jj = 0; jj < ww.size(); jj++){
										vvvvv.push_back(ww.at(jj));
									}
									sort(vvvvv.begin(), vvvvv.end(), compare);
									for (int mm = 0; mm < vvvvv.size(); mm++){
										buff[mm] = '0' + vvvvv.at(mm);
									}
									maps.insert(make_pair(atoll(buff), 0));
								}
							}
							else{
								vector<int >vec = m_shunzi.at(i);
								vector<int >vec1 = m_shunzi.at(j);
								vector<int >vec2 = m_shunzi.at(m);
								vector<int>vvvvv;
								for (int kk = 0; kk < vec.size(); kk++){
									vvvvv.push_back(vec.at(kk));
									vvvvv.push_back(vec1.at(kk));
									vvvvv.push_back(vec2.at(kk));
								}
								for (int jj = 0; jj < ww.size(); jj++){
									vvvvv.push_back(ww.at(jj));
								}
								sort(vvvvv.begin(), vvvvv.end(), compare);
								for (int mm = 0; mm < vvvvv.size(); mm++){
									buff[mm] = '0' + vvvvv.at(mm);
								}
								maps.insert(make_pair(atoll(buff), 0));
							}
						}
					}
					else{
						vector<int >vec = m_shunzi.at(i);
						vector<int >vec1 = m_shunzi.at(j);
						vector<int>vvvvv;
						for (int kk = 0; kk < vec.size(); kk++){
							vvvvv.push_back(vec.at(kk));
							vvvvv.push_back(vec1.at(kk));
						}
						for (int jj = 0; jj < ww.size(); jj++){
							vvvvv.push_back(ww.at(jj));
						}
						sort(vvvvv.begin(), vvvvv.end(), compare);
						for (int mm = 0; mm < vvvvv.size(); mm++){
							buff[mm] = '0' + vvvvv.at(mm);
						}
						maps.insert(make_pair(atoll(buff), 0));
					}
				}
			}
			else{
				vector<int >vec = m_shunzi.at(i);
				vector<int>vvvvv;
				for (int kk = 0; kk < vec.size(); kk++){
					vvvvv.push_back(vec.at(kk));
				}
				for (int jj = 0; jj < ww.size(); jj++){
					vvvvv.push_back(ww.at(jj));
				}
				sort(vvvvv.begin(), vvvvv.end(), compare);
				for (int mm = 0; mm < vvvvv.size(); mm++){
					buff[mm] = '0' + vvvvv.at(mm);
				}
				maps.insert(make_pair(atoll(buff), 0));
			}
		}
	}
	else{
		if (!ww.empty()){
			sort(ww.begin(), ww.end(), compare);
			for (int mm = 0; mm < ww.size(); mm++){
				buff[mm] = '0' + ww.at(mm);
			}
			maps.insert(make_pair(atoll(buff), 0));
		}
	}
	
	setFengKeTo(co,maps);
}

void ConfigData::setFengKeTo(int len, map<uint64, int> &maps){
	if (m_lianke.find(len) == m_lianke.end()){
		m_lianke.insert(make_pair(len, maps));
	}
	else{
		map<uint64, int>*mp = &m_lianke.at(len);
		map<uint64, int>::iterator itr = maps.begin();
		for (itr; itr != maps.end(); itr++){
			mp->insert(make_pair(itr->first,itr->second));
			
		}
	}
}

HuItem ConfigData::isHu(int *pai,bool ispengqing){
	bool isq = ispengqing;
	HuItem huitem;
	huitem._hutype = None;
	huitem._hy = HEI;
	//统计总牌数
	map<int, int>dui;
	int count = 0;
	int qing = -1;
	int duicount = 0;
	for (int i = 0; i < 14; i++){
		int v = pai[i];
		if (v>0){
			count++;
			if (dui.find(v) == dui.end()){
				dui.insert(make_pair(v, 1));
			}
			else{
				dui.at(v)++;
			}
			if (qing == -1){
				qing = v / 16;
			}
			else if (qing != v / 16){
				isq = false;
			}
			int co = dui.at(v);
			if (co == 2 || co == 4){
				duicount++;
			}
		}
	}
	//统计对子数
	
	if (duicount==0){
		return huitem;
	}
	else if (duicount == 7){
		huitem._hutype = isq?QYSQD:QIDUI;
		return huitem;
	}
	else{
		if (count == 2){
			huitem._hutype =isq? QYSPENG:PENGPENG;
			return huitem;
		}
		
		auto itr1 = dui.rbegin();
		for (itr1; itr1 != dui.rend();itr1++){
			int count1 = itr1->second;
			if (count1 < 2){
				continue;
			}
			int temppai[14] = { 0 };
			memcpy(temppai,pai,sizeof(int)*14);
			int v = itr1->first;
			int baocount = 0;
			setValueZero(temppai,v,2,baocount);
			map<int, vector<int>> kindcards;
			getKindCard(temppai, kindcards);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end();itr++){
				int baocount = 0;
				HuTypeEnum type = isFit(&itr->second, baocount, huitem, itr->first);
				if (ii > 0){
					if (type>None&& type > lasttype){
						lasttype = type;
					}
					else if (type == None){
						lasttype = type;
					}
				}
				else{
					lasttype = type;
				}

				if (type == None){
					ii++;
					break;
				}
				ii++;
			}
			if (lasttype == PI){
				huitem._hutype = isq ? QINGYISE : PI;
				return huitem;
			}
			else if (lasttype == PENGPENG){
				huitem._hutype = isq ? QYSPENG : PENGPENG;
				return huitem;
			}
		}
		return huitem;
	}
	return huitem;
}

void ConfigData::getKindCard(int *temppai, map<int, vector<int>> &kindcards){
	for (int i = 0; i < 14; i++){
		int v = temppai[i];
		if (v>0){
			int kind = v / 16;
			if (kindcards.find(kind) == kindcards.end()){
				vector<int >vv;
				vv.push_back(v % 16);
				kindcards.insert(make_pair(kind, vv));
			}
			else{
				vector<int > *vv = &kindcards.at(kind);
				vv->push_back(v % 16);
			}
		}
	}
}

HuItem ConfigData::isHu(int *pai, bool ispengqing, int bao){
	quickSort(pai, 0, 13);
	HuItem huitem = isHu(pai, ispengqing);
	if (huitem._hutype == QYSPENG || huitem._hutype == QYSQD || huitem._hutype == QINGYISE){
		//这些黑的比任何软的胡都大，所以不用考虑软的胡
		huitem._hy = HEI;
		return huitem;
	}
	bool isq = ispengqing;
	int temppai[14] = { 0 };
	memcpy(temppai, pai, sizeof(int)* 14);
	int baocount = 0;
	int count = 0;
	map<int, int> dui;
	int qing = -1;
	int duicount = 0;
	//统计总牌数
	for (int i = 0; i < 14; i++){
		int v = temppai[i];
		if (v>0){
			if (v == bao){
				baocount++;
				temppai[i] = 0;
			}
			else{
				count++;
				if (dui.find(v) == dui.end()){
					dui.insert(make_pair(v, 1));
				}
				else{
					dui.at(v)++;
				}
				if (qing == -1){
					qing = v / 16;
				}
				else if (qing != v / 16){
					isq = false;
				}
				int co = dui.at(v);
				if (co == 2||co==4){
					duicount++;
				}
			}
		}
	}
	if (baocount == 0){
		return huitem;
	}
	//统计对子数
	if ((baocount == 1 && duicount==6) || (baocount == 2 && duicount==5) || (baocount == 3 && duicount==4)||(baocount==4&&duicount==3)){
		huitem._hutype = isq ? QYSQD : QIDUI;
		huitem._hy = RUAN;
	}
	else{
		if (count == 0){
			huitem._hutype = isq ? QYSPENG:PENGPENG;
			huitem._hy = HEI;
			return huitem;
		}
		else if (count == 1){
			huitem._hutype = isq ? QYSPENG:PENGPENG;
			huitem._hy = RUAN;
			return huitem;
		}
		else if (count==2){
			if (duicount == 1){
				huitem._hutype = isq ? QYSPENG:PENGPENG;
				huitem._hy = HEI;
				return huitem;
			}
			else{
				huitem._hutype = isq ? QYSPENG:PENGPENG;
				huitem._hy = RUAN;
				return huitem;
			}
		}
		else if (duicount==0){
			auto itr1 = dui.begin();
			for (itr1; itr1 != dui.end();itr1++){
				int v = itr1->first;
				int co = itr1->second;
				if (v>0&&co==1){
					itr1->second++;
				}
			}
		}
		
		map<int, int>::iterator itr1 = dui.begin();
		for (itr1; itr1 != dui.end(); itr1++){
			int count1 = itr1->second;
			int temppai1[14] = { 0 };
			memcpy(temppai1, temppai, sizeof(int)* 14);
			int v = itr1->first;
			int baocount1 = baocount;
			setValueZero(temppai1, v, 2,baocount1);
			map<int, vector<int>> kindcards;
			getKindCard(temppai1, kindcards);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end(); itr++){
				
				HuTypeEnum type = isFit(&itr->second, baocount1, huitem, itr->first);
				
				if (ii > 0){
					if (type > None&& type > lasttype){
						lasttype = type;
						huitem._hy = RUAN;
					}
					else if (type == None){
						lasttype = type;
					}
				}
				else{
					lasttype = type;
				}

				if (type == None){
					ii++;
					break;
				}
				ii++;
			}
			
			if (lasttype == PI){
				if (huitem._hutype == None){
					huitem._hutype = isq ? QINGYISE : PI;
					huitem._hy = RUAN;
				}
				return huitem;
			}
			else if (lasttype == PENGPENG){
				if (isq&&huitem._hutype==PENGPENG){
					huitem._hutype = QYSPENG;
					huitem._hy = RUAN;
					return huitem;
				}
				return huitem;
			}
		}
		return huitem; 
	}
	return huitem;
}

HuTypeEnum ConfigData::isFit(vector<int>*p, int &baocount, HuItem &item, int kind){
	char buff[200];
	int len = p->size();
	if (len == 0){
		item._hy = HEI;
		return PENGPENG;
	}
	for (int i = 0; i < len; i++){
		buff[i] = '0' + p->at(i);
	}
	int l = (len / 3 + (len % 3 == 0 ? 0 : 1)) * 3 + 2;
	int co = l * 10 + 2;//碰碰胡
	int baoco1 = l * 100 + baocount;
	int baoco2 = co * 100 + baocount;//碰碰胡

	map<int, map<uint64, int>>::iterator itr1 = m_lianke.find(co);
	if (itr1 != m_lianke.end()){
		map<uint64, int> *maps = &itr1->second;
		if (maps->find(atoll(buff)) != maps->end()){
			item._hy = HEI;
			return PENGPENG;
		}
	}

	int sub = (((strlen(buff) - 1) / 3 + 1) * 3 - strlen(buff));
	if (baocount > 0){
		itr1 = m_liankebao.find(baoco2);
		if (itr1 != m_liankebao.end()){
			map<uint64, int> *maps = &itr1->second;
			if (maps->find(atoll(buff)) != maps->end()){
				item._hy = RUAN;
				baocount -= sub;
				return PENGPENG;
			}
		}
	}

	if (kind < 3){
		itr1 = m_lianke.find(l);
		if (itr1 != m_lianke.end()){
			map<uint64, int> *maps = &itr1->second;
			if (maps->find(atoll(buff)) != maps->end()){
				item._hy = HEI;
				return PI;
			}
		}
	}

	if (baocount > 0){
		if (kind < 3){
			itr1 = m_liankebao.find(baoco1);
			if (itr1 != m_liankebao.end()){
				map<uint64, int> *maps = &itr1->second;
				if (maps->find(atoll(buff)) != maps->end()){
					item._hy = RUAN;
					baocount -= sub;
					return PI;
				}
			}
		}
	}
	
	return None;
}

void ConfigData::setValueZero(int *a, int v, int len, int &baocount){
	int index = 0;
	for (int i = 0; i < 14; i++){
		if (a[i] == v){
			a[i] = 0;
			index++;
			if (index >= len){
				break;
			}
		}
	}
	baocount -=(2 - index);
}

void ConfigData::setLiankeBao(int i){
	char buff[200];
	map<int, map<uint64, int>>::iterator itr = m_lianke.begin();
	for (itr; itr != m_lianke.end();itr++){
		int keykey = itr->first*100+i;
		map<uint64, int>maps1;
		map<uint64, int> *maps = &itr->second;
		map<uint64, int>::iterator itr1 = maps->begin();
		for (itr1; itr1 != maps->end();itr1++){
			uint64 key = itr1->first;
			sprintf(buff,"%lld",key);
			int len = strlen(buff);
			if (i >= 1){
				for (int j = 0; j < len; j++){
					if (i >= 2){
						for (int k = j + 1; k < len; k++){
							if (i >= 3){
								for (int m = k + 1; m < len; m++){
									if (i >= 4){
										for (int n = m + 1; n < len; n++){
											string kkey = buff;
											kkey.erase(n,1);
											kkey.erase(m, 1);
											kkey.erase(k, 1);
											kkey.erase(j, 1);
											
											if (!kkey.empty()){
												maps1.insert(make_pair(atoll(kkey.c_str()), 0));
											}

										}
									}
									else{
										string kkey = buff;
										kkey.erase(m, 1);
										kkey.erase(k, 1);
										kkey.erase(j, 1);
										if (!kkey.empty()){
											maps1.insert(make_pair(atoll(kkey.c_str()), 0));
										}
									}
								}
							}
							else{
								string kkey = buff;
								kkey.erase(k, 1);
								kkey.erase(j, 1);
								if (!kkey.empty()){
									maps1.insert(make_pair(atoll(kkey.c_str()), 0));
								}
							}
						}
					}
					else{
						string kkey = buff;
						kkey.erase(j, 1);
						if (!kkey.empty()){
							maps1.insert(make_pair(atoll(kkey.c_str()), 0));
						}
					}
				}
			}
		}
		if (!maps1.empty()){
			CLog::log("***********baoke%d*********",keykey);
			m_liankebao.insert(make_pair(keykey, maps1));
		}
	}
	
}

void ConfigData::isTing(int *pai, int bao, map<int, int> &ting){
	int count = 0;
	int index = -1;
	for (int i = 0; i < 14; i++){
		if (index==-1&&pai[i] == 0){
			index = i;
		}
		else{
			count++;
		}
	}
	if (count == 2 || count == 5 || count == 8 || count == 11 || count == 14){
		return ;
	}
	
	for (int i = 0; i < g_kind;i++){
		int v = g_all_mjkind[i];
		int tp[14];
		memcpy(tp,pai,sizeof(int)*14);
		if (v >= 0x31 && v <= 0x34){
#ifdef DONGXINANBEI
			tp[index] = v;
#else
			continue;
#endif
		}
		else if (v >= 0x35 && v <= 0x37){
#ifdef ZHONGFABAI
			tp[index] = v;
#else
			continue;
#endif
		}
		else{
			tp[index] = v;
		}
		
		if (isHu(tp, false, bao)._hutype != None){
			if (ting.find(v) == ting.end()){
				ting.insert(make_pair(v, 1));
			}
			else{
				ting.at(v)++;
			}
		}
	}
}

map<int, map<int, int>> ConfigData::chuTing(int *pai, int bao){
	map<int, map<int, int>> datas;
	int count = 0;
	map<int, int>maps;
	map<int, int>pos;
	for (int i = 0; i < 14; i++){
		int v = pai[i];
		if (v > 0){
			count++;
			if (maps.find(v) != maps.end()){
				maps.at(v)++;
			}
			else{
				maps.insert(make_pair(v,1));
			}
			if (pos.find(v) == pos.end()){
				pos.insert(make_pair(v, i));
			}
		}
	}
	
	if (count == 2 || count == 5 || count == 8 || count == 11 || count == 14){
		map<int, int>::iterator itr= maps.begin();
		for (itr; itr != maps.end();itr++){
			int v = itr->first;
			if (datas.find(v) == datas.end()){
				int temp[14];
				memcpy(temp, pai, sizeof(int)* 14);
				temp[pos.at(v)] = 0;
				map<int, int>ting;
				isTing(temp, bao,ting);
				if (!ting.empty()){
					datas.insert(make_pair(v, ting));
				}
			}
		}
	}
	return datas;
}

PaoItem ConfigData::getHandOtherPao(PaoNeed pn){
	PaoItem pi;
	//fa
	if (pn._facount >0 && pn._facount<3){
		pi._fapao = pn._facount;
	}
	else if (pn._facount >= 3){
		if (pn._baoniang == 0x36){
			pi._fapao = 10;
		}
		else{
			if (pn._facount == 3){
				pi._fapao = 3;
			}
			else{
				pi._fapao = 10;
			}
		}
	}
	//hutype
	int hutypepao = 0;
	HUTYPE hutype;
	if (pn._isgang){
		hutypepao = 5;
		if (pn._zhua){
			hutype = ZIMOGANG_TYPE;
		}
		else{
			hutype = PAOGANG_TYPE;
		}
	}
	else{
		if (pn._zhua){
			hutype = ZIMO_TYPE;
			hutypepao = 1;
		}
		else{
			hutype = PAO_TYPE;
		}
	}
	pi._hu.insert(make_pair(hutype,hutypepao));
	
	
	//pengpao
	int ppp[3] = {0};
	for (int i = 0; i < 4; i++){
		int v = pn._peng[i];
		PENGPAO pp = pn._ptype[i];
		if (v>0){
			if (pn._baoniang == v){
				if (v == 0x35 || v == 0x37){
					if (pp == MGANG_PAO){
						ppp[pp + 1] += 3;
					}
					else if (pp==AGANG_PAO){
						ppp[pp + 1] += 4;
					}
				}
				else{
					if (pp == MGANG_PAO){
						ppp[pp + 1] += 1;
					}
					else if (pp == AGANG_PAO){
						ppp[pp + 1] += 2;
					}
				}
			}
			else{
				if (v == 0x35 || v == 0x37){
					if (pp == MGANG_PAO){
						ppp[pp + 1] += 3;
					}
					else if (pp == AGANG_PAO){
						ppp[pp + 1] += 4;
					}
					else if (pp == PENG_PAO){
						ppp[pp + 1] += 1;
					}
				}
				else{
					if (pp == MGANG_PAO){
						ppp[pp + 1] += 1;
					}
					else if (pp == AGANG_PAO){
						ppp[pp + 1] += 2;
					}
				}
			}
		}
	}

	//handpao
	int count = 0;
	map<int, int>maps;
	for (int i = 0; i < 14; i++){
		int v = pn._handcards[i];
		if (v>0){
			if (maps.find(v) == maps.end()){
				maps.insert(make_pair(v, 1));
			}
			else{
				maps.at(v)++;
			}
			count++;
		}
	}
	if (count == 14){
		pi._handpao.insert(make_pair(MENQING_PAO, 1));
	}
	else{
		bool ist = true;
		for (int i = 0; i < 4; i++){
			if (pn._ptype[i] != AGANG_PAO){
				ist = false;
			}
		}
		if (ist){
			pi._handpao.insert(make_pair(MENQING_PAO, 1));
		}
	}
	map<int, int>::iterator itr = maps.begin();
	for (itr; itr != maps.end(); itr++){
		int count = itr->second;
		int v = itr->second;
		if (count == 4 && pn._hu == QIDUI){
			if (v == 0x35 || v == 0x37){
				if (pi._handpao.find(HONGBAN_PAO) != pi._handpao.end()){
					pi._handpao.at(HONGBAN_PAO) += 3;
				}
				else{
					pi._handpao.insert(make_pair(HONGBAN_PAO,3));
				}
			}
			else{
				if (pi._handpao.find(KE_PAO) != pi._handpao.end()){
					pi._handpao.at(KE_PAO) += 1;
				}
				else{
					pi._handpao.insert(make_pair(KE_PAO, 1));
				}
			}
		}
		else if (count==3){
			if (v == 0x35 || v == 0x37){
				if (pi._handpao.find(HONGBAN_PAO) != pi._handpao.end()){
					pi._handpao.at(HONGBAN_PAO) +=(pn._hucard==v?1: 2);
				}
				else{
					pi._handpao.insert(make_pair(HONGBAN_PAO, (pn._hucard == v ? 1 : 2)));
				}
			}
		}
	}
	//还有卡独
	int temp[14];
	memcpy(temp, pn._handcards, sizeof(int)* 14);
	for (int i = 0; i < 14; i++){
		if (temp[i] == pn._hucard){
			temp[i] = 0;
		}
	}
	map<int, int> vec;
	isTing(temp, pn._bao,vec);
	if (vec.size()==1&&vec.begin()->first==pn._hucard){
		//卡 独
		pi._handpao.insert(make_pair(KA_DU_PAO, 1));
	}
	return pi;
}