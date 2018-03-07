#include "XXIconv.h"

#define UTF8COMPUTE(Char, Mask, Len)        \
	if (Char < 128)                \
{                        \
	Len = 1;                    \
	Mask = 0x7f;                \
}                        \
else if ((Char & 0xe0) == 0xc0)        \
{                        \
	Len = 2;                    \
	Mask = 0x1f;                \
}                        \
else if ((Char & 0xf0) == 0xe0)        \
{                        \
	Len = 3;                    \
	Mask = 0x0f;                \
}                        \
else if ((Char & 0xf8) == 0xf0)        \
{                        \
	Len = 4;                    \
	Mask = 0x07;                \
}                        \
else if ((Char & 0xfc) == 0xf8)        \
{                        \
	Len = 5;                    \
	Mask = 0x03;                \
}                        \
else if ((Char & 0xfe) == 0xfc)        \
{                        \
	Len = 6;                    \
	Mask = 0x01;                \
}                        \
else                        \
	Len = -1;

#define UTF8LENGTH(Char)            \
	((Char) < 0x80 ? 1 :                \
	((Char) < 0x800 ? 2 :            \
	((Char) < 0x10000 ? 3 :            \
	((Char) < 0x200000 ? 4 :            \
	((Char) < 0x4000000 ? 5 : 6)))))


bool XXIconv::IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	iconv_t cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		return false;
	}

	const char **pin = &inbuf;
	char **pout = &outbuf;
	memset(outbuf, 0, outlen);
	size_t ret = iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen);

	iconv_close(cd);
	return ret == (size_t)(-1) ? false : true;
#else
	return true;
#endif
}

std::string XXIconv::GBK2UTF(const char * str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	char *textOut = new char[strlen(str) * 4];
	bool ret = IConvConvert("gb2312", "utf-8", str, strlen(str),textOut, strlen(str) * 4);
	//std::string outString = ret ? std::string(textOut) : std::string();
	std::string outString = ret ? std::string(textOut) : std::string(str);

	delete[] textOut;

	return outString;
#else
	return std::string(str);
#endif
}

std::string XXIconv::UTF2GBK(const char * str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	char *textOut = new char[strlen(str) / 4];
	bool ret = IConvConvert("utf-8","gb2312" , str, strlen(str),textOut, strlen(str) / 4);
	std::string outString = ret ? std::string(textOut) : std::string();
	delete[] textOut;

	return outString;
#else
	return std::string(str);
#endif
}
//UTF-8 coding
//unicode:0x00000000 - 0x0000007F:
//0xxxxxxx
//unicode:0x00000080 - 0x000007FF:
//110xxxxx 10xxxxxx
//unicode:0x00000800 - 0x0000FFFF:
//1110xxxx 10xxxxxx 10xxxxxx
//unicode:0x00010000 - 0x001FFFFF:
//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

std::string XXIconv::getU8SubString(std::string u8_str,int sBegin,int strnum,const char *ext)
{
	std::string::const_iterator iStr = u8_str.begin();
	int strNo = 0;//chinese number

	int byteBegin = -1;//字节开始处
	int byteNum = 0;//字节数
	int byteSize = 0;//每一个UTF8字的,字节大小
	int byteMask = 0;

	while(iStr != u8_str.end() )
	{	
		UTF8COMPUTE((unsigned char)*iStr,byteMask,byteSize);

		iStr += byteSize;

		//record range
		if ( strNo == sBegin )
		{
			byteBegin = iStr - u8_str.begin();
		}


		if( byteSize > 0 )
		{
			strNo ++;
			if(byteBegin >= 0)
			{
				strnum -= 1;
				if( strnum <= 0)
				{
					break;
				}
			}			
		}	
		
	}
	byteNum = iStr - u8_str.begin() - byteBegin;
	//汉字个数大于要截取的个数
	if( iStr != u8_str.end() )
	{
		return u8_str.substr(byteBegin,byteNum).append(ext);
	}
	else
	{
		return u8_str.substr(byteBegin,byteNum);
	}	
}
