// LogFile.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////
#include "CLog.h"
#include "Common.h"
#include "stdarg.h"

FILE* CLog::m_file = NULL;
string CLog::m_filepath = "";
string CLog::m_content = "";
string CLog::m_tt = "";
string CLog::m_num = "";
string CLog::m_ontstr = "";
CLog::CLog()
{
	m_filepath = CLog::getFile();
}
CLog::~CLog()
{
}
//获取日志文件名
string CLog::getFile()
{
	string strFileName="./res/gate_";
	strFileName += Common::getLocalTimeDay() + ".log";

	return strFileName;
}


void CLog::log(const char* fmt, ...){
	m_content = "【";
	m_content += Common::getLocalTime();
	m_content += "】";
	double vargflt = 0;
	int  vargint = 0;
	char* vargpch = NULL;
	char vargch = 0;
	char* pfmt = NULL;
	va_list vp;

	va_start(vp, fmt);
	pfmt = (char*)fmt;

	while (*pfmt)
	{
		if (*pfmt == '%')
		{
			bool ist = false;
			m_tt="";
			m_num="" ;
			m_ontstr = "";
			char dd='0';
			char cc;
			while (!ist){
				switch (*(++pfmt))
				{

				case 'c':
					vargch = va_arg(vp, int);
					dd = 'c';
					printch(vargch);
					ist = true;
					break;
				case 'd':
				case 'i':
					vargint = va_arg(vp, int);
					dd = 'd';
					printdec(vargint);
					ist = true;
					break;
				case 'f':
					vargflt = va_arg(vp, double);
					dd = 'f';
					printflt(vargflt);
					ist = true;
					break;
				case 's':
					vargpch = va_arg(vp, char*);
					dd = 's';
					printstr(vargpch);
					ist = true;
					break;
				case 'b':
				case 'B':
					vargint = va_arg(vp, int);
					dd = 'b';
					printbin(vargint);
					ist = true;
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, int);
					dd = 'X';
					printhex(vargint);
					ist = true;
					break;
				case '%':
					printch('%');
					ist = true;
					break;
				default:
					cc = *pfmt;
					if (cc == '0'||cc=='.'){
						m_tt += cc;
					}
					else{
						m_num += cc;
					}
					break;
				}
			}
			{
				bool isf = false;
				int len = m_ontstr.length();
				if (dd == 'd' || dd == 'X'){
					isf = false;
				}
				else if (dd == 'f'){
					isf = true;
					Common::replace_all(m_tt, "0", "");
					Common::replace_all(m_tt, ".", "0");
					int index = m_ontstr.find(".");
					len = len-index-1;
				}
				else{
					isf = false;
					Common::replace_all(m_tt, "0", " ");
				}
				string ct;
				int l = atoi(m_num.c_str());
				if (l > 0 && m_tt.empty()){
					m_tt = " ";
				}
				if (!isf){
					for (int i = 0; i < l - len; i++){
						ct += m_tt;
					}
					ct += m_ontstr;
				}
				else{
					ct += m_ontstr;
					for (int i = 0; i < l - len; i++){
						ct += m_tt;
					}
				}
				m_content += ct;
			}
			pfmt++;
		}
		else
		{
			printch(*pfmt++,true);
		}
	}
	
	va_end(vp);
	if (m_content.find("\n") == -1){
		m_content += "\n";
	}
	printf("%s",m_content.c_str());
	string file = getFile();
	if (!m_file){
		m_filepath = file;
		m_file = fopen(m_filepath.c_str(), "a+");
		fseek(m_file, 0, SEEK_END);
	}
	else{
		if (m_filepath.compare(file) != 0){
			fclose(m_file);
			m_filepath = file;
			m_file = fopen(m_filepath.c_str(), "a+");
			fseek(m_file, 0, SEEK_END);
		}
	}
	fprintf(m_file,m_content.c_str());
	fflush(m_file);
}


void  CLog::printch(char ch, bool other){
	if (other){
		m_content += ch;
	}
	else{
		m_ontstr += ch;
	}
	
	//console_print(ch);
}

void CLog::printdec(int dec)
{
	int hh = dec / 10;
	if (hh>0)
	{
		printdec(dec / 10);
	}
	printch((char)(dec % 10 + '0'));
	
}

void CLog::printflt(double flt)
{
	int icnt = 0;
	int tmpint = 0;

	tmpint = (int)flt;
	printdec(tmpint);
	printch('.');
	flt = flt - tmpint;
	tmpint = (int)(flt * 1000000);
	printdec(tmpint);
}

void CLog::printstr(char* str)
{
	while (*str)
	{
		printch(*str++);
	}
}

void CLog::printbin(int bin)
{
	int hh = bin/2;
	if (hh>0)
	{
		printbin(bin / 2);
	}
	printch((char)(bin % 2 + '0'));
}

void CLog::printhex(int hex)
{
	int hh = hex / 16;
	if (hh > 0){
		printhex(hex / 16);
	}

	if (hex < 10||hex%16<10)
	{
		printch((char)(hex % 16 + '0'));
	}
	else
	{
		int a = hex % 16 - 10 + 'A';
		char b = a;
		printch(b);
	}
}