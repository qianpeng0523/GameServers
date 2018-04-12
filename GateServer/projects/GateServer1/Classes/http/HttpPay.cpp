#include "HttpPay.h"
#include "HttpEvent.h"
#include "XmlConfig.h"
#include "redis.h"
#include "Common.h"
#include "StatTimer.h"
#include "RedisGet.h"
#include "LibEvent.h"
HttpPay *HttpPay::m_Ins = NULL;
// 
// string prepay_id = maps.at("prepay_id");
// string shopid = ordermap.at("attach");
// string out_trade_no = ordermap.at("out_trade_no");
// string uid = ordermap.at("userid");
// string starttime = ordermap.at("time_start");
// string endtime = ordermap.at("time_expire");

string payrecord[] = {"prepay_id","attach","out_trade_no","userid","time_start","time_expire","body","total_fee","spbill_create_ip"};

HttpPay::HttpPay(){
	m_count = 0;
	m_pRedis = redis::getIns();
	int len = 0;
	char *dd = redis::getIns()->get("notime", len);
	if (dd){
		m_lasttime = Common::getLocalTimeDay();
		if (m_lasttime.compare(dd) != 0){
			m_pRedis->set("notime", (char *)m_lasttime.c_str(), m_lasttime.length());
		}
	}
	else{
		m_lasttime = Common::getLocalTimeDay();
		m_pRedis->set("notime", (char *)m_lasttime.c_str(), m_lasttime.length());
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
		m_pRedis->set("nonceid", INITNONCEID, strlen(INITNONCEID));
		m_pRedis->set("outtradeno", INITNO, strlen(INITNO));
	}
	if (m_count % 360 == 0){
		checkPay();
	}
	m_count++;
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
	char *dd = m_pRedis->get("nonceid", len);
	if (!dd){
		time += INITNONCEID;
		m_pRedis->set("nonceid", INITNONCEID, strlen(INITNONCEID));
	}
	else{
		sprintf(buff,"%08d",atoi(dd)+1);
		time += buff;
		m_pRedis->set("nonceid", buff, strlen(buff));
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
	char *dd = m_pRedis->get("outtradeno", len);
	if (!dd){
		time += INITNO;
		m_pRedis->set("outtradeno", INITNO, strlen(INITNO));
	}
	else{
		sprintf(buff, "%08d", atoi(dd)+1);
		time += buff;
		m_pRedis->set("outtradeno", buff, strlen(buff));
	}
	return time;
}

size_t read_data1(void* buffer, size_t size, size_t nmemb, void *stream)
{
	string *result = (string *)stream;
	result->append((char *)buffer);
	return size*nmemb;
}

void HttpPay::respondResult(string content, struct evhttp_request *req){
	map<string, string>maps = XmlConfig::getIns()->parseXmlData(content);
	for (auto itr = maps.begin(); itr != maps.end(); itr++){
		printf("%s:%s\n", itr->first.c_str(), itr->second.c_str());
	}
	string returncode = maps.find("return_code")->second;
	string resultcode = maps.find("result_code")->second;
	if (returncode.compare("SUCCESS") == 0 || resultcode.compare("SUCCESS") == 0){
		//支付结果通知
		string shopid = maps.find("attach")->second;
		string openid = maps.find("openid")->second;
		string sign = maps.find("sign")->second;//回调给微信验证
		//通过openid获取uid
		string uid;
		string con;
		map<string, string>tt;
		auto itr = maps.find("out_trade_no");
		if (itr != maps.end()){
			string out_trade_no = itr->second;
			string totalfree = maps.at("total_fee");
			ShopItem si = RedisGet::getIns()->getShop(atoi(shopid.c_str()));

			if (si.consume().number() == atoi(totalfree.c_str())){
				//校验成功
				int len = m_pRedis->eraseList("out_trade_no", out_trade_no);
				if (len > 0){
					maps.erase(maps.find("sign"));
					string nowsign = createSign(maps);
					if (nowsign.compare("sign") == 0){
						//out_trade_no查询是否下发购买的商品
						tt.insert(make_pair("return_code", "SUCCESS"));
						tt.insert(make_pair("return_msg", "OK"));
					}
					else{
						tt.insert(make_pair("return_code", "FAIL"));
						tt.insert(make_pair("return_msg", XXIconv::GBK2UTF("签名失败")));
					}
				}
				else{
					//说明已经收到一次通知或者查询了
					tt.insert(make_pair("return_code", "SUCCESS"));
					tt.insert(make_pair("return_msg", "OK"));
				}
			}
			else{
				//校验失败
				tt.insert(make_pair("return_code", "FAIL"));
				tt.insert(make_pair("return_msg", XXIconv::GBK2UTF("total_fee订单金额与商户侧的订单金额不一致")));
			}

		}
		else{
			tt.insert(make_pair("return_code", "FAIL"));
			tt.insert(make_pair("return_msg", XXIconv::GBK2UTF("参数格式校验错误")));
		}
		con = XmlConfig::getIns()->setXmlData1(tt);
		if (req){
			HttpEvent::getIns()->SendMsg(con, req);
		}
	}
	else{
		printf("%s\n", XXIconv::GBK2UTF("格式错误或签名错误").c_str());
	}
}

void HttpPay::requestCheck(string xml){
	string url = "https://api.mch.weixin.qq.com/pay/orderquery";
	string content= HttpEvent::getIns()->requestData(url, xml.c_str(), read_data1);
	respondCheck(content);
}

void HttpPay::respondCheck(string content){
	map<string, string>maps = XmlConfig::getIns()->parseXmlData(content);
	for (auto itr = maps.begin(); itr != maps.end(); itr++){
		printf("%s:%s\n", itr->first.c_str(), itr->second.c_str());
	}
	string returncode = maps.find("return_code")->second;
	string resultcode = maps.find("result_code")->second;
	if (returncode.compare("SUCCESS") == 0||resultcode.compare("SUCCESS")==0){
		//不做处理
		//支付查询 成功则和支付结果通知接口一样
		string out_trade_no = maps.find("out_trade_no")->second;
		auto itr = maps.find("trade_state");
		if (itr != maps.end()){
			string statusstr = itr->second;
			if (statusstr.compare("NOTPAY") == 0 || statusstr.compare("CLOSED") == 0
				|| statusstr.compare("REVOKED") == 0 || statusstr.compare("PAYERROR") == 0){
				m_pRedis->eraseList("out_trade_no", out_trade_no);
			}
			else if (itr->second.compare("SUCCESS") == 0){
				respondResult(content);
			}
		}
		//等待支付结果
	}
	else{
		//格式错误，签名错误不管
		printf("%s\n",XXIconv::GBK2UTF("格式错误或签名错误").c_str());
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
	valuemap.insert(make_pair("out_trade_no", XXIconv::GBK2UTF(getOutTradeNo().c_str())));
	valuemap.insert(make_pair("spbill_create_ip", ip));

	time_t time = Common::getTime();
	string starttime = Common::getTimeStr(time);
	string endtime = Common::getTimeStr(time+2*60);
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
	string content= HttpEvent::getIns()->requestData(url, xml.c_str(), read_data1);
	respondOrder(content, valuemap);
}

void HttpPay::respondOrder(string content, map<string, string> ordermap){
	map<string, string>maps = XmlConfig::getIns()->parseXmlData(content);
	for (auto itr = maps.begin(); itr != maps.end(); itr++){
		printf("%s:%s\n", itr->first.c_str(), itr->second.c_str());
	}
	string uid = ordermap.find("userid")->second;
	ClientData *data = LibEvent::getIns()->getClientData(uid);
	if (data){
		int fd = data->_fd;
		string returncode = maps.find("return_code")->second;
		string resultcode = maps.find("result_code")->second;
		if (returncode.compare("SUCCESS") == 0 || resultcode.compare("SUCCESS") == 0){
			//下单
			map<string, string> record;
			for (int i = 0; i < 9; i++){
				string v = payrecord[i];
				for (auto itr = maps.begin(); itr != maps.end(); itr++){
					if (v.compare(itr->first) == 0){
						record.insert(make_pair(v, itr->second));
					}
				}
				for (auto itr = ordermap.begin(); itr != ordermap.end(); itr++){
					if (v.compare(itr->first) == 0){
						record.insert(make_pair(v, itr->second));
					}
				}
			}
			PayRecord pr1;
			PayRecord *pr = (PayRecord *)m_pRedis->PopDataFromRedis(pr1.GetTypeName(), record);
			m_pRedis->List(pr1.GetTypeName(), pr);
			delete pr;
			//记录下单，支付结果收到删除
			//支付状态 等待，失效，支付成功（给购买的商品，删除）
			//string status = XXIconv::GBK2UTF("下单成功，等待支付");
			string out_trade_no = ordermap.find("out_trade_no")->second;
			m_pRedis->List("out_trade_no", (char *)out_trade_no.c_str());
			//定时3分钟查询一次
		}
		else{
			//下单失败,返回客户端信息
		}
	}
}

void HttpPay::test(){
	requestOrder("10001","2",1,XXIconv::GBK2UTF("支付测试"),"27.46.6.74");
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

void HttpPay::checkPay(){
	vector<int> lens;
	vector<char *> vecs = m_pRedis->getList("out_trade_no", lens);
	for (int i = 0; i < vecs.size();i++){
		char* out1 = vecs.at(i);
		string out = out1;
		printf("out:%s", XXIconv::GBK2UTF(out.c_str()).c_str());
		map<string, string> valuemap;
		valuemap.insert(make_pair("appid", APPID));
		valuemap.insert(make_pair("mch_id", MCHID));
		valuemap.insert(make_pair("nonce_str", getNonceId()));
		valuemap.insert(make_pair("out_trade_no", XXIconv::GBK2UTF(out.c_str())));
		string sign = createSign(valuemap);
		valuemap.insert(make_pair("sign", sign));
		string xml = XmlConfig::getIns()->setXmlData(valuemap);
		requestCheck(xml);
		delete out1;
	}
}