#ifndef __XXICONV_H__
#define __XXICONV_H__

#include "stdafx.h"
#include <iostream>




class XXIconv
{
public:
	static std::string GBK2UTF(const char * str);
	static std::string UTF2GBK(const char * str);
	//��ȡ���еļ�������,���ಿ����"..."��ʾ
	static std::string getU8SubString(std::string u8_str,int sBegin,int strnum,const char *ext = "...");
private:
	static bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);
};


#endif
