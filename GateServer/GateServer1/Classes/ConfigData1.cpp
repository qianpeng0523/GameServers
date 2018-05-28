
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
			char a = buff2[i];
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
			char a = buff2[i];
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
	printf("\n");
}

_int64 ConfigData1::getLeftValue(_int64 a1, map<char, int> v2, vector<int> &leftvs){
	char buff1[30];
	sprintf(buff1,"%lld",a1);
	map<char, int> v1;
	for (int i = 0; i < strlen(buff1); i++){
		char a = buff1[i];
		if (v1.find(a) == v1.end()){
			v1.insert(make_pair(a, 1));
		}
		else{
			v1.at(a)++;
		}
	}
	
	auto itr = v2.begin();
	for (itr; itr != v2.end();itr++){
		char a = itr->first;
		int count = itr->second;
		if (v1.find(a) != v1.end()){
			v1.at(a) -= count;
			if (v1.at(a) <= 0){
				v1.erase(v1.find(a));
			}
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
	return a;
}

vector<_int64> ConfigData1::getLtftValue(_int64 a, int baocount, vector<vector<int>> &leftvs){
	auto itr = m_hus.begin();
	char buff[20];
	vector<_int64> vec;
	for (itr; itr != m_hus.end(); itr++){
		vector<int> vv;
		_int64 aa = getLeftValue(a, itr->second,vv);
		if (aa > 0){
			sprintf(buff, "%lld", aa);
			int ll = strlen(buff);
			if (ll <= baocount){
				vec.push_back(aa);
				leftvs.push_back(vv);
			}
		}
	}
	return vec;
}

HuItem ConfigData1::isHu(int *pai, int baovalue){
	_int64 a[5] = { 0 };
	int baocount = 0;
	for (int i = 0; i < 14; i++){
		int v = pai[i];
		if (v>0){
			if (v == baovalue){
				baocount++;
			}
			else{
				int type = v / 16;
				int va = v % 16;
				a[type] = a[type] * 10 + va;
			}
		}
	}
	int baotype = baovalue / 16;
	int baov = baovalue % 16;
	bool ishei = true;
	int len = 0;
	char buff[20];
	vector<vector<vector<int>>>bbs;
	for (int i = 0; i < 5; i++){
		vector<vector<int>> vv;
		vector<_int64> aa = getLeftValue(a[i], baocount,vv);
		bbs.push_back(vv);
		int sz = vv.at(i).size();
		if (ishei){
			if (i != baotype&&sz>0){
				ishei = false;
			}
			else if (i == baotype&&sz > 0){
				for (int j = 0; j < vv.at(i); j++){
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
	for (int i = 0; i < vv1.size(); i++){
		if (ist){
			break;
		}
		int l1 = vv1.at(i).size();
		if (l1>baocount){
			continue;
		}
		else{
			if (vv5.empty() && vv4.empty() && vv3.empty() && vv2.empty()){
				ist = true;
				break;
			}
		}
		for (int j = 0; j < vv2.size(); j++){
			if (ist){
				break;
			}
			int l2 = vv2.at(j).size();
			if (l1 + l2>baocount){
				continue;
			}
			else{
				if (vv5.empty() && vv4.empty() && vv3.empty()){
					ist = true;
					break;
				}
			}
			for (int k = 0; k < vv3.size(); k++){
				if (ist){
					break;
				}
				int l3 = vv3.at(k).size();
				if (l1 + l2+l3>baocount){
					continue;
				}
				else{
					if (vv5.empty()&&vv4.empty()){
						ist = true;
						break;
					}
				}
				for (int m = 0; m < vv4.size(); m++){
					if (ist){
						break;
					}
					int l4 = vv4.at(m).size();
					if (l1 + l2 + l3+l4>baocount){
						continue;
					}
					else{
						if (vv5.empty()){
							ist = true;
							break;
						}
					}
					for (int n = 0; n < vv5.size(); n++){
						int l5 = vv5.at(n).size();
						if (l1 + l2 + l3 + l4+l5>baocount){
							continue;
						}
						else{
							ist = true;
							break;
						}
					}
				}
			}
		}
	}
	HuItem item;
	if (!ist){
		item->_hutype = None;
		return item;
	}
	else{
		item->_hutype = ;
	}
	
}

HuItem ConfigData1::isQidui(int *pai, int baovalue){
	HuItem item;
	map<int, int>mps;
	int cou = 0;
	for (int i = 0; i < 14; i++){
		int va = pai[i];
		if (va>0){
			if (baovalue != va){
				if (mps.find(va) != mps.end()){
					mps.at(va)++;
				}
				else{
					mps.insert(va) = 1;
				}
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
	
	item->_hy = HEI;
	for (itr; itr != mps.end();){
		int co = itr->second;
		if (co == 2){
			count++;
			itr = mps.erase(itr);
		}
		else if (co > 2){
			count++;
			itr->second = itr->second-2;
			itr++;
		}
		else{
			itr++;
		}
	}
	if (7 - count <= baovalue){
		item->_hutype = QIDUI;
		itr = mps.begin();
		for (itr; itr != mps.end(); itr++){
			int va = itr->first;
			if (va != baovalue){
				item->_hy = RUAN;
				break;
			}
		}
	}
	else{
		item->_hutype = None;
	}
	return item;
}

HuItem ConfigData1::isPPH(int *pai, int baovalue){
	HuItem item = isQidui(pai,baovalue);
	
	if (item->_hutype == None){
		map<int, int>mps;
		for (int i = 0; i < 14; i++){
			int va = pai[i];
			if (va>0){
				if (baovalue != va){
					if (mps.find(va) != mps.end()){
						mps.at(va)++;
					}
					else{
						mps.insert(va) = 1;
					}
				}
			}
		}
		//怎么判断是碰碰胡

	}
	else if (item->_hutype == QIDUI&&item->_hy == RUAN){
		map<int, int>mps;
		for (int i = 0; i < 14; i++){
			int va = pai[i];
			if (va>0){
				if (mps.find(va) != mps.end()){
					mps.at(va)++;
				}
				else{
					mps.insert(va) = 1;
				}
			}
		}
		auto itr = mps.begin();
		int duicount = 0;
		for (itr; itr != mps.end();itr++){
			if (itr->second < 2){
				return item;
			}
			else if (itr->second == 2){
				duicount++;
				if (duicount>1){
					return item;
				}
			}
		}

	}
	else{
		return item;
	}
}

HuItem ConfigData1::isQYS(int *pai, int baovalue){

}