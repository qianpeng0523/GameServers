
#include "ConfigData.h"
#include "time.h"
#include "Common.h"
#include "CSVDataInfo.h"


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

void ConfigData::init(){
	
#ifdef FENG_LIAN

#else
	for (int i = 0x31; i <= 0x37; i++){
		vector<int >vec;
		for (int j = 0; j < 3; j++){
			vec.push_back(i);
		}
		m_fengkezi.push_back(vec);
	}
	for (int i = 1; i <= 4; i++){
		setFengKezi(i);
	}
#endif
	int64_t t = Common::getCurrentTime();
	FILE *fp = fopen("./res/ke5.csv", "r");
	char buff[200];
	if (fp == NULL){
		setKezi();
		setShunzi();
		for (int i = 1; i <= 4; i++){
			for (int j = 0; j <= i; j++){
				init3P(i, j);
			}
		}

		map<int, map<string, int>>::iterator itr = m_lianke.begin();
		
		for (itr; itr != m_lianke.end(); itr++){
			int sr = itr->first;
			sprintf(buff, "./res/ke%d.csv", sr);
			FILE *fp = fopen(buff, "w+");
			fseek(fp, 0L, SEEK_END);
			map<string, int> vec = itr->second;
			map<string, int>::iterator itr1 = vec.begin();
			for (itr1; itr1 != vec.end(); itr1++){
				string key = itr1->first;
				int nn = itr1->second;
				sprintf(buff, "%s,%d\n", key.c_str(), nn);
				fprintf(fp, buff);
			}
			fclose(fp);
		}
	}
	else{
		CSVDataInfo *pp = CSVDataInfo::getIns();
		string vv[] = {"5","52","8","82","11","112","14","142"};
		CSVSTRUCT ct[] = { CSV_HU5, CSV_HU52, CSV_HU8, CSV_HU82, CSV_HU11, CSV_HU112, CSV_HU14, CSV_HU142 };
		for (int i = 0; i < 8; i++){
			sprintf(buff,"./res/ke%s.csv",vv[i].c_str());
			pp->openCSVFile(buff, ct[i]);
			std::map<int, Object *> vec5 = pp->getDatas(ct[i]);
			map<string, int> maps;
			for (int j = 0; j < vec5.size(); j++){
				CSVHuItem *item = (CSVHuItem *)vec5.at(j);
				maps.insert(make_pair(item->_key, item->_value));
			}
			m_lianke.insert(make_pair(atoi(vv[i].c_str()), maps));
			pp->eraseData(ct[i]);
		}
	}
	
	
	int64_t t1 = Common::getCurrentTime();
	int tt = t1-t;
	printf("**********use time:%dus***********\n",tt);

	FILE *fp1 = fopen("./res/baoke5.csv", "r");
	if (fp1 == NULL){
		setLiankeBao();
	}
	else{
		CSVDataInfo *pp = CSVDataInfo::getIns();
		string vv[] = { "5", "52", "8", "82", "11", "112", "14", "142" };
		CSVSTRUCT ct[] = { CSV_BAOHU5, CSV_BAOHU52, CSV_BAOHU8, CSV_BAOHU82, CSV_BAOHU11, CSV_BAOHU112, CSV_BAOHU14, CSV_BAOHU142};
		for (int i = 0; i < 8; i++){
			sprintf(buff, "./res/baoke%s.csv", vv[i].c_str());
			pp->openCSVFile(buff, ct[i]);
			std::map<int, Object *> vec5 = pp->getDatas(ct[i]);
			map<string, int> maps;
			for (int j = 0; j < vec5.size(); j++){
				CSVHuItem *item = (CSVHuItem *)vec5.at(j);
				maps.insert(make_pair(item->_key, item->_value));
			}
			m_liankebao.insert(make_pair(atoi(vv[i].c_str()), maps));
			pp->eraseData(ct[i]);
		}
	}
	
	time_t t2 = Common::getCurrentTime();
	tt = t2 - t1;
	printf("******111use time:%dus******\n", tt);



	test();
	
}

void ConfigData::test(){
	int64_t ttt = Common::getCurrentTime();
	int index = 0;
	int a[4][14] = { { 1, 2, 3, 5, 5 }, { 1, 1, 1, 17, 17, 17, 17, 3 }, { 22, 22 }, {1,23,2,3,4,5,6,7,17,18,19,33,34,35} };
	while (index < 10000){
		int64_t t = Common::getCurrentTime();
		HuItem item = isHu(a[index%4], true,1);
		if (item._hutype != None){
			//printf("--------------success:%d\n", item._hutype);
		}
		int64_t t1 = Common::getCurrentTime();
		int tt = t1 - t;
		printf("isfit.use time:%d\n", tt);
		index++;
	}
	int64_t t1 = Common::getCurrentTime();
	int tt = t1 - ttt;
	printf("******111use time:%dus******\n", tt);
	printf("1111\n");
}

void ConfigData::quickSort(int *&s, int l, int r)
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

void ConfigData::setFengKezi(int jj){
	map<string, int> maps;
	int sz = m_fengkezi.size();
	char buff[200];
	int len = jj * 3 + 2;
	for (int i = 0; i < sz; i++){
		if (jj>1){
			for (int j = i + 1; j < sz; j++){
				if (jj>2){
					for (int k = j + 1; k < sz; k++){
						if (jj>3){
							for (int m = k + 1; m < sz; m++){
								vector<int >vec = m_fengkezi.at(i);
								vector<int >vec1 = m_fengkezi.at(j);
								vector<int >vec2 = m_fengkezi.at(k);
								vector<int >vec3 = m_fengkezi.at(m);
								vector<int>vvvvv;
								for (int kk = 0; kk < vec.size(); kk++){
									vvvvv.push_back(vec.at(kk));
									vvvvv.push_back(vec1.at(kk));
									vvvvv.push_back(vec2.at(kk));
									vvvvv.push_back(vec3.at(kk));
								}
								sort(vvvvv.begin(), vvvvv.end(), compare);
								for (int mm = 0; mm < vvvvv.size(); mm++){
									sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
								}
								maps.insert(make_pair(buff, 1));
							}
						}
						else{
							vector<int >vec = m_fengkezi.at(i);
							vector<int >vec1 = m_fengkezi.at(j);
							vector<int >vec2 = m_fengkezi.at(k);
							vector<int>vvvvv;
							for (int kk = 0; kk < vec.size(); kk++){
								vvvvv.push_back(vec.at(kk));
								vvvvv.push_back(vec1.at(kk));
								vvvvv.push_back(vec2.at(kk));
							}
							sort(vvvvv.begin(), vvvvv.end(), compare);
							for (int mm = 0; mm < vvvvv.size(); mm++){
								sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
							}
							maps.insert(make_pair(buff, 1));
						}
					}
				}
				else{
					vector<int >vec = m_fengkezi.at(i);
					vector<int >vec1 = m_fengkezi.at(j);
					vector<int>vvvvv;
					for (int kk = 0; kk < vec.size(); kk++){
						vvvvv.push_back(vec.at(kk));
						vvvvv.push_back(vec1.at(kk));
					}
					sort(vvvvv.begin(), vvvvv.end(), compare);
					for (int mm = 0; mm < vvvvv.size(); mm++){
						sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
					}
					maps.insert(make_pair(buff, 1));
				}
			}
		}
		else{
			vector<int >vec = m_fengkezi.at(i);
			sort(vec.begin(), vec.end(), compare);
			for (int mm = 0; mm < vec.size(); mm++){
				sprintf(buff + mm * 2, "%02X", vec.at(mm));
			}
			maps.insert(make_pair(buff, 1));
		}
	}
	setFengKeTo(len, maps);
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
	
	
}

void ConfigData::init3L(int shunnum, int index, vector<int>ww){
	char buff[300];
	int co = index * 3 + 2;
	if (shunnum == 0){
		co = co*10+2;
	}
	int szsz = m_shunzi.size();
	map<string, int> maps;
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
										sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
									}
									maps.insert(make_pair(buff, 1));
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
									sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
								}
								maps.insert(make_pair(buff, 1));
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
							sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
						}
						maps.insert(make_pair(buff, 1));
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
					sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
				}
				maps.insert(make_pair(buff, 1));
			}
		}
	}
	else{
		vector<int>vvvvv;
		for (int jj = 0; jj < ww.size(); jj++){
			vvvvv.push_back(ww.at(jj));
		}
		sort(vvvvv.begin(), vvvvv.end(), compare);
		for (int mm = 0; mm < vvvvv.size(); mm++){
			sprintf(buff + mm * 2, "%02X", vvvvv.at(mm));
		}
		maps.insert(make_pair(buff, 1));
	}
	
	setFengKeTo(co,maps);
}

void ConfigData::setFengKeTo(int len, map<string, int> maps){
	if (m_lianke.find(len) == m_lianke.end()){
		m_lianke.insert(make_pair(len, maps));
	}
	else{
		map<string, int>mp = m_lianke.at(len);
		map<string, int>::iterator itr = maps.begin();
		for (itr; itr != maps.end(); itr++){
			mp.insert(make_pair(itr->first, itr->second));
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
			setValueZero(temppai,v,2);
			map<int, vector<int>> kindcards = getKindCard(temppai);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end();itr++){
				vector<int> vec = itr->second;
				sort(vec.begin(), vec.end(), compare);
				HuTypeEnum type = isFit(vec, false,huitem);
				if (ii > 0){
					if (lasttype != type){
						lasttype = PI;
					}
					else{
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
				if (kind == 3){
#ifdef FENG_LIAN
					vv.push_back(v % 16);
#else
					vv.push_back(v);
#endif
				}
				else{
					vv.push_back(v % 16);
				}
				kindcards.insert(make_pair(kind, vv));
			}
			else{
				vector<int >vv = kindcards.at(kind);
				if (kind == 3){
#ifdef FENG_LIAN
					vv.push_back(v % 16);
#else
					vv.push_back(v);
#endif
				}
				else{
					vv.push_back(v);
				}
				kindcards.at(kind) = vv;
			}
		}
	}
	return kindcards;
}

HuItem ConfigData::isHu(int *pai, bool ispengqing, int bao){
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
		if (baocount > 0&&dui.size()==0){
			for (int i = 0; i < 14; i++){
				int v = temppai[i];
				if (v>0){
					dui.insert(make_pair(v, 1));
				}
			}
		}
		map<int, int>::iterator itr1 = dui.begin();
		for (itr1; itr1 != dui.end(); itr1++){
			int temppai1[14] = { 0 };
			memcpy(temppai1, temppai, sizeof(int)* 14);
			int v = itr1->first;
			setValueZero(temppai1, v, 2);
			map<int, vector<int>> kindcards = getKindCard(temppai1);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end(); itr++){
				vector<int> vec = itr->second;
				sort(vec.begin(),vec.end(),compare);
				HuTypeEnum type = isFit(vec, true, huitem);
				if (ii > 0){
					if (lasttype != type){
						lasttype = PI;
					}
					else{
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

HuTypeEnum ConfigData::isFit(vector<int>p, bool isbao, HuItem &item){
	char buff[200];
	int len = p.size();
	if (len == 1){
		sprintf(buff, "%02X", p.at(0));
	}
	else if (len == 2){
		sprintf(buff, "%02X%02X", p.at(0), p.at(1));
	}
	else if (len == 3){
		sprintf(buff, "%02X%02X%02X", p.at(0), p.at(1), p.at(2));
	}
	else if (len == 4){
		sprintf(buff, "%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3));
	}
	else if (len == 5){
		sprintf(buff, "%02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4));
	}
	else if (len == 6){
		sprintf(buff, "%02X%02X%02X%02X02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5));
	}
	else if (len == 7){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6));
	}
	else if (len == 8){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7));
	}
	else if (len == 9){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7), p.at(8));
	}
	else if (len == 10){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7), p.at(8), p.at(9));
	}
	else if (len == 11){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7), p.at(8), p.at(9), p.at(10));
	}
	else if (len == 12){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7), p.at(8), p.at(9), p.at(10), p.at(11));
	}
	else if (len == 13){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X%02X", p.at(0), p.at(1), p.at(2), p.at(3), p.at(4), p.at(5), p.at(6), p.at(7), p.at(8), p.at(9), p.at(10), p.at(11), p.at(12));
	}
	int l = len / 3 + (len % 3 == 0 ? 0 : 1);
	len = l * 3 + 2;
	int co = len * 10 + 2;
	if (!isbao){
		item._hy = HEI;
		if (m_lianke.find(co) != m_lianke.end()){
			map<string, int> maps = m_lianke.at(co);
			if (maps.find(buff) != maps.end()){
				return PENGPENG;
			}
		}
		if (m_lianke.find(len) != m_lianke.end()){
			map<string, int> maps = m_lianke.at(len);
			if (maps.find(buff) != maps.end()){
				return PI;
			}
		}
	}
	else{
		if (m_liankebao.find(co) != m_liankebao.end()){
			map<string, int> maps = m_liankebao.at(co);
			if (maps.find(buff) != maps.end()){
				if (strlen(buff) / 2 == len - 2){
					item._hy = HEI;
				}
				else{
					item._hy = RUAN;
				}
				return PENGPENG;
			}
		}
		if (m_liankebao.find(len) != m_liankebao.end()){
			map<string, int> maps = m_liankebao.at(len);
			if (maps.find(buff) != maps.end()){
				if (strlen(buff) / 2 == len - 2){
					item._hy = HEI;
				}
				else{
					item._hy = RUAN;
				}
				return PI;
			}
		}
	}
	return None;
}

HuTypeEnum ConfigData::isFit(int *p, int len,bool isbao,HuItem &item){
	char buff[200];
	if (len == 1){
		sprintf(buff, "%02X", p[0]);
	}
	else if (len == 2){
		sprintf(buff, "%02X%02X", p[0], p[1]);
	}
	else if (len == 3){
		sprintf(buff, "%02X%02X%02X", p[0], p[1], p[2]);
	}
	else if (len == 4){
		sprintf(buff, "%02X%02X%02X%02X", p[0], p[1], p[2], p[3]);
	}
	else if (len == 5){
		sprintf(buff, "%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4]);
	}
	else if (len == 6){
		sprintf(buff, "%02X%02X%02X%02X02X%02X", p[0], p[1], p[2], p[3], p[4], p[5]);
	}
	else if (len == 7){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
	}
	else if (len == 8){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
	}
	else if (len == 9){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
	}
	else if (len == 10){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
	}
	else if (len == 11){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10]);
	}
	else if (len == 12){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11]);
	}
	else if (len == 13){
		sprintf(buff, "%02X%02X%02X%02X02X%02X%02X%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]);
	}
	int l = len / 3 + (len % 3 == 0 ? 0 : 1);
	len = l*3+2;
	int co = len*10+2;
	if (!isbao){
		item._hy = HEI;
		if (m_lianke.find(co) != m_lianke.end()){
			map<string, int> maps = m_lianke.at(co);
			if (maps.find(buff) != maps.end()){
				return PENGPENG;
			}
		}
		if (m_lianke.find(len) != m_lianke.end()){
			map<string, int> maps = m_lianke.at(len);
			if (maps.find(buff) != maps.end()){
				return PI;
			}
		}
	}
	else{
		if (m_liankebao.find(co) != m_liankebao.end()){
			map<string, int> maps = m_liankebao.at(co);
			if (maps.find(buff) != maps.end()){
				if (strlen(buff) / 2 == len - 2){
					item._hy = HEI;
				}
				else{
					item._hy = RUAN;
				}
				return PENGPENG;
			}
		}
		if (m_liankebao.find(len) != m_liankebao.end()){
			map<string, int> maps = m_liankebao.at(len);
			if (maps.find(buff) != maps.end()){
				if (strlen(buff)/2 == len-2){
					item._hy = HEI;
				}
				else{
					item._hy = RUAN;
				}
				return PI;
			}
		}
	}
	return None;
}

void ConfigData::setValueZero(int *a, int v,int len){
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
}

void ConfigData::setLiankeBao(){
	map<int, map<string, int>>::iterator itr = m_lianke.begin();
	for (itr; itr != m_lianke.end();itr++){
		int keykey = itr->first;
		map<string, int>maps1;
		map<string, int> maps = itr->second;
		map<string, int>::iterator itr1 = maps.begin();
		for (itr1; itr1 != maps.end();itr1++){
			string key = itr1->first;
			int len = key.length();
			for (int i = 0; i <= 4; i++){
				if (i >= 1){
					for (int j = 0; j < len; j += 2){
						if (i >= 2){
							for (int k = j + 2; k < len; k += 2){
								if (i >= 3){
									for (int m = k + 2; m < len; m += 2){
										if (i >= 4){
											for (int n = m + 2; n < len; n += 2){
												string kkey = key;
												kkey.replace(j, 2, "  ");
												kkey.replace(k, 2, "  ");
												kkey.replace(m, 2, "  ");
												kkey.replace(n, 2, "  ");
												Common::replace_all(kkey, "  ", "");

												maps1.insert(make_pair(kkey, 1));

											}
										}
										else{
											string kkey = key;
											kkey.replace(j, 2, "  ");
											kkey.replace(k, 2, "  ");
											kkey.replace(m, 2, "  ");
											Common::replace_all(kkey, "  ", "");
											maps1.insert(make_pair(kkey, 1));
										}
									}
								}
								else{
									string kkey = key;
									kkey.replace(j, 2, "  ");
									kkey.replace(k, 2, "  ");
									Common::replace_all(kkey, "  ", "");
									maps1.insert(make_pair(kkey, 1));
								}
							}
						}
						else{
							string kkey = key;
							kkey.replace(j, 2, "  ");
							Common::replace_all(kkey, "  ", "");
							maps1.insert(make_pair(kkey, 1));
						}
					}
				}
				else{
					maps1.insert(make_pair(key, 1));
				}
			}
		}
		m_liankebao.insert(make_pair(keykey, maps1));
		printf("*********%d**********\n", keykey);
	}
	map<int, map<string, int>>::iterator itr2 = m_liankebao.begin();
	char buff[200];
	for (itr2; itr2 != m_liankebao.end(); itr2++){
		int sr = itr2->first;
		sprintf(buff, "./res/baoke%d.csv", sr);
		FILE *fp = fopen(buff, "w+");
		fseek(fp, 0L, SEEK_END);
		map<string, int> vec = itr2->second;
		map<string, int>::iterator itr1 = vec.begin();
		for (itr1; itr1 != vec.end(); itr1++){
			string key = itr1->first;
			int nn = itr1->second;
			sprintf(buff, "%s,%d\n", key.c_str(), nn);
			fprintf(fp, buff);
		}
		fclose(fp);
	}
	printf("\n");
}