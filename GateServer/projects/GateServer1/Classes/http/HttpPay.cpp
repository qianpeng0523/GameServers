#include "HttpPay.h"
#include "HttpEvent.h"
#include "XmlConfig.h"
#include "redis.h"
#include "Common.h"
#include "StatTimer.h"

HttpPay *HttpPay::m_Ins = NULL;



HttpPay::HttpPay(){
	int len = 0;
	char *dd = redis::getIns()->get("notime", len);
	if (dd){
		m_lasttime = dd;
		
	}
	else{
		m_lasttime = Common::getLocalTimeDay();
		redis::getIns()->set("notime", (char *)m_lasttime.c_str(), m_lasttime.length());
	}
	m_isopen = false;
	openUpdate(true);
}
HttpPay::~HttpPay(){
	openUpdate(false);
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

void HttpPay::update(float dt){
	string time = Common::getLocalTimeDay();
	if (time.compare(m_lasttime) != 0){
		m_lasttime = time;
		redis::getIns()->set("nonceid", INITNONCEID, strlen(INITNONCEID));
		redis::getIns()->set("outtradeno", INITNO, strlen(INITNO));
	}
}

void HttpPay::openUpdate(bool isopen){
	if (isopen&&!m_isopen){
		m_isopen = true;
		StatTimer::getIns()->scheduleSelector(this, schedule_selector(HttpPay::update), 0.5);
	}
	else if(!isopen&&m_isopen){
		m_isopen = false;
		StatTimer::getIns()->unscheduleSelector(this, schedule_selector(HttpPay::update));
	}
}

string HttpPay::getNonceId(){
	string time = Common::getLocalTimeDay1();
	int len = 0;
	char buff[50];
	char *dd = redis::getIns()->get("nonceid",len);
	if (!dd){
		time += INITNONCEID;
		redis::getIns()->set("nonceid", INITNONCEID, strlen(INITNONCEID));
	}
	else{
		sprintf(buff,"%08d",atoi(dd)+1);
		time += buff;
		redis::getIns()->set("nonceid", buff, strlen(buff));
	}
	MD5 md5;
	md5.update(time);
	string nonceid = md5.toString();
	transform(nonceid.begin(), nonceid.end(), nonceid.begin(), toupper);
	return time;
}

string HttpPay::getOutTradeNo(){
	string time = Common::getLocalTimeDay1()+"YLHD";
	int len = 0;
	char buff[50];
	char *dd = redis::getIns()->get("outtradeno", len);
	if (!dd){
		time += INITNO;
		redis::getIns()->set("outtradeno", INITNO, strlen(INITNO));
	}
	else{
		sprintf(buff, "%08d", atoi(dd)+1);
		time += buff;
		redis::getIns()->set("outtradeno", buff, strlen(buff));
	}
	return time;
}

size_t read_data1(void* buffer, size_t size, size_t nmemb, void *stream)
{
	string *result = (string *)stream;
	result->append((char *)buffer);
	return size*nmemb;
}

void HttpPay::respond(map<string, string> maps, map<string, string> ordermap){
	for (auto itr = maps.begin(); itr != maps.end(); itr++){
		printf("%s:%s\n",itr->first.c_str(),itr->second.c_str());
	}
	if (maps.find("prepay_id")!=maps.end()){
		//下单
		string prepay_id = maps.at("prepay_id");
		string shopid = ordermap.at("attach");
		string out_trade_no = ordermap.at("out_trade_no");
		string uid = ordermap.at("userid");
		string starttime = ordermap.at("time_start");
		string endtime = ordermap.at("time_expire");
		//记录下单，支付结果收到删除
		//支付状态 等待，失效，支付成功（给购买的商品，删除）
		string status = XXIconv::GBK2UTF("下单成功，等待支付");

		//定时3分钟查询一次
	}
	else if (maps.find("openid")!=maps.end()){
		//支付结果通知
		string shopid = maps.find("attach")->second;
		string openid = maps.find("openid")->second;
		string sign = maps.find("sign")->second;//回调给微信验证
		//通过openid获取uid
		string uid;
		string out_trade_no = maps.find("out_trade_no")->second;
		//out_trade_no查询是否下发购买的商品

	}
	else{
		//不做处理
		//支付查询 成功则和支付结果通知接口一样
		string out_trade_no = maps.find("out_trade_no")->second;
		//等待支付结果
	}
}

void HttpPay::requestOrder(string uid, string shopid, int price, string body, string ip){
	map<string, string> valuemap;
	valuemap.insert(make_pair("appid", APPID));
	valuemap.insert(make_pair("body", body));
	valuemap.insert(make_pair("mch_id", MCHID));
	valuemap.insert(make_pair("attach", shopid));
	valuemap.insert(make_pair("nonce_str", getNonceId()));
	valuemap.insert(make_pair("notify_url", NOTIFYURL));
	valuemap.insert(make_pair("out_trade_no", getOutTradeNo()));
	valuemap.insert(make_pair("spbill_create_ip", ip));

	time_t time = Common::getTime();
	string starttime = Common::getTimeStr(time);
	string endtime = Common::getTimeStr(time+2*60*60);
	valuemap.insert(make_pair("time_start", starttime));
	valuemap.insert(make_pair("time_expire", endtime));
	char buff[30];
	sprintf(buff,"%d",price);
	valuemap.insert(make_pair("total_fee", buff));
	valuemap.insert(make_pair("trade_type", TRADETYPE));
	string sign = createSign(valuemap);
	valuemap.insert(make_pair("sign", sign));
	string xml = XmlConfig::getIns()->setXmlData(valuemap);
	string url = "https://api.mch.weixin.qq.com/pay/unifiedorder";
	valuemap.insert(make_pair("userid",uid));
	HttpEvent::getIns()->requestData(url, xml.c_str(), read_data1,valuemap);
}

void HttpPay::test(){
	requestOrder("10001",2,XXIconv::GBK2UTF("支付测试"),"27.46.6.74");
}

string HttpPay::createSign(map<string, string> valuemap){
	string signA;
	for (auto itr = valuemap.begin(); itr != valuemap.end();itr++){
		string name = itr->first;
		string value = itr->second;
		if (itr != valuemap.begin()){
			signA += "&";
		}
		signA += name+"="+value;
	}
	printf("%s\n",signA.c_str());
	signA += "&key=19890523ylhdwlkjyxgs201804112012";
	MD5 md5;
	md5.update(signA);
	string sign = md5.toString();
	transform(sign.begin(), sign.end(), sign.begin(), toupper);
	return sign;
}