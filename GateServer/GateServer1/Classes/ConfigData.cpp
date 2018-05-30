
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

void ConfigData::init(){
	
	createHus();
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
	m_cards.clear();
	m_index = 0;
	for (int i = 0; i < CARDNUMBER; i++){
		m_cards.push_back(g_all_mj[i]);
	}
	random_shuffle(m_cards.begin(), m_cards.end());
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

static int g_index = 0;
void ConfigData::createHus(){
	//1.生成所有的刻子
	vector<string> ke;
	for (int i = 0; i < 9; i++){
		string vc;
		for (int j = 0; j < 3; j++){
			int v = i + 1;
			vc += (char)(v + '0');
		}
		ke.push_back(vc);
	}
	//2.生成所有的连子
	vector<string> lian;
	for (int i = 0; i < 7; i++){
		int v1 = i + 1;
		int v2 = i + 2;
		int v3 = i + 3;
		string vc;
		vc += (char)(v1 + '0');
		vc += (char)(v2 + '0');
		vc += (char)(v3 + '0');
		lian.push_back(vc);
	}
	//组合所有的刻子和连子
	map<string, string>mps;
	for (int i = 0; i < lian.size(); i++){
		string t1 = lian.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps.find(con) == mps.end()){
			mps.insert(make_pair(con, ""));
		}
		for (int j = i; j < lian.size(); j++){
			string t2 = lian.at(j);
			con = t1 + t2;
			sort(con.begin(), con.end());
			if (mps.find(con) == mps.end()){
				mps.insert(make_pair(con, ""));
			}
			for (int k = j; k < lian.size(); k++){
				string t3 = lian.at(k);
				con = t1 + t2 + t3;
				sort(con.begin(), con.end());
				if (mps.find(con) == mps.end()){
					mps.insert(make_pair(con, ""));
				}
				for (int m = k; m < lian.size(); m++){
					string t4 = lian.at(m);
					con = t1 + t2 + t3 + t4;
					sort(con.begin(), con.end());
					if (mps.find(con) == mps.end()){
						mps.insert(make_pair(con, ""));
					}
				}
			}
		}
	}
	mps.insert(make_pair("", ""));
	map<string, string>mps1;
	for (int i = 0; i < ke.size(); i++){
		string t1 = ke.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps1.find(con) == mps1.end()){
			mps1.insert(make_pair(con, ""));
		}
		for (int j = i + 1; j < ke.size(); j++){
			string t2 = ke.at(j);
			string con = t1 + t2;
			sort(con.begin(), con.end());
			if (mps1.find(con) == mps1.end()){
				mps1.insert(make_pair(con, ""));
			}
			for (int m = j + 1; m < ke.size(); m++){
				string t3 = ke.at(m);
				string con = t1 + t2 + t3;
				sort(con.begin(), con.end());
				if (mps1.find(con) == mps1.end()){
					mps1.insert(make_pair(con, ""));
				}
				for (int n = m + 1; n < ke.size(); n++){
					string t4 = ke.at(n);
					string con = t1 + t2 + t3 + t4;
					sort(con.begin(), con.end());
					if (mps1.find(con) == mps1.end()){
						mps1.insert(make_pair(con, ""));
					}
				}

			}

		}

	}
	mps1.insert(make_pair("", ""));
	//组合
	map<string, string>allmps[5];
	auto itr = mps.begin();
	for (itr; itr != mps.end(); itr++){
		string con1 = itr->first;
		auto itr1 = mps1.begin();
		for (itr1; itr1 != mps1.end(); itr1++){
			string con2 = itr1->first;
			string con = con1 + con2;
			int len = strlen(con.c_str());
			if (len > 12){
				continue;
			}
			else if (Over4(con) > 4){
				continue;
			}
			sort(con.begin(), con.end());
			map<string, string> *vec = &allmps[len/3];
			if (vec->find(con) == vec->end()){
				vec->insert(make_pair(con, ""));
				//allmps.at(len) = vec;
			}
			
		}
	}




	//终极所有
	for (int i = 0; i <5; i++){
		map<string, string> *vec1 = &allmps[i];
		auto itr11 = vec1->begin();
		for (itr11; itr11 != vec1->end(); itr11++){
			string con1 = itr11->first;
			if (!PushHus(con1, i*pow(5,4))){
				continue;
			}
			for (int j = 0; j < 5 - i; j++){
				map<string, string> *vec2 = &allmps[j];
				auto itr12 = vec2->begin();
				for (itr12; itr12 != vec2->end(); itr12++){
					string con2 = itr12->first;
					addnumber(con2, 10);
					if (!PushHus(con1 + con2, i*pow(5, 4) + j*pow(5, 3))){
						continue;
					}
					for (int k = 0; k < 5 - j - i; k++){
						map<string, string> *vec3 = &allmps[k];
						auto itr13 = vec3->begin();
						for (itr13; itr13 != vec3->end(); itr13++){
							string con3 = itr13->first;
							addnumber(con3, 20);
							if (!PushHus(con1 + con2 + con3, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2))){
								continue;
							}
							for (int l = 0; l < 5 - k - j - i; l++){
								map<string, string> *vec4 = &allmps[l];
								auto itr14 = vec4->begin();
								for (itr14; itr14 != vec4->end(); itr14++){
									string con4 = itr14->first;
									
									if (con4.find("5") != -1 || con4.find("6") != -1 || con4.find("7") != -1 || con4.find("8") != -1 || con4.find("9") != -1){
										continue;
									}
									addnumber(con4, 30);
									if (!PushHus(con1 + con2 + con3 + con4, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2) + l*pow(5, 1))){
										continue;
									}

									for (int m = 0; m < 5 - l - k - j - i; m++){
										map<string, string> *vec5 = &allmps[m];
										auto itr15 = vec5->begin();
										for (itr15; itr15 != vec5->end(); itr15++){
											string con5 = itr15->first;
											if (con5.find("4") != -1 || con5.find("5") != -1 || con5.find("6") != -1 || con5.find("7") != -1 || con5.find("8") != -1 || con5.find("9") != -1){
												continue;
											}
											addnumber(con5, 40);
											if (!PushHus(con1 + con2 + con3 + con4 + con5, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2) + l*pow(5, 1)+m)){
												continue;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	g_index = 0;
	for (int i = 0; i < g_kind; i++){
		int type = g_all_mjkind[i] / 16;
		int va = g_all_mjkind[i] % 16;
		char a = va+type*10+'0';
		for (int j = 0; j < 3125;j++){
			map<string, string> *vec = &m_zjallmaps[j];
			auto it1 = vec->begin();
			for (it1; it1 != vec->end();it1++){
				string content = it1->first;
				string tp = inserDui(content, a);
				if (tp.compare(content) != 0){
					map<string, string > *mmp = &m_zjallmaps1[j];
					if (mmp->find(tp) == mmp->end()){
						mmp->insert(make_pair(tp, ""));
						printf("%d.[%s]\n", g_index++, tp.c_str());
					}
				}
			}
		}
	}
	printf("\n");
}

void ConfigData::addnumber(string &content, int num){
	for (int i = 0; i < content.length();i++){
		content[i] = content[i] + num;
	}
}

static string g_typestr[5] = { "#", "@", "$", "%", "&" };
string ConfigData::inserDui(string &content,char va){
	if (count(content.begin(), content.end(), a)>2){
		return content;
	}
	string tp = content;
	tp += va;
	tp += va;
	
	sort(tp.begin(),tp.end());
	return tp;
}

bool ConfigData::Over4(string &content, char a){
	if (count(content.begin(), content.end(), a) > 4){
		return true;
	}
	return false;
}

bool ConfigData::Over4(string &content){
	if (count(content.begin(), content.end(), 1 + '0') > 4 || count(content.begin(), content.end(), 2 + '0') > 4 || count(content.begin(), content.end(), 3 + '0') > 4
		|| count(content.begin(), content.end(), 4 + '0') > 4 || count(content.begin(), content.end(), 5 + '0') > 4 || count(content.begin(), content.end(), 6 + '0') > 4
		|| count(content.begin(), content.end(), 7 + '0') > 4 || count(content.begin(), content.end(), 8 + '0') > 4 || count(content.begin(), content.end(), 9 + '0') > 4){
		return true;
	}
	return false;
}

bool ConfigData::OverLen(string &content,int count){
	int len = strlen(content.c_str());
	if (len > count){
		return true;
	}
	return false;
}

void ConfigData::eraseEmpty(string &content, string old){
	string temp = content;
	int index1 = temp.find(old);
	if (index1 != -1){
		int index2 = temp.substr(index1 + 1, temp.length()).find(old);
		if (index2==0){
			content.replace(index1 + 1, 1, "");
			content.replace(index1, 1, "");
		}
	}
}


bool ConfigData::PushHus(string &content, int index){
	map<string, string> *mps = &m_zjallmaps[index];
	if (mps->find(content) == mps->end()){
		mps->insert(make_pair(content,""));
		printf("%d.%s\n", g_index++, content.c_str());
	}
	
	return true;
}