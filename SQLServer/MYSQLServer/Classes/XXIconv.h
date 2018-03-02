#ifndef __XXICONV_H__
#define __XXICONV_H__

#include "stdafx.h"
#include <iostream>


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "iconv.h"
#pragma comment(lib,"../MYSQLServer/Debug/libiconv.lib") 
#endif

class XXIconv
{
public:
	static std::string GBK2UTF(const char * str);
	static std::string UTF2GBK(const char * str);
	//截取其中的几个汉字,多余部分用"..."表示
	static std::string getU8SubString(std::string u8_str,int sBegin,int strnum,const char *ext = "...");
private:
	static bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);
};


#endif
