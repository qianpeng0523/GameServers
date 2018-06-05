
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
	//test();
}

#define  TESTCOUNT 8000
void ConfigData::test(){
	/*
	int a[TESTCOUNT][14] = { 0 };
	int b[TESTCOUNT][14] = { 0 };
	int index = 0;
	int64_t t0 = Common::getCurrentTime();
	while (index < TESTCOUNT){
		//胡牌和出牌听牌测试
		for (int i = 2; i <= 14; i += 3){
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
	int64_t t1 = Common::getCurrentTime();
	int64_t tt = t1 - t0;
	CLog::log("******rand.use time:%fs******\n", tt / 1000.0 / 1000);


	index = 0;
	while (index < TESTCOUNT){
		PaoNeed pn = isHu(a[index], true, 1);
		CLog::log("hu:%d,type:%d",pn._hu._hutype,pn._hu._hy);
		index++;
	}
	int64_t t2 = Common::getCurrentTime();
	tt = t2 - t1;
	CLog::log("******Hu.use time:%fs******\n", tt / 1000.0 / 1000);
	*/
	/*
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
		map<int, vector<int>> vec = chuTing(a[index], 1);
		index++;
	}
	int64_t t1 = Common::getCurrentTime();
	tt = t1 - t2;
	CLog::log("******ChuTing.use time:%gs******\n", tt / 1000.0 / 1000);
	*/


	int pai[14] = {0x01,0x02};
	PaoNeed pn = isHu(pai, true, 1);
	CLog::log("hu:%d,type:%d", pn._hu._hutype, pn._hu._hy);

	CLog::log("1111\n");
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

PaoNeed ConfigData::isHu(int *pai, bool isqing, int baovalue){
	char temppai[14];
	for (int i = 0; i < 14;i++){
		temppai[i] = pai[i]+'0';
	}
	char bv = baovalue+'0';
	return isHu(temppai, isqing, bv);
}

PaoNeed ConfigData::isHu(char *pai, bool isqing, char baovalue){
	int baocount = 0;
	map<char, int> dds;
	for (int i = 0; i < 14; i++){
		char va = pai[i];
		if (va>'0'){
			if (baovalue == va){
				baocount++;
			}
			if (dds.find(va) == dds.end()){
				dds.insert(make_pair(va, 1));
			}
			else{
				dds.at(va)++;
			}
		}
	}
	PaoNeed pn;
	memcpy(pn._handcards, pai, sizeof(char)* 14);
// 	map<char, int> mm;
// 	dds.swap(mm);
// 	dds.clear();
	if (baocount == 0){
		pn = isHu(pai, isqing);
		return pn;
	}
	else{
		//先计算黑的
		pn = isHu(pai, isqing);

		memcpy(pn._handcards, pai, sizeof(char)* 14);
		return pn;
	}
}

PaoNeed ConfigData::isHu(char *pai, bool isqing){
	int baocount = 0;
	string hcon;
	int types[5] = { 0 };
	for (int i = 0; i < 14; i++){
		int va = pai[i]-'0';
		if (va>0){
			int type = va / 16;
			int value = type % 16;
			types[type]++;
			char a = va + type * 10 + '0'+10;
			hcon += a;
		}
	}
	PaoNeed pn;
	int index = 0;
	int yus[5] = { types[0] % 3, types[1] % 3, types[2] % 3, types[3] % 3, types[4] % 3 };
	int co = 0;
	int tts[5] = { types[0] / 3, types[1] / 3, types[2] / 3, types[3] / 3, types[4] / 3 };
	for (int i = 0; i < 5; i++){
		if (yus[i]>0){
			co++;
			if (co > 2){
				return pn;
			}
		}
		index += pow(5, 4 - i)*tts[i];
	}
	Compress(hcon);
	return isHu((char *)hcon.c_str(), index, isqing);
}

PaoNeed ConfigData::isHu(char* pai, int index, bool isqing){
	PaoNeed pn;
	if (isqing){
		if (!(index == pow(5, 4) || index == pow(5, 4) * 2 || index == pow(5, 4) * 3 || index == pow(5, 4) * 4 || index == 0)){
			isqing = false;
		}
		else if (!(index == pow(5, 3) || index == pow(5, 3) * 2 || index == pow(5, 3) * 3 || index == pow(5, 3) * 4 || index == 0)){
			isqing = false;
		}
		else if (!(index == pow(5, 2) || index == pow(5, 2) * 2 || index == pow(5, 2) * 3 || index == pow(5, 2) * 4 || index == 0)){
			isqing = false;
		}
	}
	string ss = pai;
	int len = ss.length();
	int count2 = count(ss.begin(),ss.end(), '2');
	int count4 = count(ss.begin(), ss.end(), '4');
	int count3 = count(ss.begin(), ss.end(), '3');
	if (count2 + count4 / 2 == 7){
		pn._hu._hutype = isqing?QYSQD:QIDUI;
		pn._hu._hy = HEI;
		return pn;
	}
	else if (count2 + count4 / 2==0){
		return pn;
	}
	string mps;
	getHusString(index,mps);
	if (mps.empty()){
		if (mps.find(pai) != -1){
			if (count3 == len / 3){
				pn._hu._hutype = isqing ? QYSPENG : PENGPENG;
			}
			else{
				pn._hu._hutype = isqing ? QINGYISE : PI;
			}
			pn._hu._hy = HEI;
			return pn;
		}
	}
	return pn;
}

static int g_index = 0;

void ConfigData::Compress(string &content){
	string zip;	//"aaaabbndaa"压缩后a4b2nda2
	int  n = 1;
	for (int i = 0; i < content.length(); i++)
	{
		if (content[i] == content[i + 1])
		{
			n++;
		}
		else
		{
			//int convert to string
			char tmp[5];
			sprintf(tmp, "%d", n);
			string str_n = tmp;
			zip += content[i];
			if (n != 1)
				zip += str_n;
			n = 1;
		}

	}
	content = zip;
}

void ConfigData::createHus(){
	time_t t0 = Common::getCurrentTime();
	//1.生成所有的刻子
	vector<string> ke;
	for (int i = 0; i < 9; i++){
		string vc;
		for (int j = 0; j < 3; j++){
			int v = i + 1;
			vc += (char)(v+10 + '0');
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
		vc += (char)(v1 + 10 + '0');
		vc += (char)(v2 + 10 + '0');
		vc += (char)(v3 + 10 + '0');
		lian.push_back(vc);
	}
	//组合所有的刻子和连子
	set<string>mps;
	for (int i = 0; i < lian.size(); i++){
		string t1 = lian.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps.find(con) == mps.end()){
			mps.insert(con);
		}
		for (int j = i; j < lian.size(); j++){
			string t2 = lian.at(j);
			con = t1 + t2;
			if (Over4(con)){
				continue;
			}
			sort(con.begin(), con.end());
			if (mps.find(con) == mps.end()){
				mps.insert(con);
			}
			for (int k = j; k < lian.size(); k++){
				string t3 = lian.at(k);
				con = t1 + t2 + t3;
				if (Over4(con)){
					continue;
				}
				sort(con.begin(), con.end());
				if (mps.find(con) == mps.end()){
					mps.insert(con);
				}
				for (int m = k; m < lian.size(); m++){
					string t4 = lian.at(m);
					con = t1 + t2 + t3 + t4;
					if (Over4(con)){
						continue;
					}
					sort(con.begin(), con.end());
					if (mps.find(con) == mps.end()){
						mps.insert(con);
					}
				}
			}
		}
	}

	vector<string>v1;
	lian.clear();
	lian.swap(v1);

	mps.insert("");
	set<string>mps1;
	for (int i = 0; i < ke.size(); i++){
		string t1 = ke.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		//Compress(con);
		if (mps1.find(con) == mps1.end()){
			mps1.insert(con);
		}
		for (int j = i + 1; j < ke.size(); j++){
			string t2 = ke.at(j);
			string con = t1 + t2;
			if (Over4(con)){
				continue;
			}
			sort(con.begin(), con.end());
			//Compress(con);
			if (mps1.find(con) == mps1.end()){
				mps1.insert(con);
			}
			for (int m = j + 1; m < ke.size(); m++){
				string t3 = ke.at(m);
				string con = t1 + t2 + t3;
				if (Over4(con)){
					continue;
				}
				sort(con.begin(), con.end());
				//Compress(con);
				if (mps1.find(con) == mps1.end()){
					mps1.insert(con);
				}
				for (int n = m + 1; n < ke.size(); n++){
					string t4 = ke.at(n);
					string con = t1 + t2 + t3 + t4;
					if (Over4(con)){
						continue;
					}
					sort(con.begin(), con.end());
					//Compress(con);
					if (mps1.find(con) == mps1.end()){
						mps1.insert(con);
					}
				}

			}

		}

	}
	mps1.insert("");
	
	ke.clear();
	ke.swap(v1);

	time_t t1 = Common::getCurrentTime();
	printf("1.use time:%f\n",(t1-t0)/1000.0/1000);
	//组合 0 3 6 9 12
	set<string>allmps[5][5];
	auto itr = mps.begin();
	for (itr; itr != mps.end(); itr++){
		string con1 = *itr;
		auto itr1 = mps1.begin();
		for (itr1; itr1 != mps1.end(); itr1++){
			string con2 = *itr1;
			string con = con1 + con2;
			int len = strlen(con.c_str());
			if (len > 12){
				continue;
			}
			else if (Over4(con)){
				continue;
			}
			sort(con.begin(), con.end());
			string content0 = con;
			//Compress(content0);
			set<string> *vec = &allmps[0][len / 3];
			if (vec->find(content0) == vec->end()){
				vec->insert(content0);
				//allmps.at(len) = vec;
			}
			string content1 = con;
			addnumber(content1, 10);
			//Compress(content1);
			set<string> *vec1 = &allmps[1][len / 3];
			if (vec1->find(content1) == vec1->end()){
				vec1->insert(content1);
				//allmps.at(len) = vec;
			}
			string content2 = con;
			addnumber(content2, 20);
			//Compress(content2);
			set<string> *vec2 = &allmps[2][len / 3];
			if (vec2->find(content2) == vec2->end()){
				vec2->insert(content2);
			}
			string content3 = con;
			if (con.find(10 + '5') != -1 || con.find(10 + '6') != -1 || con.find(10 + '7') != -1 || con.find(10 + '8') != -1 || con.find(10 + '9') != -1){
				continue;
			}
			addnumber(content3, 30);
			//Compress(content3);
			set<string> *vec3 = &allmps[3][len / 3];
			if (vec3->find(content3) == vec3->end()){
				vec3->insert(content3);
			}
			string content4 = con;
			if (con.find(10 + '4') != -1){
				continue;
			}
			addnumber(content4, 40);
			//Compress(content4);
			set<string> *vec4 = &allmps[4][len / 3];
			if (vec4->find(content4) == vec4->end()){
				vec4->insert(content4);
			}
			
		}
	}
	set<string>mm;
	mps.swap(mm);
	mps.clear();
	
	mps1.swap(mm);
	mps1.clear();
	time_t t2 = Common::getCurrentTime();
	printf("2.use time:%f\n", (t2 - t1) / 1000.0 / 1000);

	map<int, set<string>> zjallmaps;

	//终极所有 0 1 2 3 4 5色
	for (int i = 0; i <5; i++){
		set<string> *vec1 = &allmps[0][i];
		auto itr11 = vec1->begin();
		for (itr11; itr11 != vec1->end(); itr11++){
			string con1 = *itr11;
			if (!PushHus(con1, i*pow(5, 4), zjallmaps)){
				continue;
			}
			int len1 = 5-i;
			for (int j = 0; j < len1; j++){
				set<string> *vec2 = &allmps[1][j];
				auto itr12 = vec2->begin();
				for (itr12; itr12 != vec2->end(); itr12++){
					string con2 = *itr12;
					//addnumber(con2, 10);
					if (!PushHus(con1 + con2, i*pow(5, 4) + j*pow(5, 3), zjallmaps)){
						continue;
					}
					int len2 = 5 - i-j;
					for (int k = 0; k < len2; k++){
						set<string> *vec3 = &allmps[2][k];
						auto itr13 = vec3->begin();
						for (itr13; itr13 != vec3->end(); itr13++){
							string con3 = *itr13;
							//addnumber(con3, 20);
							if (!PushHus(con1 + con2 + con3, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2), zjallmaps)){
								continue;
							}
							int len3 = 5 - i - j-k;
							for (int l = 0; l <len3; l++){
								set<string> *vec4 = &allmps[3][l];
								auto itr14 = vec4->begin();
								for (itr14; itr14 != vec4->end(); itr14++){
									string con4 = *itr14;
									
									//addnumber(con4, 30);
									if (!PushHus(con1 + con2 + con3 + con4, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2) + l*pow(5, 1), zjallmaps)){
										continue;
									}
									int len4 = 5 - i - j - k - l;
									for (int m = 0; m < len4; m++){
										set<string> *vec5 = &allmps[4][m];
										auto itr15 = vec5->begin();
										for (itr15; itr15 != vec5->end(); itr15++){
											string con5 = *itr15;
											//addnumber(con5, 40);
											if (!PushHus(con1 + con2 + con3 + con4 + con5, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2) + l*pow(5, 1) + m, zjallmaps)){
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
	time_t t3 = Common::getCurrentTime();
	printf("3.use time:%f\n", (t3 - t2) / 1000.0 / 1000);
	//写入index
 	char buff[50];
// 	auto tt = m_husindex.begin();
// 	string ttf;
// 	FILE *file = fopen("./res/ss/index", "a+");
// 	int inde = 0;
// 	int sz1 = m_husindex.size();
// 	for (tt; tt != m_husindex.end();tt++){
// 		int di = tt->first;
// 		if (inde != sz1 - 1){
// 			sprintf(buff, "%d\n", di);
// 		}
// 		else{
// 			sprintf(buff, "%d", di);
// 		}
// 		fprintf(file,buff);
// 		inde++;
// 	}
// 	fclose(file);
	
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			allmps[i][j].swap(mm);
			allmps[i][j].clear();
		}
	}
	g_index = 0;
	
	auto itrr = zjallmaps.begin();
	for (itrr; itrr != zjallmaps.end();){
		int index = itrr->first;
		if (isHaveFile(index)){
			itrr++;
			continue;
		}
		map<int, string> mmaps;
		string str;
		for (int i = 0; i < g_kind; i++){
			int type = g_all_mjkind[i] / 16;
			int va = g_all_mjkind[i] % 16;
			char a = va + type * 10 + '0'+10;
			set<string> *vec = &itrr->second;
			auto it1 = vec->begin();
			for (it1; it1 != vec->end(); it1++){
				string content = *it1;
				inserDui(content, a);
				if (content.length() % 3 != 0){
					PushHus1(content, index, str,mmaps);
				}
			}
		}
		itrr->second.swap(mm);
		itrr->second.clear();
		
		itrr = zjallmaps.erase(itrr);
		g_index++;
		str.clear();
		string tt;
		str.swap(tt);
		
		
		
		auto iiiit = mmaps.begin();
		for (iiiit; iiiit != mmaps.end();){
			int in = iiiit->first;
			string *cont = &iiiit->second;
			char buff[50];
			sprintf(buff, "./res/ssbao/hu%d", in);
			FILE *file = fopen(buff, "a+");
			fprintf(file, cont->c_str());
			fclose(file);
			string mi;
			cont->clear();
			cont->swap(mi);
			iiiit = mmaps.erase(iiiit);
			printf("****%s*****\n",buff);
			
		}
		map<int, string> mmaps1;
		mmaps.clear();
		mmaps.swap(mmaps1);

// 		int ind = m_husindex.at(index);
// 		auto vec = &m_zjallmaps1[ind];
// 		auto ir1 = vec->begin();
// 		sprintf(buff, "./res/ss/hu%d", ind);
// 		FILE *file = fopen(buff, "r");
// 		if (!file){
// 			file = fopen(buff, "a+");
// 			string con;
// 			int inde = 0;
// 			int sz = vec->size();
// 			for (ir1; ir1 != vec->end(); ir1++){
// 				con += *ir1;
// 				if (inde != sz - 1){
// 					con += "\n";
// 				}
// 				fprintf(file, con.c_str());
// 				con = "";
// 				inde++;
// 			}
// 			fclose(file);
// 		}
		printf("[%d].wirte data\n", g_index);
// 		set<char *>mm1;
// 		vec->swap(mm1);
// 		vec->clear();
		
		
	}
	time_t t4 = Common::getCurrentTime();
	printf("4.use time:%f\n", (t4 - t3) / 1000.0 / 1000);
	printf("5.alluse time:%f\n", (t4 - t0) / 1000.0 / 1000);
	printf("\n");
}

bool ConfigData::isHaveFile(int index){
	char buff[50];
	for (int i = 15; i >= 1; i--){
		sprintf(buff, "./res/ssbao/hu%d", i*10000+index);
		FILE *file = fopen(buff, "r");
		if (file){
			return true;
		}
	}
	return false;
}

void ConfigData::addnumber(string &content, int num){
	for (int i = 0; i < content.length();i++){
		content[i] = content[i] + num;
	}
}

static string g_typestr[5] = { "#", "@", "$", "%", "&" };
string ConfigData::inserDui(string &content,char va){
	if (count(content.begin(), content.end(), va)>2){
		return content;
	}
	content += va;
	content += va;
	sort(content.begin(), content.end());
	//Compress(content);
	return content;
}

bool ConfigData::Over4(string &content){
	for (int i = 1; i <= 9; i++){
		if (count(content.begin(), content.end(), i+10+'0')>4){
			return true;
		}
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


bool ConfigData::PushHus(string &content, int index, map<int, set<string>> &zjallmaps){
	if (m_husindex.find(index) == m_husindex.end()){
		m_husindex.insert(make_pair(index, m_husindex.size()));

		printf("%d.%s\n", m_husindex.size(), content.c_str());
	}
	if (zjallmaps.find(index) != zjallmaps.end()){
		set<string> *mps = &zjallmaps.at(index);
		if (mps->find(content) == mps->end()){
			mps->insert(content);
		}
	}
	else{
		set<string> mps;
		mps.insert(content);
		zjallmaps.insert(make_pair(index, mps));
	}
	
	
	return true;
}

bool ConfigData::PushHus1(string &content, int index, string &zjallmaps1, map<int, string> &mmaps){
	int ind = m_husindex.at(index);
	if (zjallmaps1.find(content) == -1){
		zjallmaps1.append(content+"\n");
		if (g_index % 1000000 == 0){
			printf("%d.%s\n", g_index++, content.c_str());
		}

		//写入文件 
		//time_t t0 = Common::getCurrentTime();
		for (int i = 0; i < content.length(); i++){
			string con = content;
			con.erase(i, 1);
			string ct = con;
			Compress(ct);
			PushHus2(ct, index, mmaps);
			
			for (int j = 0; j < con.length(); j++){
				string con1 = con;
				con1.erase(j, 1);
				string ct1 = con1;
				Compress(ct1);
				PushHus2(ct1, index, mmaps);
				
				for (int k = 0; k < con1.length(); k++){
					string con2 = con1;
					con2.erase(k, 1);
					string ct2 = con2;
					Compress(ct2);
					PushHus2(ct2, index, mmaps);
					
					for (int m = 0; m < con2.length(); m++){
						string con3 = con2;
						con3.erase(m, 1);
						string ct3 = con3;
						Compress(ct3);
						PushHus2(ct3, index, mmaps);
						
					}
				}
			}
		}
		//time_t t1 = Common::getCurrentTime();
		//printf("use time:%f\n",(t1-t0)/1000.0/1000);
	}

	return true;
}

void ConfigData::getHusString(int index, string &content){
	if (m_husindex.find(index) != m_husindex.end()){
		int in = m_husindex.at(index);
		char buff[50];
		sprintf(buff, "./res/ss/hu%d", in);
		CSVDataInfo::getIns()->openCSVFile(buff, content);
	}
}

void ConfigData::PushHus2(string &content, int index, map<int, string> &maps){
	int a = index+(content.length()+1)*10000;
	if (maps.find(a) == maps.end()){
		string vs;
		if (content.empty()){
			vs += "\n";
			maps.insert(make_pair(a, vs));
		}
		else{
			vs += content+"\n";
			maps.insert(make_pair(a,vs));
		}
// 		char buff[50];
// 		sprintf(buff, "./res/ssbao/hu%d", a);
// 		FILE *file = fopen(buff, "a+");
// 		fprintf(file,(content+"\n").c_str());
// 		fclose(file);
	}
	else{
		if (!content.empty()){
			string *vs = &maps.at(a);
			if (vs->find(content) ==-1){
				vs->append(content + "\n");
// 				char buff[50];
// 				sprintf(buff, "./res/ssbao/hu%d", a);
// 				FILE *file = fopen(buff, "a+");
// 				fprintf(file, (content+"\n").c_str());
// 				fclose(file);
			}
		}
	}
	
}