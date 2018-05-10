// LogFile.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __CLog_H__
#define __CLog_H__
#include "stdafx.h"

#define console_print(ch)    putchar(ch)//SendOneByte(ch)

class CLog
{
public:
	CLog();
	virtual~CLog();
public:
	static string getFile();
	static void log(const char* fmt, ...);
	

	static void    printch(char ch,bool other=false);
	static void    printdec(int dec);
	static void	   printflt(double flt);
	static void    printstr(char* str);
	static void    printbin(int bin);

	static void    printhex(int hex);
private:
	static FILE* m_file;
	static string m_filepath;
	static string m_content;
	static string m_tt;
	static string m_num;
	static string m_ontstr;
};
#endif// !defined(AFX_LOGFILE_H__DD3CDF99_1639_4506_88F4_887CFD1DE775__INCLUDED_)