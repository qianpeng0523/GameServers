
#include "ConfigData.h"

ConfigData *ConfigData::m_ins=NULL;
ConfigData::ConfigData(){
	m_index = 0;
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
	//random_shuffle(m_cards.begin(), m_cards.end());
}

void ConfigData::init(){
	for (int i = 2; i <= 14; i += 3){
		vector<vector<int>> vecs;
		for (int m = 0; m < CARDNUMBER; m++){
			int *a = new int[i];
			initMJ();
			for (int j = m; j < m + i; j++){
				a[j-m] = getMJ(j);
			}
			//获取所有的牌有误
			quickSort(a, 0, i - 1);
			if (isHu(a, i)){
				vector<int>vec;
				for (int k = 0; k < i; k++){
					vec.push_back(a[k]);
					printf("0x%02X ", a[k]);
				}
				printf("\n");
				vecs.push_back(vec);
			}
			delete a;
		}
		m_hus.insert(make_pair(i,vecs));
	}
	printf("*************\n");
}

void ConfigData::copy(int *&a, int *b, int len){
	for (int i = 0; i < len; i++){
		a[i] = b[i];
	}
}

bool ConfigData::isHu(int *p, int len){
	quickSort(p, 0,len-1);
	if (is7Dui(p, len)){
		return true;
	}
	for (int d1 = 0; d1 < len - 1; d1++){
		int *m = new int[len];
		copy(m, p, len);
		if (isDui(m, d1, d1+1)){
			setZero(m, d1);
			setZero(m, d1+1);
		}

		int *n = new int[len];
		copy(n, m, len);
		int index = 0;
		while (index < 2){
			for (int l1 = 0; l1 < len - 1; l1++){
				for (int l2 = l1 + 1; l2 < len - 1; l2++){
					for (int l3 = l2 + 1; l3 < len - 1; l3++){
						if (index == 0){
							if (is3Same(n, l1, l2, l3)){
								setZero(n, l1);
								setZero(n, l2);
								setZero(n, l3);
							}
							else if (is3Lian(n, l1, l2, l3)){
								setZero(n, l1);
								setZero(n, l2);
								setZero(n, l3);
							}
						}
						else if (index == 1){
							if (is3Lian(n, l1, l2, l3)){
								setZero(n, l1);
								setZero(n, l2);
								setZero(n, l3);
							}
							else if (is3Same(n, l1, l2, l3)){
								setZero(n, l1);
								setZero(n, l2);
								setZero(n, l3);
							}
						}

						bool ishu = true;
						for (int k = 0; k < len; k++){
							if (n[k]>0){
								ishu = false;
								break;
							}
						}

						if (ishu){
							delete m;
							delete n;
							return true;
						}
					}
				}
			}
			index++;
		}
		delete n;
		bool ishu = true;
		for (int k = 0; k < len; k++){
			if (m[k]>0){
				ishu = false;
				break;
			}
		}

		if (ishu){
			delete m;
			return true;
		}
		
	}
	return false;
}

bool ConfigData::isDui(int *p, int index1, int index2){
	if (p[index1]>0&&p[index2]>0&&p[index1] == p[index2]){
		return true;
	}
	return false;
}

bool ConfigData::is3Same(int *p,int index1, int index2, int index3){
	if (p[index1] == 0 || p[index2] == 0 || p[index3] == 0){
		return false;
	}
	else if (p[index1] == p[index2] && p[index2] == p[index3]){
		return true;
	}
	return false;
}

bool ConfigData::is3Lian(int *p, int index1, int index2, int index3){
	quickSort(p, 0, 2);
	if (p[index1] == 0 || p[index2] == 0 || p[index3] == 0){
		return false;
	}
	else if (p[index1] + 1 == p[index2] && p[index2] + 1 == p[index3]){
		return true;
	}
	return false;
}

bool ConfigData::is7Dui(int *dui7,int len){
	if (len < 14){
		return false;
	}
	int temp[14];
	for (int i = 0; i < 14; i++){
		temp[i] = dui7[i];
		if (dui7[i] == 0){
			return false;
		}
	}
	for (int i = 0; i < len;i++){
		int v1 = temp[i];
		for (int j = i + 1; j < len;j++){
			int v2 = temp[j];
			if (v1 == v2&&v1!=0&&v2!=0){
				temp[i] = 0;
				temp[j] = 0;
				break;
			}
		}
	}
	for (int i = 0; i < 14; i++){
		if (temp[i]>0){
			return false;
		}
	}
	return true;
}

bool ConfigData::isQing(int *p, int len){
	int a = p[0]/16;
	for (int i = 1; i < len; i++){
		if (p[i]==0|| p[i] / 9 != a){
			return false;
		}
	}
	return true;
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

void ConfigData::setZero(int *&a, int index){
	a[index] = 0;
}