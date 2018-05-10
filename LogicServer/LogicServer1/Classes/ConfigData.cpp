
#include "ConfigData.h"
#include "time.h"
#include "Common.h"
#include "CSVDataInfo.h"
#include "RoomControl.h"

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
	return m_cards.at(m_index++);
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
	int len = 0;
	m_predis = redis::getIns();
	char* vv= m_predis->getLastList("baoke14204");
	char buff[200];
	char buff1[200];
	if (!vv){
		setKezi();
		setShunzi();
		for (int i = 1; i <= 4; i++){
			for (int j = 0; j <= i; j++){
				init3P(i, j);
			}
		}

		map<int, map<uint64, int>>::iterator itr = m_lianke.begin();
		
		for (itr; itr != m_lianke.end(); itr++){
			int sr = itr->first;
			sprintf(buff, "ke%d", sr);
			uint64 kk = getRedisLastIndex(buff);
			map<uint64, int> vec = itr->second;
			map<uint64, int>::iterator itr1 = vec.find(kk);
			if (itr1 == vec.end()){
				itr1 = vec.begin();
			}
			else{
				itr1++;
			}
			for (itr1; itr1 != vec.end(); itr1++){
				sprintf(buff1,"%ld",itr1->first);
				m_predis->List(buff, buff1);
			}
			CLog::log("******%s***********\n",buff);
		}

		for (int i = 1; i <= 4; i++){
			setLiankeBao(i);
		}
		CLog::log("______\n");
	}
	else{
		string vv[] = {"5","52","8","82","11","112","14","142"};
		CSVSTRUCT ct[] = { CSV_HU5, CSV_HU52, CSV_HU8, CSV_HU82, CSV_HU11, CSV_HU112, CSV_HU14, CSV_HU142 };
		string vr[] = { "01", "02", "03", "04" };
		for (int i = 0; i < 8; i++){
			sprintf(buff,"ke%s",vv[i].c_str());
			CLog::log("*********type:%d********\n", ct[i]);
			map<uint64, int> vec = m_predis->getList(buff);
			if (!vec.empty()){
				int len = atoi(vv[i].c_str());
				m_lianke.insert(make_pair(len, vec));
			}
			for (int j = 0; j < 4; j++){
				sprintf(buff, "baoke%s%s", vv[i].c_str(), vr[j].c_str());
				CSVSTRUCT type = (CSVSTRUCT)(CSV_BAOHU51 + i * 4 + j);
				map<uint64, int> vec5 = m_predis->getIns()->getList(buff);
				if (!vec5.empty()){
					m_liankebao.insert(make_pair(atoi((vv[i] + vr[j]).c_str()), vec5));
				}
				CLog::log("**********type:%d**********\n", type);
			}
		}

	}
	int64_t t2 = Common::getCurrentTime();
	uint64_t tt = t2 - t;
	CLog::log("******use time:%gs******\n", tt / 1000.0 / 1000);

	RoomControl::getIns()->test();

	//test();
	
}
#define  TESTCOUNT 50
void ConfigData::test(){
	int ta[14] = {1,2,3,4,5,6,7,0x35,0x36,0x37,0x37};
	HuItem ii = isHu(ta,false,1);

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
		//听牌随机牌测试
		for (int i = 1; i <= 13; i += 3){
			initMJ();
			for (int j = 0; j < i; j++){
				b[index][j] = getMJ();
			}
		}

		index++;
	}
	int64_t ttt = Common::getCurrentTime();
	int64_t tt = ttt - t;
	CLog::log("******rand.use time:%gs******\n", tt / 1000.0 / 1000);


	index = 0;
	while (index < TESTCOUNT){
		isHu(a[index], false,1);
		index++;
	}
	int64_t t3 = Common::getCurrentTime();
	tt = t3 - ttt;
	CLog::log("******Hu.use time:%gs******\n", tt / 1000.0 / 1000);

	index = 0;
	while (index < TESTCOUNT){
		vector<int> vec = isTing(b[index], 1);
		index++;
	}
	int64_t t2 = Common::getCurrentTime();
	tt = t2 - t3;
	CLog::log("******Ting.use time:%gs******\n", tt / 1000.0 / 1000);

	index = 0;
	while (index < TESTCOUNT){
		map<int,vector<int>> vec=chuTing(a[index],1);
		index++;
	}
	int64_t t1 = Common::getCurrentTime();
	tt = t1 - t2;
	CLog::log("******ChuTing.use time:%gs******\n", tt/1000.0/1000);

	

	


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
									vector<int >vec = m_kezi.at(i);
									vector<int >vec1 = m_kezi.at(j);
									vector<int >vec2 = m_kezi.at(m);
									vector<int >vec3 = m_kezi.at(n);
									vector<int>vvvvv;
									for (int kk = 0; kk < vec.size(); kk++){
										vvvvv.push_back(vec.at(kk));
										vvvvv.push_back(vec1.at(kk));
										vvvvv.push_back(vec2.at(kk));
										vvvvv.push_back(vec3.at(kk));
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

void ConfigData::init3L(int shunnum, int index, vector<int>ww){
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
										sprintf(buff + mm, "%c", 48+vvvvv.at(mm));
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
									sprintf(buff + mm, "%c", 48 + vvvvv.at(mm));
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
							sprintf(buff + mm, "%c", 48 + vvvvv.at(mm));
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
					sprintf(buff + mm, "%c", 48 + vvvvv.at(mm));
				}
				maps.insert(make_pair(atoll(buff), 0));
			}
		}
	}
	else{
		if (!ww.empty()){
			sort(ww.begin(), ww.end(), compare);
			for (int mm = 0; mm < ww.size(); mm++){
				sprintf(buff + mm, "%c", 48 + ww.at(mm));
			}
			maps.insert(make_pair(atoll(buff), 0));
		}
	}
	
	setFengKeTo(co,maps);
}

void ConfigData::setFengKeTo(int len, map<uint64, int> maps){
	if (m_lianke.find(len) == m_lianke.end()){
		m_lianke.insert(make_pair(len, maps));
	}
	else{
		map<uint64, int>mp = m_lianke.at(len);
		map<uint64, int>::iterator itr = maps.begin();
		for (itr; itr != maps.end(); itr++){
			mp.insert(make_pair(itr->first,itr->second));
			
		}
		m_lianke.at(len) = mp;
	}
}

HuItem ConfigData::isHu(int *pai,bool ispengqing){
	bool isq = ispengqing;
	HuItem huitem;
	huitem._hutype = None;
	//统计总牌数
	int a[g_kind] = { 0 };
	int count = 0;
	int qing = -1;
	for (int i = 0; i < 14; i++){
		int v = pai[i];
		if (v>0){
			count++;
			a[v / 16 * 9 + v % 16 - 1]++;
			if (qing == -1){
				qing = v / 16;
			}
			else if (qing != v / 16){
				isq = false;
			}
		}
	}
	//统计对子数
	int duicount = 0;
	map<int,int>dui;
	for (int i = 0; i < g_kind; i++){
		int in = a[i];
		if (in >= 2){
			int v = i/9*16+i%9+1;
			dui.insert(make_pair(v, in));
			duicount += in / 2;
		}
	}

	if (duicount==0){
		return huitem;
	}
	else if (duicount == 7){
		huitem._hutype = isq?QYSQD:QIDUI;
		return huitem;
	}
	else{
		if (count == 2){
			huitem._hutype =isq?QYSPENG:PENGPENG;
			return huitem;
		}
		
		map<int, int>::iterator itr1 = dui.begin();
		for (itr1; itr1 != dui.end();itr1++){
			int temppai[14] = { 0 };
			memcpy(temppai,pai,sizeof(int)*14);
			int v = itr1->first;
			int baocount = 0;
			setValueZero(temppai,v,2,baocount);
			map<int, vector<int>> kindcards = getKindCard(temppai);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end();itr++){
				vector<int> vec = itr->second;
				//sort(vec.begin(), vec.end(), compare);
				HuTypeEnum type = isFit(vec, 0,huitem,itr->first);
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
}

map<int, vector<int>> ConfigData::getKindCard(int *temppai){
	map<int, vector<int>> kindcards;
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
				vector<int >vv = kindcards.at(kind);
				vv.push_back(v % 16);
				kindcards.at(kind) = vv;
			}
		}
	}
	return kindcards;
}

HuItem ConfigData::isHu(int *pai, bool ispengqing, int bao){
	quickSort(pai, 0, 13);
	int64_t t = Common::getCurrentTime();
	HuItem huitem = isHu(pai, ispengqing);
	if (huitem._hutype != None){
		huitem._hy = HEI;
		return huitem;
	}
	bool isq = ispengqing;
	huitem._hutype = None;
	huitem._hy = HEI;
	int temppai[14] = { 0 };
	memcpy(temppai, pai, sizeof(int)* 14);
	int baocount = 0;
	int count = 0;
	int a[g_kind] = { 0 };
	int qing = -1;
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
				a[v / 16 * 9 + v % 16 - 1]++;
				if (qing == -1){
					qing = v / 16;
				}
				else if (qing != v / 16){
					isq = false;
				}
			}
		}
	}
	if (baocount == 0){
		return huitem;
	}
	//统计对子数
	int duicount = 0;
	map<int, int>dui;
	for (int i = 0; i < g_kind; i++){
		int in = a[i];
		if (in >= 2){
			int v = i / 9 * 16 + i % 9 + 1;
			dui.insert(make_pair(v, in));
			duicount += in / 2;
		}
	}

	if (duicount == 0&&baocount==0){
		return huitem;
	}
	else if (duicount == 7){
		huitem._hutype = isq ? QYSQD : QIDUI;
		huitem._hy = HEI;
		return huitem;
	}
	else if ((baocount == 1 && duicount==6) || (baocount == 2 && duicount==5) || (baocount == 3 && duicount==4)){
		huitem._hutype = isq ? QYSQD : QIDUI;
		huitem._hy = RUAN;
	}
	else{
		if ((count == 2&&duicount==1)||(count==2&&duicount==2)){
			huitem._hutype = isq ? QYSPENG : PENGPENG;
			huitem._hy = HEI;
			return huitem;
		}
		else if ((count == 2 && duicount == 1)){
			huitem._hutype = isq ? QYSPENG : PENGPENG;
			huitem._hy = RUAN;
			return huitem;
		}
		if (baocount > 0){
			if (baocount == 1){
				for (int i = 0; i < 14; i++){
					int v = temppai[i];
					if (v>0){
						dui.insert(make_pair(v, 1));
					}
				}
			}
			else{
				for (int i = 0; i < g_kind; i++){
					dui.insert(make_pair(g_all_mjkind[i], 1));
				}
			}
		}
		
		map<int, int>::iterator itr1 = dui.begin();
		for (itr1; itr1 != dui.end(); itr1++){
			int temppai1[14] = { 0 };
			memcpy(temppai1, temppai, sizeof(int)* 14);
			int v = itr1->first;
			int baocount1 = baocount;
			setValueZero(temppai1, v, 2,baocount1);
			map<int, vector<int>> kindcards = getKindCard(temppai1);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end(); itr++){
				vector<int> vec = itr->second;
				//sort(vec.begin(),vec.end(),compare);
				HuTypeEnum type = isFit(vec, baocount1, huitem, itr->first);
				
				if (ii > 0){
					if (type > None&& type > lasttype){
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

HuTypeEnum ConfigData::isFit(vector<int>p, int baocount, HuItem &item, int kind){
	char buff[200];
	int len = p.size();
	if (len == 0){
		item._hy = HEI;
		return PENGPENG;
	}
	for (int i = 0; i < len; i++){
		sprintf(buff+i,"%c",48+p.at(i));
	}
	int l = (len / 3 + (len % 3 == 0 ? 0 : 1)) * 3 + 2;
	int co = l * 10 + 2;//碰碰胡
	int baoco1 = l * 100 + baocount;
	int baoco2 = co * 100 + baocount;//碰碰胡
	map<int, map<uint64, int>>::iterator itr1 = m_lianke.find(co);
	if (itr1 != m_lianke.end()){
		map<uint64, int> maps = itr1->second;
		if (maps.find(atoll(buff)) != maps.end()){
			item._hy = HEI;
			return PENGPENG;
		}
	}

	if (baocount > 0){
		itr1 = m_liankebao.find(baoco2);
		if (itr1 != m_liankebao.end()){
			map<uint64, int> maps = itr1->second;
			if (maps.find(atoll(buff)) != maps.end()){
				item._hy = RUAN;
				return PENGPENG;
			}
		}
	}

	if (kind < 3){
		itr1 = m_lianke.find(l);
		if (itr1 != m_lianke.end()){
			map<uint64, int> maps = itr1->second;
			if (maps.find(atoll(buff)) != maps.end()){
				item._hy = HEI;
				return PI;
			}
		}
	}
	
	if (baocount > 0){
		if (kind < 3){
			itr1 = m_liankebao.find(baoco1);
			if (itr1 != m_liankebao.end()){
				map<uint64, int> maps = itr1->second;
				if (maps.find(atoll(buff)) != maps.end()){
					item._hy = RUAN;
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
		map<uint64, int> maps = itr->second;
		map<uint64, int>::iterator itr1 = maps.begin();
		for (itr1; itr1 != maps.end();itr1++){
			uint64 key = itr1->first;
			sprintf(buff,"%ld",key);
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
											kkey.replace(j, 1, " ");
											kkey.replace(k, 1, " ");
											kkey.replace(m, 1, " ");
											kkey.replace(n, 1, " ");
											Common::replace_all(kkey, " ", "");
											if (!kkey.empty()){
												maps1.insert(make_pair(atoll(kkey.c_str()), 0));
											}

										}
									}
									else{
										string kkey = buff;
										kkey.replace(j, 1, " ");
										kkey.replace(k, 1, " ");
										kkey.replace(m, 1, " ");
										Common::replace_all(kkey, " ", "");
										if (!kkey.empty()){
											maps1.insert(make_pair(atoll(kkey.c_str()), 0));
										}
									}
								}
							}
							else{
								string kkey = buff;
								kkey.replace(j, 1, " ");
								kkey.replace(k, 1, " ");
								Common::replace_all(kkey, " ", "");
								if (!kkey.empty()){
									maps1.insert(make_pair(atoll(kkey.c_str()), 0));
								}
							}
						}
					}
					else{
						string kkey = buff;
						kkey.replace(j, 1, " ");
						Common::replace_all(kkey, " ", "");
						if (!kkey.empty()){
							maps1.insert(make_pair(atoll(kkey.c_str()), 0));
						}
					}
				}
			}
		}
		if (!maps1.empty()){
			m_liankebao.insert(make_pair(keykey, maps1));
		}
	}
	map<int, map<uint64,int>>::iterator itr2 = m_liankebao.begin();
	char buff1[200];
	for (itr2; itr2 != m_liankebao.end(); itr2++){
		int sr = itr2->first;
		sprintf(buff, "baoke%d", sr);
		uint64 kk = getRedisLastIndex(buff);
		map<uint64, int> vec = itr2->second;
		map<uint64, int>::iterator itr = vec.find(kk);
		if (itr == vec.end()){
			itr = vec.begin();
		}
		else{
			itr++;
		}
		for (itr; itr != vec.end(); itr++){
			sprintf(buff1,"%ld",itr->first);
			m_predis->List(buff, buff1);
		}
		CLog::log("*********%s**********\n", buff);
	}
	
	CLog::log("\n");
}

vector<int> ConfigData::isTing(int *pai, int bao){
	int count = 0;
	int index = -1;
	for (int i = 0; i < 14; i++){
		if (pai[i] == 0){
			index = i;
		}
		else{
			count++;
		}
	}
	vector<int> vec;
	if (count == 2 || count == 5 || count == 8 || count == 11 || count == 14){
		return vec;
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
		if (v >= 0x35 && v <= 0x37){
#ifdef ZHONGFABAI
			tp[index] = v;
#else
			continue;
#endif
		}
		else{
			tp[index] = v;
		}
		quickSort(tp, 0, 13);
		if (isHu(tp, false, bao)._hutype != None){
			vec.push_back(v);
		}
	}
	return vec;
}

map<int, vector<int>>ConfigData::chuTing(int *pai, int bao){
	map<int, vector<int>> datas;
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
				pos.insert(make_pair(v,i));
			}
		}
	}
	quickSort(pai, 0, 13);
	if (count == 2 || count == 5 || count == 8 || count == 11 || count == 14){

		map<int, int>::iterator itr= maps.begin();
		for (itr; itr != maps.end();itr++){
			int v = itr->first;
			if (v == 0){
				break;
			}
			else{
				if (itr->second < 4){
					if (datas.find(v) == datas.end()){
						int temp[14];
						memcpy(temp, pai, sizeof(int)* 14);
						temp[pos.at(v)] = 0;
						vector<int>ting = isTing(temp, bao);
						if (!ting.empty()){
							datas.insert(make_pair(v, ting));
						}
					}
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
	vector<int> vec = isTing(temp, pn._bao);
	if (vec.size()==1&&vec.at(0)==pn._hucard){
		//卡 独
		pi._handpao.insert(make_pair(KA_DU_PAO, 1));
	}
	return pi;
}