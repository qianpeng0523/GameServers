#include "HttpEvent.h"
#include "MD5.h"
#include "curl.h"
#include "HttpLogic.h"
#include "Common.h"

#define WECHAT_APPID "wx73057982ca59e10c"
#define WECHAT_APPSECRET "021Odmeo0sPwHo1Nqxco0Glneo0OdmeH"

HttpEvent *HttpEvent::m_ins = NULL;

HttpEvent::HttpEvent()
{	
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
}

HttpEvent::~HttpEvent()
{
	if (m_httpd){
		evhttp_free(m_httpd);
	}
}


std::vector<std::string> getStrings(std::string str, std::string findstr){
	std::vector<std::string> vecs;
	int index = str.find(findstr);
	int len = findstr.length();
	while (index != -1){
		std::string temp;
		temp = str.substr(0, index);
		if (!temp.empty()){
			vecs.push_back(temp);
		}
		str = str.substr(index + len, str.length());
		index = str.find(findstr);
	}
	vecs.push_back(str);
	return vecs;
}

HttpEvent* HttpEvent::getIns(){
	if (!m_ins){
		m_ins = new HttpEvent();
	}
	return m_ins;
}


size_t read_data(void* buffer, size_t size, size_t nmemb, void *stream)
{
	string *content = (string *)stream;
	content->append((char*)buffer, size * nmemb);
	return size*nmemb;
}

//处理模块
void httpd_handler(struct evhttp_request *req, void *arg) {
	
	//HTTP header
	evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");
	//输出的内容
	
	//获取客户端请求的URI(使用evhttp_request_uri或直接req->uri)
	const char *uri;
	uri = evhttp_request_uri(req);
	string str = uri;
	str = str.substr(1,str.length());
	str = Common::replace_all(str, REPLACESTRR,"\r" );
	str = Common::replace_all(str, REPLACESTRN,"\n");
	HttpEvent::getIns()->EventDispath(req, str);
}

void HttpEvent::EventDispath(struct evhttp_request *&req, string uri){
	m_req = req;
	YMSocketData sd = getSocketDataByStr(uri,uri.length());
	//通过url分写逻辑
	SendMsg(sd,req);
	
}

void HttpEvent::SendMsg(YMSocketData &sd, struct evhttp_request *req){
	struct evbuffer *buf;
	buf = evbuffer_new();
	char *packBuffer = (char *)malloc(4096);
	int packSize = 0;
	HttpLogic::getIns()->HandleLogic(sd, packBuffer, packSize);
	evbuffer_add(buf, packBuffer, packSize);
	if (req){
		evhttp_send_reply(req, HTTP_OK, "OK", buf);
	}
	else{
		evhttp_send_reply(m_req, HTTP_OK, "OK", buf);
	}
	evbuffer_free(buf);
	free(packBuffer);
}

void HttpEvent::init(){
	//默认参数
	char *httpd_option_listen = "0.0.0.0";
	int httpd_option_port = 8080;
	int httpd_option_timeout = 120; //in seconds
	printf("http start port:%d\n",httpd_option_port);
	/* 使用libevent创建HTTP Server */

	//初始化event API
	event_init();

	//创建一个http server
	
	m_httpd = evhttp_start(httpd_option_listen, httpd_option_port);
	if (!m_httpd){
		printf("端口被占用\n");
		return;
	}
	evhttp_set_timeout(m_httpd, httpd_option_timeout);

	//指定generic callback
	evhttp_set_gencb(m_httpd, httpd_handler, NULL);

	//如果大厅是短连接，则需要短连接带上token

	//也可以为特定的URI指定callback
	//evhttp_set_cb(httpd, "/", specific_handler, NULL);

	//循环处理events
	event_dispatch();

	evhttp_free(m_httpd);

}

YMSocketData HttpEvent::getSocketDataByStr(string str,int sz){
	YMSocketData sd;
	if (str.empty()){
		return sd;
	}
	sd.parse((char *)str.c_str(), sz);

	return sd;
}