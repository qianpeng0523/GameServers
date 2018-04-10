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

void HttpPay::requestOrder(string xml){
	/*
	
	<xml>
	<appid>wx2421b1c4370ec43b</appid>
	<attach>÷ß∏∂≤‚ ‘</attach>
	<body>APP÷ß∏∂≤‚ ‘</body>
	<mch_id>10000100</mch_id>
	<nonce_str>1add1a30ac87aa2db72f57a2375d8fec</nonce_str>
	<notify_url>http://wxpay.wxutil.com/pub_v2/pay/notify.v2.php</notify_url>
	<out_trade_no>1415659990</out_trade_no>
	<spbill_create_ip>14.23.150.211</spbill_create_ip>
	<total_fee>1</total_fee>
	<trade_type>APP</trade_type>
	<sign>0CB01533B8C1EF103065174F50BCA001</sign>
	</xml>


	
	*/
	string url = "https://api.mch.weixin.qq.com/pay/unifiedorder";
	HttpEvent::getIns()->requestData(url, xml);
}