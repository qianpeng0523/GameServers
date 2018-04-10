#include "HttpPay.h"
#include "HttpEvent.h"

HttpPay *HttpPay::m_Ins = NULL;



HttpPay::HttpPay(){
	//this->scheduleUpdate(0.5, 1.0);
}
HttpPay::~HttpPay(){
	
}

bool HttpPay::init()
{
	
    return true;
}

HttpPay *HttpPay::getIns(){
	if (!m_Ins){
		m_Ins = new HttpPay();
		m_Ins->init();
	}
	return m_Ins;
}