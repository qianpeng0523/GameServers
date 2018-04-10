
#include "ConfigData1.h"
#include "time.h"
#include "Common.h"
#include "CSVDataInfo.h"


bool compare1(int a, int b){
	if (a < b){
		return true;
	}
	else{
		return false;
	}
}


ConfigData1 *ConfigData1::m_ins=NULL;
ConfigData1::ConfigData1(){
	srand((unsigned int)time(NULL));
}

ConfigData1::~ConfigData1(){
	if (m_ins){
		delete m_ins;
		m_ins = NULL;
	}
}

ConfigData1 *ConfigData1::getIns(){
	if (!m_ins){
		m_ins = new ConfigData1();
		m_ins->init();
	}
	return m_ins;
}


void ConfigData1::init(){
	setKezi();
	setShunzi();
	int a[14] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4};
	isHu(a);
	printf("11111\n");
// 	int64_t t = Common::getCurrentTime();
// 	
// 	int64_t t3 = Common::getCurrentTime();
// 	int64_t tt = t3 - t;
// 	printf("******3.use time:%gs******\n", tt / 1000.0 / 1000);

}

void ConfigData1::quickSort(int *s, int l, int r)
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

void ConfigData1::setKezi(){
	char buff[100];
	for (int i = 0; i < 9;i++){
		for (int j = 0; j < 3; j++){
			int v = i+1;
			sprintf(buff,"%c",48+v);
		}
		m_kezi.insert(make_pair(buff, true));
	}
#ifdef FENG_LIAN

#else
	for (int i = 0x31; i <= 0x37; i++){
		for (int j = 0; j < 3; j++){
			int v = i + 1;
			sprintf(buff, "%c", 48 + v);
		}
		m_kezi.insert(make_pair(buff, true));
	}
#endif
}

void ConfigData1::setShunzi(){
	char buff[100];
	for (int i = 0; i < 7; i++){
		int v1 = i+1;
		int v2 = i+2;
		int v3 = i+3;
		sprintf(buff, "%c%c%c", 48 + v1, 48 + v2, 48 + v3);
		m_shunzi.insert(make_pair(buff,false));
	}
}

HuItem ConfigData1::isHu(int *pai, int bao){
	int temp[14] = {0};
	memcpy(temp,pai,sizeof(int )*14):
	HuItem item;
	item = isHu(temp);
	if (item._hutype == None || item._hutype == PI){
		int baocount = 4;
		setValueZero(temp, bao, 4, baocount);
		for (int i = 1; i < baocount; i++){
			memcpy(temp, pai, sizeof(int)* 14);
			for (int j = 0; j < g_kind; j++){
				int v = g_all_mjkind[j];
				
				
			}
		}
	}
	else{
		return item;
	}
}

void ConfigData1::setTemp(vector<int>&tp, int v1, int v2 , int v3, int v4 ){

}

HuItem ConfigData1::isHu(int *pai){
	HuItem item;
	item._hutype = None;
	item._hy = HEI;
	quickSort(pai, 0, 13);
	int temp[14];
	memcpy(temp,pai,sizeof(int)*14);
	map<int, int>duis;
	for (int i = 0; i < 14; i++){
		int v = temp[i];
		if (v>0){
			if (duis.find(v) == duis.end()){
				duis.insert(make_pair(v, 1));
			}
			else{
				duis.at(v)++;
			}
		}
	}
	int duicount = 0;
	map<int, int>::iterator itr = duis.begin();
	for (itr; itr != duis.end();itr++){
		duicount += (itr->second/2);
	}
	if (duicount == 7){
		item._hutype = QIDUI;
		return item;
	}
	int bb = 0;
	map<int, int>::iterator itr = duis.begin();
	for (itr; itr != duis.end();itr++){
		int count = itr->second;
		if (count >= 2){
			int dui = itr->first;
			setValueZero(temp, dui, 2, bb);
			quickSort(temp,0,13);

			HuTypeEnum lasttype = None;
			map<int, vector<int>> vec = getKindCard(temp);
			map<int, vector<int>>::iterator itr1 = vec.begin();
			for (itr1; itr1 != vec.end();itr1++){
				vector<int> kindcards = itr1->second;
				HuTypeEnum type = isFit(kindcards);
				if (type == None){
					return item;
				}
				else if (type==PI){
					lasttype = type;
				}
				else if (type==PENGPENG){
					if (lasttype == PI){
						lasttype = PI;
					}
					else{
						lasttype = PENGPENG;
					}
				}
			}
			if (lasttype != None){
				item._hutype= lasttype;
				return item;
			}
		}
	}
	return item;
}

HuTypeEnum ConfigData1::isFit(vector<int> p){
	HuTypeEnum type = None;
	int sz = p.size();
	if (sz%3!=0){
		return type;
	}
	char buff[100];
	//碰碰
	{
		map<string, vector<int>> vec1;
		for (int i = 0; i < sz; i++){
			int v1 = p.at(i);
			for (int j = i + 1; j < sz; j++){
				int v2 = p.at(j);
				for (int k = j + 1; k < sz; k++){
					int v3 = p.at(k);
					if ((v1 == v2&&v2 == v3)){
						vector<int>vec;
						vec.push_back(v1);
						vec.push_back(v2);
						vec.push_back(v3);
						sprintf(buff, "%c%c%c", 48 + v1, 48 + v2, 48 + v3);
						vec1.insert(make_pair(buff, vec));
					}

				}
			}
		}
		for (int i = 0; i < vec1.size(); i++){
			vector<int>tp = copyVec(p);
			type = isFit1(i, vec1, tp);
			if (type != None){
				return type;
			}
		}
	}
	//pi
	{
		map<string, vector<int>> vec1;
		for (int i = 0; i < sz; i++){
			int v1 = p.at(i);
			for (int j = i + 1; j < sz; j++){
				int v2 = p.at(j);
				for (int k = j + 1; k < sz; k++){
					int v3 = p.at(k);
					if ((v1 == v2&&v2 == v3)||(v1 + 1 == v2&&v2 + 1 == v3)){
						vector<int>vec;
						vec.push_back(v1);
						vec.push_back(v2);
						vec.push_back(v3);
						sprintf(buff, "%c%c%c", 48 + v1, 48 + v2, 48 + v3);
						vec1.insert(make_pair(buff, vec));
					}

				}
			}
		}
		for (int i = 0; i < vec1.size(); i++){
			vector<int>tp = copyVec(p);
			type = isFit1(i, vec1, tp);
			if (type != None){
				return type;
			}
		}
		
	}
	return None;
}

HuTypeEnum ConfigData1::isFit1(int index, map<string, vector<int>>&vec, vector<int>&p){
	HuTypeEnum type=None;
	map<string, vector<int>>::iterator  itr = vec.begin();
	int i = 0;
	for (itr; itr != vec.end(); itr++){
		vector<int>bv = itr->second;
		type = eraseVec(p, bv);
		if (type != None){
			type = isFit2(i, vec, p);
			if (type != None){
				return type;
			}
		}
		i++;
	}
	return type;
}

HuTypeEnum ConfigData1::isFit2(int index, map<string, vector<int>>&vec, vector<int>&p){
	HuTypeEnum type=None;
	map<string, vector<int>>::iterator  itr = vec.begin();
	int i = 0;
	for (itr; itr != vec.end();itr++){
		vector<int>bv = itr->second;
		type = eraseVec(p, bv);
		if (type != None){
			type = isFit3(i, vec, p);
			if (type != None){
				return type;
			}
		}
		
		i++;
	}
	return type;
}

HuTypeEnum ConfigData1::isFit3(int index, map<string, vector<int>>&vec, vector<int>&p){
	HuTypeEnum type=None;
	map<string, vector<int>>::iterator  itr = vec.begin();
	int i = 0;
	for (itr; itr != vec.end(); itr++){
		vector<int>bv = itr->second;
		type = eraseVec(p, bv);
		if (type != None){
			type = isFit4(i,vec,p);
			if (type != None){
				return type;
			}
		}
		i++;
	}
	return type;
}

HuTypeEnum ConfigData1::isFit4(int index, map<string, vector<int>>&vec, vector<int>&p){
	HuTypeEnum type;
	map<string, vector<int>>::iterator  itr = vec.begin();
	for (itr; itr != vec.end(); itr++){
		vector<int>bv = itr->second;
		type = eraseVec(p, bv);
		if (type!=None){
			return type;
		}
	}
	return None;
}

vector<int> ConfigData1::copyVec(vector<int> p){
	vector<int>tp;
	for (int i = 0; i < p.size(); i++){
		tp.push_back(p.at(i));
	}
	return tp;
}

HuTypeEnum ConfigData1::eraseVec(vector<int>&p, vector<int>ep){
	HuTypeEnum type = None;
	vector<int>tp = copyVec(p);
	vector<int>::iterator itr = tp.begin();
	int v1 = ep.at(0);
	int v2 = ep.at(1);
	int v3 = ep.at(2);
	if (v1 == v2&&v2 == v3){
		type = PENGPENG;
	}
	else{
		type = PI;
	}
	for (itr; itr != tp.end();){
		int v = *itr;
		if (v == v1){
			v1 = 0;
			itr= tp.erase(itr);
		}
		else if (v == v2){
			v2 = 0;
			itr = tp.erase(itr);
		}else if (v == v3){
			v3 = 0;
			itr = tp.erase(itr);
		}
		else{
			itr++;
		}
	}
	if (v1 == v2 &&v2== v3 &&v3== 0){
		p = tp;
		return type;
	}
	return None;
}

map<int, vector<int>> ConfigData1::getKindCard(int *temppai){
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


void ConfigData1::setValueZero(int *a, int v, int len, int &baocount){
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
	baocount = index;
}

vector<int> ConfigData1::isTing(int *pai, int bao){
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
		tp[index] = v;
		quickSort(tp, 0, 13);
// 		if (isHu(tp, false, bao)._hutype != None){
// 			vec.push_back(v);
// 		}
	}
	return vec;
}

map<int, vector<int>>ConfigData1::chuTing(int *pai, int bao){
	map<int, vector<int>> datas;
	int count = 0;
	for (int i = 0; i < 14; i++){
		if (pai[i] > 0){
			count++;
		}
	}
	quickSort(pai, 0, 13);
	if (count == 2 || count == 5 || count == 8 || count == 11 || count == 14){
		for (int i = 13; i >= 0; i--){
			int v = pai[i];
			if (v == 0){
				break;
			}
			else{
				if (datas.find(v) == datas.end()){
					int temp[14];
					memcpy(temp, pai, sizeof(int)* 14);
					temp[i] = 0;
					vector<int>ting = isTing(temp, bao);
					if (!ting.empty()){
						datas.insert(make_pair(v, ting));
					}
				}
			}
		}
	}
	return datas;
}

PaoItem ConfigData1::getHandOtherPao(int *a, int *peng, PENGPAO *ptype, int facount, int bao, int baoniang, int zhua, bool isgang){
	PaoItem pi;
	//fa
	if (facount >0&&facount<3){
		pi._fapao = facount;
	}
	else if (facount >= 3){
		if (baoniang == 0x36){
			pi._fapao = 10;
		}
		else{
			if (facount == 3){
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
	if (isgang){
		hutypepao = 5;
		if (zhua){
			hutype = ZIMOGANG_TYPE;
		}
		else{
			hutype = PAOGANG_TYPE;
		}
	}
	else{
		if (zhua){
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
		int v = peng[i];
		PENGPAO pp = ptype[i];
		if (v>0){
			if (baoniang == v){
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
	return pi;
}