#ifndef __HttpPay_SCENE_H__
#define __HttpPay_SCENE_H__

#include "stdafx.h"

class HttpPay:public Object
{
public:
	HttpPay();
	~HttpPay();
    virtual bool init();
	static HttpPay *getIns();
	
public:
	
private:
	static HttpPay *m_Ins;
	
public:
	
};

#endif 