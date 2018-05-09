// LogFile.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////
#include "CLog.h"
#include "Common.h"
#include "WinBase.h"

FILE* CLog::m_file = NULL;
string CLog::m_filepath = "";
string CLog::m_content = "";
string CLog::m_tt = "";
string CLog::m_num = "";
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
	string strFileName;
	strFileName = Common::getLocalTimeDay() + ".log";

	return strFileName;
}


void CLog::log(const char* fmt, ...){
	m_content = "";
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
			char cc;
			while (!ist){
				switch (*(++pfmt))
				{

				case 'c':
					vargch = va_arg(vp, int);

					printch(vargch);
					ist = true;
					break;
				case 'd':
				case 'i':
					vargint = va_arg(vp, int);
					printdec(vargint);
					ist = true;
					break;
				case 'f':
					vargflt = va_arg(vp, double);
					printflt(vargflt);
					ist = true;
					break;
				case 's':
					vargpch = va_arg(vp, char*);
					printstr(vargpch);
					ist = true;
					break;
				case 'b':
				case 'B':
					vargint = va_arg(vp, int);
					printbin(vargint);
					ist = true;
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, int);
					printhex(vargint);
					ist = true;
					break;
				case '%':
					printch('%');
					ist = true;
					break;
				default:
					cc = *pfmt;
					if (cc == '0'){
						m_tt += cc;
					}
					else{
						m_num += cc;
					}
					break;
				}
				pfmt++;
			}
		}
		else
		{
			printch(*pfmt++);
		}
	}
	va_end(vp);
	printf("%s\n",m_content.c_str());
}


void  CLog::printch(char ch)
{
	if (m_tt.empty() && m_num.empty()){
		m_content += ch;
	}
	else if (!m_num.empty()){

	}
	//console_print(ch);
}

void CLog::printdec(int dec)
{
	if (dec == 0)
	{
		return;
	}
	printdec(dec / 10);
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
	if (bin == 0)
	{
		printstr("0b");
		return;
	}
	printbin(bin / 2);
	printch((char)(bin % 2 + '0'));
}

void CLog::printhex(int hex)
{
	if (hex == 0)
	{
		printstr("0x");
		return;
	}
	printhex(hex / 16);
	if (hex < 10)
	{
		printch((char)(hex % 16 + '0'));
	}
	else
	{
		printch((char)(hex % 16 - 10 + 'a'));
	}
}