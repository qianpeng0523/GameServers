
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
	//getHus();
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

void ConfigData::getHus(){
	CSVDataInfo::getIns()->openCSVFile("./res/ss/index", m_husindex);
	char buff[30];
	auto itr = m_husindex.begin();
	for (itr; itr != m_husindex.end();itr++){
		int type = itr->first;
		int index = itr->second;
		sprintf(buff,"./res/ss/hu%d",index);
		CSVDataInfo::getIns()->openCSVFile(buff, m_zjallmaps1[index]);
		printf("%s\n",buff);
	}
	printf("\n");
}

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
	map<string, char>mps;
	for (int i = 0; i < lian.size(); i++){
		string t1 = lian.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		if (mps.find(con) == mps.end()){
			mps.insert(make_pair(con, '\0'));
		}
		for (int j = i; j < lian.size(); j++){
			string t2 = lian.at(j);
			con = t1 + t2;
			if (Over41(con)){
				continue;
			}
			sort(con.begin(), con.end());
			if (mps.find(con) == mps.end()){
				mps.insert(make_pair(con, '\0'));
			}
			for (int k = j; k < lian.size(); k++){
				string t3 = lian.at(k);
				con = t1 + t2 + t3;
				if (Over41(con)){
					continue;
				}
				sort(con.begin(), con.end());
				if (mps.find(con) == mps.end()){
					mps.insert(make_pair(con, '\0'));
				}
				for (int m = k; m < lian.size(); m++){
					string t4 = lian.at(m);
					con = t1 + t2 + t3 + t4;
					if (Over41(con)){
						continue;
					}
					sort(con.begin(), con.end());
					if (mps.find(con) == mps.end()){
						mps.insert(make_pair(con, '\0'));
					}
				}
			}
		}
	}
	mps.insert(make_pair("", '\0'));
	map<string, char>mps1;
	for (int i = 0; i < ke.size(); i++){
		string t1 = ke.at(i);
		string con = t1;
		sort(con.begin(), con.end());
		//Compress(con);
		if (mps1.find(con) == mps1.end()){
			mps1.insert(make_pair(con, '\0'));
		}
		for (int j = i + 1; j < ke.size(); j++){
			string t2 = ke.at(j);
			string con = t1 + t2;
			if (Over41(con)){
				continue;
			}
			sort(con.begin(), con.end());
			//Compress(con);
			if (mps1.find(con) == mps1.end()){
				mps1.insert(make_pair(con, '\0'));
			}
			for (int m = j + 1; m < ke.size(); m++){
				string t3 = ke.at(m);
				string con = t1 + t2 + t3;
				if (Over41(con)){
					continue;
				}
				sort(con.begin(), con.end());
				//Compress(con);
				if (mps1.find(con) == mps1.end()){
					mps1.insert(make_pair(con, '\0'));
				}
				for (int n = m + 1; n < ke.size(); n++){
					string t4 = ke.at(n);
					string con = t1 + t2 + t3 + t4;
					if (Over41(con)){
						continue;
					}
					sort(con.begin(), con.end());
					//Compress(con);
					if (mps1.find(con) == mps1.end()){
						mps1.insert(make_pair(con, '\0'));
					}
				}

			}

		}

	}
	mps1.insert(make_pair("", '\0'));
	//组合 0 3 6 9 12
	map<string, char>allmps[5][5];
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
			else if (Over41(con)){
				continue;
			}
			sort(con.begin(), con.end());
			string content0 = con;
			//Compress(content0);
			map<string, char> *vec = &allmps[0][len / 3];
			if (vec->find(content0) == vec->end()){
				vec->insert(make_pair(content0, '\0'));
				//allmps.at(len) = vec;
			}
			string content1 = con;
			addnumber(content1, 10);
			//Compress(content1);
			map<string, char> *vec1 = &allmps[1][len / 3];
			if (vec1->find(content1) == vec1->end()){
				vec1->insert(make_pair(content1, '\0'));
				//allmps.at(len) = vec;
			}
			string content2 = con;
			addnumber(content2, 20);
			//Compress(content2);
			map<string, char> *vec2 = &allmps[2][len / 3];
			if (vec2->find(content2) == vec2->end()){
				vec2->insert(make_pair(content2, '\0'));
			}
			string content3 = con;
			if (con.find(10 + '5') != -1 || con.find(10 + '6') != -1 || con.find(10 + '7') != -1 || con.find(10 + '8') != -1 || con.find(10 + '9') != -1){
				continue;
			}
			addnumber(content3, 30);
			//Compress(content3);
			map<string, char> *vec3 = &allmps[3][len / 3];
			if (vec3->find(content3) == vec3->end()){
				vec3->insert(make_pair(content3, '\0'));
			}
			string content4 = con;
			if (con.find(10 + '4') != -1){
				continue;
			}
			addnumber(content4, 40);
			//Compress(content4);
			map<string, char> *vec4 = &allmps[4][len / 3];
			if (vec4->find(content4) == vec4->end()){
				vec4->insert(make_pair(content4, '\0'));
			}
			
		}
	}
	map<string, char>mm;
	mps.clear();
	mps.swap(mm);
	
	mps1.clear();
	mps1.swap(mm);

	//终极所有 0 1 2 3 4 5色
	for (int i = 0; i <5; i++){
		map<string, char> *vec1 = &allmps[0][i];
		auto itr11 = vec1->begin();
		for (itr11; itr11 != vec1->end(); itr11++){
			string con1 = itr11->first;
			if (!PushHus(con1, i*pow(5,4))){
				continue;
			}
			int len1 = 5-i;
			for (int j = 0; j < len1; j++){
				map<string, char> *vec2 = &allmps[1][j];
				auto itr12 = vec2->begin();
				for (itr12; itr12 != vec2->end(); itr12++){
					string con2 = itr12->first;
					//addnumber(con2, 10);
					if (!PushHus(con1 + con2, i*pow(5, 4) + j*pow(5, 3))){
						continue;
					}
					int len2 = 5 - i-j;
					for (int k = 0; k < len2; k++){
						map<string, char> *vec3 = &allmps[2][k];
						auto itr13 = vec3->begin();
						for (itr13; itr13 != vec3->end(); itr13++){
							string con3 = itr13->first;
							//addnumber(con3, 20);
							if (!PushHus(con1 + con2 + con3, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2))){
								continue;
							}
							int len3 = 5 - i - j-k;
							for (int l = 0; l <len3; l++){
								map<string, char> *vec4 = &allmps[3][l];
								auto itr14 = vec4->begin();
								for (itr14; itr14 != vec4->end(); itr14++){
									string con4 = itr14->first;
									
									//addnumber(con4, 30);
									if (!PushHus(con1 + con2 + con3 + con4, i*pow(5, 4) + j*pow(5, 3) + k*pow(5, 2) + l*pow(5, 1))){
										continue;
									}
									int len4 = 5 - i - j - k - l;
									for (int m = 0; m < len4; m++){
										map<string, char> *vec5 = &allmps[4][m];
										auto itr15 = vec5->begin();
										for (itr15; itr15 != vec5->end(); itr15++){
											string con5 = itr15->first;
											//addnumber(con5, 40);
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
	
	//写入index
	char buff[30];
	auto tt = m_husindex.begin();
	string ttf;
	FILE *file = fopen("./res/ss/index", "a+");
	for (tt; tt != m_husindex.end();tt++){
		int di = tt->first;
		sprintf(buff,"%d\n",di);
		fprintf(file,buff);
	}
	fclose(file);
	
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			allmps[i][j].clear();
			map<string, char>mm;
			allmps[i][j].swap(mm);
		}
	}
	g_index = 0;
	
	auto itrr = m_zjallmaps.begin();
	for (itrr; itrr != m_zjallmaps.end();){
		int index = itrr->first;
		for (int i = 0; i < g_kind; i++){
			int type = g_all_mjkind[i] / 16;
			int va = g_all_mjkind[i] % 16;
			char a = va + type * 10 + '0';
			map<string, char> *vec = &itrr->second;
			auto it1 = vec->begin();
			for (it1; it1 != vec->end(); it1++){
				string content = it1->first;
				inserDui(content, a);
				if (content.length() % 3 != 0){
					PushHus1(content, index);
				}
			}
		}
		itrr->second.clear();
		map<string, char>mm;
		itrr->second.swap(mm);
		itrr = m_zjallmaps.erase(itrr);

		int ind = m_husindex.at(index);
		auto vec = &m_zjallmaps1[ind];
		auto ir1 = vec->begin();
		sprintf(buff, "./res/ss/hu%d", ind);
		FILE *file = fopen(buff, "r");
		if (!file){
			file = fopen(buff, "a+");
			int index1 = 0;
			int count = 10;
			int ii = 0;
			string con;
			for (ir1; ir1 != vec->end(); ir1++){
				con += ir1->first;
				con+="\n";
				if (index1 == count){
					printf("[%d].wirte data\n", ii + 1);
					fprintf(file, con.c_str());
					con = "";
					index1 = 0;
					ii++;
				}
				index1++;

			}
			fclose(file);
		}
		vec->clear();
		vec->swap(mm);
		
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
	if (count(content.begin(), content.end(), va)>2){
		return content;
	}
	content += va;
	content += va;
	sort(content.begin(), content.end());
	Compress(content);
	return content;
}

bool ConfigData::Over4(string &content){
	if (content.find('12') != -1 || content.find('5') != -1 || content.find('6') != -1 || content.find('7') != -1
		|| content.find('8') != -1 || content.find('9') != -1 || content.find('10') != -1 || content.find('11') != -1){
		return true;
	}
	return false;
}

bool ConfigData::Over41(string &content){
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


bool ConfigData::PushHus(string &content, int index){
	if (m_husindex.find(index) == m_husindex.end()){
		m_husindex.insert(make_pair(index, m_husindex.size()));
	}
	if (m_zjallmaps.find(index) != m_zjallmaps.end()){
		map<string, char> *mps = &m_zjallmaps.at(index);
		if (mps->find(content) == mps->end()){
			mps->insert(make_pair(content, '\0'));
			printf("%d.%s\n", g_index++, content.c_str());
		}
	}
	else{
		map<string, char> mps;
		mps.insert(make_pair(content, '\0'));
		m_zjallmaps.insert(make_pair(index,mps));
		printf("%d.%s\n", g_index++, content.c_str());
	}
	
	
	return true;
}

bool ConfigData::PushHus1(string &content, int index){
	int ind = m_husindex.at(index);
	map<string, char> *mps = &m_zjallmaps1[ind];
	if (mps->find(content) == mps->end()){
		mps->insert(make_pair(content, '\0'));
		printf("%d.%s\n", g_index++, content.c_str());
	}

	return true;
}