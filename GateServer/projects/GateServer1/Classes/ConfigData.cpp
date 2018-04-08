
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
	int64_t t = Common::getCurrentTime();
	FILE *fp = fopen("./res/ke5.csv", "r");
	char buff[200];
	if (fp == NULL){
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
				sprintf(buff, "%s\n", key.c_str());
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
			map<string, int> vec5 = pp->getDatasHuItem(ct[i]);
			int len = atoi(vv[i].c_str());
			m_lianke.insert(make_pair(len, vec5));
		}
	}
	
	
	int64_t t1 = Common::getCurrentTime();
	int64_t tt = t1 - t;
	printf("**********use time:%gs***********\n", tt / 1000.0 / 1000);

	FILE *fp1 = fopen("./res/baoke501.csv", "r");
	if (fp1 == NULL){
		for (int i = 1; i <= 4; i++){
			setLiankeBao(i);
		}
	}
	else{
		CSVDataInfo *pp = CSVDataInfo::getIns();
		string vv[] = { "5", "52", "8", "82", "11", "112", "14", "142" };
		string vr[] = { "01", "02", "03", "04" };
		//CSVSTRUCT ct[] = { CSV_BAOHU51, CSV_BAOHU52, CSV_BAOHU8, CSV_BAOHU82, CSV_BAOHU11, CSV_BAOHU112, CSV_BAOHU14, CSV_BAOHU142};
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 4; j++){
				sprintf(buff, "./res/baoke%s%s.csv", vv[i].c_str(),vr[j].c_str());
				CSVSTRUCT type = (CSVSTRUCT)(CSV_BAOHU51 + i * 4 + j);
				printf("type:%d\n",type);
				pp->openCSVFile(buff, type);
				map<string, int> vec5 = pp->getDatasHuItem(type);
				m_liankebao.insert(make_pair(atoi((vv[i]+vr[j]).c_str()), vec5));
			}
		}
	}
	
	int64_t t2 = Common::getCurrentTime();
	tt = t2 - t1;
	printf("******111use time:%gs******\n", tt / 1000.0 / 1000);



	test();
	
}

void ConfigData::test(){
	int64_t ttt = Common::getCurrentTime();
	int index = 0;
	int a[4][14] = { { 1, 2, 3, 5, 5 }, { 1, 1, 1, 17, 17, 17, 17, 3 }, { 22, 22 }, {1,23,2,3,4,5,6,7,17,18,19,33,34,35} };
	while (index < 10000){
		//int64_t t = Common::getCurrentTime();
 		HuItem item = isHu(a[index%4], true,1);
		if (item._hutype != None){
			printf("[%d].--------------success:%d\n",index, item._hutype);
		}
		//int64_t t1 = Common::getCurrentTime();
		//int64_t tt = t1 - t;
		//printf("[%d].isfit.use time:%gms\n",index, tt / 1000.0);
		index++;
	}
	int64_t t1 = Common::getCurrentTime();
	int64_t tt = t1 - ttt;
	printf("******111use time:%gs******\n", tt/1000.0/1000);
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
								maps.insert(make_pair(buff,0));
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
							maps.insert(make_pair(buff, 0));
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
					maps.insert(make_pair(buff, 0));
				}
			}
		}
		else{
			vector<int >vec = m_fengkezi.at(i);
			sort(vec.begin(), vec.end(), compare);
			for (int mm = 0; mm < vec.size(); mm++){
				sprintf(buff + mm * 2, "%02X", vec.at(mm));
			}
			maps.insert(make_pair(buff, 0));
		}
	}
	setFengKeTo(len*10+2, maps);
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
	map<string,int> maps;
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
									maps.insert(make_pair(buff, 0));
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
								maps.insert(make_pair(buff, 0));
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
						maps.insert(make_pair(buff, 0));
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
				maps.insert(make_pair(buff, 0));
			}
		}
	}
	else{
		sort(ww.begin(), ww.end(), compare);
		for (int mm = 0; mm < ww.size(); mm++){
			sprintf(buff + mm, "%c", 48 + ww.at(mm));
		}
		maps.insert(make_pair(buff, 0));
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
				HuTypeEnum type = isFit(vec, 0,huitem);
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
					vv.push_back(v % 16);
				}
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
			int baocount1 = baocount;
			setValueZero(temppai1, v, 2,baocount1);
			map<int, vector<int>> kindcards = getKindCard(temppai1);
			map<int, vector<int>>::iterator itr = kindcards.begin();
			int ii = 0;
			HuTypeEnum lasttype = None;
			for (itr; itr != kindcards.end(); itr++){
				vector<int> vec = itr->second;
				//sort(vec.begin(),vec.end(),compare);

				HuTypeEnum type = isFit(vec, baocount1, huitem);
				
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

HuTypeEnum ConfigData::isFit(vector<int>p, int baocount, HuItem &item){
	char buff[200];
	int len = p.size();
	for (int i = 0; i < len; i++){
		sprintf(buff+i,"%c",48+p.at(i));
	}
	int l = (len / 3 + (len % 3 == 0 ? 0 : 1)) * 3 + 2;
	int co = l * 10 + 2;//碰碰胡
	int baoco1 = l * 100 + baocount;
	int baoco2 = co * 100 + baocount;//碰碰胡
	map<int, map<string, int>>::iterator itr1 = m_lianke.find(co);
	if (itr1 != m_lianke.end()){
		map<string, int> maps = itr1->second;
		if (maps.find(buff) != maps.end()){
			item._hy = HEI;
			return PENGPENG;
		}
	}
	itr1 = m_lianke.find(l);
	if (itr1 != m_lianke.end()){
		map<string, int> maps = itr1->second;
		if (maps.find(buff) != maps.end()){
			item._hy = HEI;
			return PI;
		}
	}

	if (baocount>0){
		itr1 = m_liankebao.find(baoco1);
		if (itr1 != m_liankebao.end()){
			map<string, int> maps =itr1->second;
			if (maps.find(buff) != maps.end()){
				item._hy = RUAN;
				return PI;
			}
		}
		itr1 = m_liankebao.find(baoco2);
		if (itr1 != m_liankebao.end()){
			map<string, int> maps = itr1->second;
			if (maps.find(buff) != maps.end()){
				item._hy = RUAN;
				return PENGPENG;
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
	map<int, map<string, int>>::iterator itr = m_lianke.begin();
	for (itr; itr != m_lianke.end();itr++){
		int keykey = itr->first*100+i;
		map<string, int>maps1;
		map<string, int> maps = itr->second;
		map<string, int>::iterator itr1 = maps.begin();
		for (itr1; itr1 != maps.end();itr1++){
			string key = itr1->first;
			int len = key.length();
			if (i >= 1){
				for (int j = 0; j < len; j++){
					if (i >= 2){
						for (int k = j + 1; k < len; k++){
							if (i >= 3){
								for (int m = k + 1; m < len; m++){
									if (i >= 4){
										for (int n = m + 1; n < len; n++){
											string kkey = key;
											kkey.replace(j, 1, " ");
											kkey.replace(k, 1, " ");
											kkey.replace(m, 1, " ");
											kkey.replace(n, 1, " ");
											Common::replace_all(kkey, " ", "");

											maps1.insert(make_pair(kkey, 0));

										}
									}
									else{
										string kkey = key;
										kkey.replace(j, 1, " ");
										kkey.replace(k, 1, " ");
										kkey.replace(m, 1, " ");
										Common::replace_all(kkey, " ", "");
										maps1.insert(make_pair(kkey, 0));
									}
								}
							}
							else{
								string kkey = key;
								kkey.replace(j, 1, " ");
								kkey.replace(k, 1, " ");
								Common::replace_all(kkey, " ", "");
								maps1.insert(make_pair(kkey, 0));
							}
						}
					}
					else{
						string kkey = key;
						kkey.replace(j, 1, " ");
						Common::replace_all(kkey, " ", "");
						maps1.insert(make_pair(kkey, 0));
					}
				}
			}
			else{
				maps1.insert(make_pair(key, 0));
			}
		}
		m_liankebao.insert(make_pair(keykey, maps1));
		printf("*********%d**********\n", keykey);
	}
	map<int, map<string,int>>::iterator itr2 = m_liankebao.begin();
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
			sprintf(buff, "%s\n", key.c_str());
			fprintf(fp, buff);
		}
		fclose(fp);
	}
	printf("\n");
}

vector<int> ConfigData::isTing(int *pai, int bao){
	quickSort(pai, 0, 13);
	bool ismenqing = true;
	for (int i = 0; i < 14; i++){
		if (pai[i] == 0){
			ismenqing = false;
			break;
		}
	}
	vector<int> vec;
	for (int i = 0; i < g_kind;i++){
		int v = g_all_mjkind[i];
		if (isHu(pai, ismenqing, bao)._hutype!=None){
			vec.push_back(v);
		}
	}
	return vec;
}