
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
	vector<int> v1;
	for (int i = 0; i < 7;i++){
		int a1 = i + 1;
		int a2 = i + 2;
		int a3 = i + 3;
		v1.push_back(a1*100+a2*10+a3);
	}
	vector<int> v2;
	for (int i = 0; i < 9;i++){
		int a = i + 1;
		v2.push_back(a * 100 + a * 10 + a);
	}

	//5张
	vector<_int64> hu1;
	for (int i = 0; i < v1.size(); i++){
		hu1.push_back(v1.at(i));
	}

	vector<_int64> hu2;
	for (int i = 0; i < v2.size(); i++){
		hu1.push_back(v2.at(i));
	}

	//8张
	
	for (int i = 0; i < v1.size(); i++){
		_int64 a1 = v1.at(i);
		for (int j = i; j < v1.size();j++){
			_int64 a2 = v1.at(j);
			_int64 t = a1 * 1000 + a2;
			hu1.push_back(t);
		}
	}

	
	for (int i = 0; i < v2.size(); i++){
		_int64 a1 = v2.at(i);
		for (int j = i + 1; j < v2.size(); j++){
			_int64 a2 = v2.at(j);
			_int64 t = a1 * 1000 + a2;
			hu2.push_back(t);
		}
	}

	//11张
	
	for (int i = 0; i < v1.size(); i++){
		_int64 a1 = v1.at(i);
		for (int j = i; j < v1.size(); j++){
			_int64 a2 = v1.at(j);
			for (int m = j; m < v1.size(); m++){
				_int64 a3 = v1.at(m);
				_int64 t = a1 * 1000*1000 + a2*1000+a3;
				hu1.push_back(t);
			}
		}
	}

	for (int i = 0; i < v2.size(); i++){
		_int64 a1 = v2.at(i);
		for (int j = i + 1; j < v2.size(); j++){
			_int64 a2 = v2.at(j);
			for (int m = j + 1; m < v2.size(); m++){
				_int64 a3 = v2.at(m);
				_int64 t = a1 * 1000 * 1000 + a2 * 1000 + a3;
				hu2.push_back(t);
			}
		}
	}

	//14张
	
	for (int i = 0; i < v1.size(); i++){
		_int64 a1 = v1.at(i);
		for (int j = i; j < v1.size(); j++){
			_int64 a2 = v1.at(j);
			for (int m = j; m < v1.size(); m++){
				_int64 a3 = v1.at(m);
				for (int n = m; n < v1.size(); n++){
					_int64 a4 = v1.at(n);
					_int64 t = a1 * 1000 * 1000*1000 + a2 * 1000*1000 + a3*1000+a4;
					hu1.push_back(t);
				}
			}
		}
	}

	
	for (int i = 0; i < v2.size(); i++){
		_int64 a1 = v2.at(i);
		for (int j = i+1; j < v2.size(); j++){
			_int64 a2 = v2.at(j);
			for (int m = j+1; m < v2.size(); m++){
				_int64 a3 = v2.at(m);
				for (int n = m+1; n < v2.size(); n++){
					_int64 a4 = v2.at(n);
					_int64 t = a1 * 1000 * 1000 * 1000 + a2 * 1000 * 1000 + a3 * 1000 + a4;
					hu2.push_back(t);
				}
			}
		}
	}
	
	for (int i = 0; i < hu1.size(); i++){
		_int64 a2 = hu1.at(i);
		char buff2[30];
		sprintf(buff2, "%lld", a2);
		map<char, int> v2;
		for (int i = 0; i < strlen(buff2); i++){
			char a = buff2[i]-'0';
			if (v2.find(a) == v2.end()){
				v2.insert(make_pair(a, 1));
			}
			else{
				v2.at(a)++;
			}
		}
		vector<map<char, int>> vv;
		if (m_hus.find(1) == m_hus.end()){
			vv.push_back(v2);
			m_hus.insert(make_pair(1, vv));
		}
		else{
			vv = m_hus.at(1);
			vv.push_back(v2);
			m_hus.at(1) = vv;
		}
	}
	
	for (int i = 0; i < hu2.size(); i++){
		_int64 a2 = hu2.at(i);
		char buff2[30];
		sprintf(buff2, "%lld", a2);
		map<char, int> v2;
		for (int i = 0; i < strlen(buff2); i++){
			char a = buff2[i]-'0';
			if (v2.find(a) == v2.end()){
				v2.insert(make_pair(a, 1));
			}
			else{
				v2.at(a)++;
			}
		}
		vector<map<char, int>> vv;
		if (m_hus.find(2) == m_hus.end()){
			vv.push_back(v2);
			m_hus.insert(make_pair(2, vv));
		}
		else{
			vv = m_hus.at(2);
			vv.push_back(v2);
			m_hus.at(2) = vv;
		}
	}
	test();
}

#define  TESTCOUNT1 50

void ConfigData1::test(){
	/*
	int a[TESTCOUNT1][14] = { 0 };
	int index = 0;
	int64_t t = Common::getCurrentTime();
	int num[] = { 2, 5, 8, 11, 14 };
	while (index < TESTCOUNT1){
		//胡牌和出牌听牌测试
		ConfigData::getIns()->initMJ();
		for (int j = 0; j < num[rand() % 5]; j++){
			a[index][j] = ConfigData::getIns()->getMJ();
		}

		ConfigData::getIns()->quickSort(a[index], 0, 14);
		index++;
	}
	int64_t ttt = Common::getCurrentTime();
	int64_t tt = ttt - t;
	CLog::log("******rand.use time:%fs******\n", tt / 1000.0 / 1000);


	index = 0;
	string con;
	while (index < TESTCOUNT1){
		HuItem item = isHu(a[index], 0x15);
		HuTypeEnum type = item._hutype;
		switch (type)
		{
		case None:
			con = "none";
			break;
		case PI:
			con = "PI";
			break;
		case PENGPENG:
			con = "PENGPENG";
			break;
		case QIDUI:
			con = "QIDUI";
			break;
		case QINGYISE:
			con = "QINGYISE";
			break;
		case QYSPENG:
			con = "QYSPENG";
			break;
		case QYSQD:
			con = "QYSQD";
			break;
		default:
			break;
		}
		CLog::log("%d.[0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d,0x%02d]:[%s]--[%s]", a[index][0], a[index][1], a[index][2], 
			a[index][3], a[index][4], a[index][5], a[index][6], a[index][7], a[index][8], a[index][9], a[index][10],
			a[index][11], a[index][12], a[index][13],
			index+1 ,con.c_str(),item._hy==HEI?"HEI":"RUAN");
		index++;
	}
	int64_t t3 = Common::getCurrentTime();
	tt = t3 - ttt;
	CLog::log("******Hu.use time:%fs******\n", tt / 1000.0 / 1000);
	*/
	int pai[14] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x37, 0x41, 0x41, 0x01 };
	HuItem item = isHu(pai,0x15);
	printf("\n");
}

_int64 ConfigData1::getLeftValue(_int64 a1, map<char, int> v2, vector<int> &leftvs){
	char buff1[30];
	sprintf(buff1,"%lld",a1);
	map<char, int> v1;
	for (int i = 0; i < strlen(buff1); i++){
		char a = buff1[i]-'0';
		if (v1.find(a) == v1.end()){
			v1.insert(make_pair(a, 1));
		}
		else{
			v1.at(a)++;
		}
	}
	
	auto itr = v2.begin();
	for (itr; itr != v2.end();){
		char a = itr->first;
		int count = itr->second;
		if (v1.find(a) != v1.end()){
			int count1 = v1.at(a);
			int cco = (count1>count ? count : count1);;
			v1.at(a) -= cco;
			if (v1.at(a) <= 0){
				v1.erase(v1.find(a));
			}
			itr->second -= cco;
			if (itr->second == 0){
				itr=v2.erase(itr);
			}
			else{
				itr++;
			}
		}
		else{
			itr++;
		}
	}
	_int64 a=0;
	auto itr1 = v1.begin();
	for (itr1; itr1 != v1.end();itr1++){
		int co = itr1->second;
		int va = itr1->first;
		for (int j = 0; j < co; j++){
			a = a*10+va;
			leftvs.push_back(va);
		}
	}

	auto itr2 = v2.begin();
	for (itr2; itr2 != v2.end(); itr2++){
		int co = itr2->second;
		int va = itr2->first;
		for (int j = 0; j < co; j++){
			a = a * 10 + va;
			leftvs.push_back(va);
		}
	}

	return a;
}

vector<_int64> ConfigData1::getLeftValue(_int64 a, int baocount, vector<vector<int>> &leftvs){
	auto itr = m_hus.begin();
	char buff[20];
	vector<_int64> vec;
	for (itr; itr != m_hus.end(); itr++){
		for (int i = 0; i<itr->second.size(); i++){
			vector<int> vv;
			_int64 aa = getLeftValue(a, itr->second.at(i), vv);
			if (aa > 0){
				sprintf(buff, "%lld", aa);
				int ll = strlen(buff);
				//这里有问题
				if (ll <= baocount){
					vec.push_back(aa);
					leftvs.push_back(vv);
				}
			}
		}
	}
	return vec;
}

HuItem ConfigData1::isHu(int *pai, int baovalue){
	HuItem item=isPPH(pai,baovalue);
	if (item._hutype == QIDUI){
		HuItem item1 = isQYS(pai,baovalue);
		if (item1._hutype == QINGYISE){
			item._hutype = QYSQD;
			if (item1._hy == HEI&&item._hy == HEI){
				item._hy = HEI;
			}
			else{
				item._hy = RUAN;
			}
		}
		return item;
	}
	else if (item._hutype == PENGPENG){
		HuItem item1 = isQYS(pai, baovalue);
		if (item1._hutype == QINGYISE){
			item._hutype = QYSPENG;
			if (item1._hy == HEI&&item._hy == HEI){
				item._hy = HEI;
			}
			else{
				item._hy = RUAN;
			}
		}
		return item;
	}
	else{
		item = isQYS(pai, baovalue);
	}
	
	map<int, int> duis;
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		if (va>0&&va!=baovalue){
			if (duis.find(va) == duis.end()){
				duis.insert(make_pair(va,1));
			}
			else{
				duis.at(va)++;
			}
		}
	}
	map<int, map<int, int>>dduis;
	auto itr = duis.begin();
	for (itr; itr != duis.end(); itr++){
		int vava = itr->first;
		int vcou = itr->second;
		map<int, int>mms;
		if (dduis.find(vcou) != dduis.end()){
			mms = dduis.at(vcou);
			if (mms.find(vava) == mms.end()){
				mms.insert(make_pair(vava,1));
			}
			dduis.at(vcou) = mms;
		}
		else{
			mms.insert(make_pair(vava, 1));
			dduis.insert(make_pair(vcou, mms));
		}
	}

	auto itr1 = dduis.crbegin();
	for (itr1; itr1 != dduis.crend(); itr1++){
		map<int, int> mms = itr1->second;
		auto itr2 = mms.begin();
		int vcou = itr1->first;
		for (itr2; itr2 != mms.end(); itr2++){
			int vava = itr2->first;

			int temp[14];
			memcpy(temp, pai, sizeof(int)* 14);
			bool ishei = true;
			int baocount = 0;
			for (int i = 0; i < 14; i++){
				int v = pai[i];
				if (v>0){
					if (v == baovalue){
						baocount++;
					}
				}
			}

			if (vcou >= 2){
				int ncount = 0;
				for (int i = 0; i < 14; i++){
					if (temp[i] == vava){
						temp[i] = 0;
						ncount++;
						if (ncount == 2){
							break;
						}
					}
				}
			}
			else if (baocount>0){
				ishei = false;
				baocount -= 1;
				int ncount[2] = { 0 };
				for (int i = 0; i < 14; i++){
					if (ncount[0] == 0 && temp[i] == vava){
						temp[i] = 0;
						ncount[0]++;
						if (ncount[1]>0){
							break;
						}
					}
					else if (ncount[1] == 0 && temp[i] == baovalue){
						temp[i] = 0;
						ncount[1]++;
						if (ncount[0] > 0){
							break;
						}
					}
				}
			}
			_int64 a[5] = { 0 };
			for (int i = 0; i < 14; i++){
				int v = temp[i];
				if (v>0){
					if (v != baovalue){
						int type = v / 16;
						int va = v % 16;
						a[type] = a[type] * 10 + va;
					}
				}
			}
			int baotype = baovalue / 16;
			int baov = baovalue % 16;

			int len = 0;
			char buff[20];
			vector<vector<vector<int>>>bbs;
			for (int i = 0; i < 5; i++){
				vector<vector<int>> vv;
				if (a[i] == 0){
					bbs.push_back(vv);
				}
				else{
					vector<_int64> aa = getLeftValue(a[i], baocount, vv);
					bbs.push_back(vv);
				}
				int sz = 0;
				if (vv.empty()){
					sz = 0;
				}
				else{
					for (int j = 0; j<vv.size(); j++){
						sz += vv.at(j).size();
					}
				}
				if (ishei){
					if (i != baotype&&sz>0){
						ishei = false;
					}
					else if (i == baotype&&sz > 0&&i<vv.size()){
						for (int j = 0; j < vv.at(i).size(); j++){
							int vaa = vv.at(i).at(j);
							if (vaa != baov){
								ishei = false;
								break;
							}
						}
					}
				}
			}

			vector<vector<int>> vv1 = bbs.at(0);
			vector<vector<int>> vv2 = bbs.at(1);
			vector<vector<int>> vv3 = bbs.at(2);
			vector<vector<int>> vv4 = bbs.at(3);
			vector<vector<int>> vv5 = bbs.at(4);
			bool ist = false;
			if (vv5.empty() && vv4.empty() && vv3.empty() && vv2.empty() && vv1.empty()){
				if (item._hutype == QINGYISE){
					item._hutype = QINGYISE;
				}
				else{
					item._hutype = PI;
				}
				item._hy = (item._hy&&ishei) ? HEI : RUAN;
				return item;
			}
			else{
				int l1 = 0;
				for (int i = 0; i < vv1.size(); i++){
					if (ist){
						break;
					}
					l1 = vv1.at(i).size();
					if (l1>baocount){
						continue;
					}
					else{
						if (vv5.empty() && vv4.empty() && vv3.empty() && vv2.empty()){
							ist = true;
							break;
						}
					}
				}
				for (int j = 0; j < vv2.size(); j++){
					if (ist){
						break;
					}
					l1 += vv2.at(j).size();
					if (l1>baocount){
						continue;
					}
					else{
						if (vv5.empty() && vv4.empty() && vv3.empty()){
							ist = true;
							break;
						}
					}
				}
				for (int k = 0; k < vv3.size(); k++){
					if (ist){
						break;
					}
					l1 += vv3.at(k).size();
					if (l1>baocount){
						continue;
					}
					else{
						if (vv5.empty() && vv4.empty()){
							ist = true;
							break;
						}
					}
				}
				for (int m = 0; m < vv4.size(); m++){
					if (ist){
						break;
					}
					l1 += vv4.at(m).size();
					if (l1>baocount){
						continue;
					}
					else{
						if (vv5.empty()){
							ist = true;
							break;
						}
					}
				}
				for (int n = 0; n < vv5.size(); n++){
					l1 += vv5.at(n).size();
					if (l1 >baocount){
						continue;
					}
					else{
						ist = true;
						break;
					}
				}
				if (ist){
					if (item._hy == QINGYISE){
						item._hutype = QINGYISE;
						item._hy = (ishei&&item._hy)?HEI:RUAN;
					}
					else{
						item._hutype = PI;
						item._hy = ishei ? HEI : RUAN;
					}
					
					return item;
				}
			}
		}
	}
	
	item._hutype = None;
	return item;
}

HuItem ConfigData1::isQidui(int *pai, int baovalue){
	HuItem item;
	map<int, int>mps;
	int cou = 0;
	int baocount = 0;
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		if (va>0){
			if (baovalue != va){
				if (mps.find(va) != mps.end()){
					mps.at(va)++;
				}
				else{
					mps.insert(make_pair(va, 1));
				}
			}
			else{
				baocount++;
			}
			cou++;
		}
	}
	if (cou < 14){
		item._hutype = None;
		return item;
	}
	auto itr = mps.begin();
	int count = 0;
	
	item._hy = HEI;
	for (itr; itr != mps.end();){
		int co = itr->second;
		if (co == 2||co==4){
			count+=co/2;
			itr = mps.erase(itr);
		}
		else if (co == 3){
			count++;
			itr->second = itr->second-2;
			itr++;
		}
		else{
			itr++;
		}
	}
	if (7 - count <= baocount){
		item._hutype = QIDUI;
		itr = mps.begin();
		for (itr; itr != mps.end(); itr++){
			int va = itr->first;
			if (va != baovalue){
				item._hy = RUAN;
				break;
			}
		}
	}
	else{
		item._hutype = None;
	}
	return item;
}

HuItem ConfigData1::isPPH(int *pai, int baovalue){
	HuItem item = isQidui(pai,baovalue);
	if (item._hutype == None){
		map<int, int>mps;
		int baocount = 0;
		for (int i = 0; i < 14; i++){
			int va = pai[i];
			if (va>0){
				if (baovalue != va){
					if (mps.find(va) != mps.end()){
						mps.at(va)++;
					}
					else{
						mps.insert(make_pair(va, 1));
					}
				}
				else{
					baocount++;
				}
			}
		}
		//怎么判断是碰碰胡
		auto itr = mps.begin();
		int slen = 0;
		bool havedui = false;
		for (itr; itr != mps.end();itr++){
			int cou = itr->second;
			if (cou == 4){
				return item;
			}
			else if (cou < 3){
				slen += 1;
				if (cou == 2){
					havedui = true;
				}
			}
		}
		if (slen - (havedui?1:0) <= baocount){
			item._hutype = PENGPENG;
			if (slen==1){
				item._hy = HEI;
			}
			else{
				item._hy = RUAN;
			}
			return item;
		}
		return item;
	}
	else if (item._hutype == QIDUI&&item._hy == RUAN){
		map<int, int>mps;
		for (int i = 0; i < 14; i++){
			int va = pai[i];
			if (va>0){
				if (mps.find(va) != mps.end()){
					mps.at(va)++;
				}
				else{
					mps.insert(make_pair(va,1));
				}
			}
		}
		auto itr = mps.begin();
		int duicount = 0;
		for (itr; itr != mps.end();itr++){
			if (itr->second < 2){
				//如果有单独一张牌的就说明不是黑碰碰胡
				return item;
			}
			else if (itr->second == 2){
				duicount++;
				if (duicount>1){
					//如果有对子有一个以上的就说明不是黑碰碰胡
					return item;
				}
			}
		}
		item._hutype = PENGPENG;
		item._hy = HEI;
		return item;
	}
	else{
		return item;
	}
}

HuItem ConfigData1::isQYS(int *pai, int baovalue){
	map<int, int>types;
	map<int, int >pais;
	for (int i = 0; i < 14;i++){
		int va = pai[i];
		if (va>0){
			if (va != baovalue){
				if (types.find(va / 16) == types.end()){
					if (types.empty()){
						types.insert(make_pair(va / 16, 1));
					}
					else{
						HuItem item;
						item._hutype = None;
						return item;
					}
				}
			}
			else{
				if (pais.find(va) == pais.end()){
					pais.insert(make_pair(va, 1));
				}
			}
		}
	}
	HuItem item;
	item._hutype = QINGYISE;
	item._hy = HEI;
	if (pais.find(baovalue) != pais.end()){
		if (types.find(baovalue / 16) != types.end()){
			item._hy = HEI;
		}
		else{
			item._hy = RUAN;
		}
	}
	return item;
}