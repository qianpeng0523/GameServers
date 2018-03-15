#include "HttpEvent.h"
#include "Common.h"
#include "MD5.h"
#include "curl/include/curl.h"
#include "HttpLogic.h"



HttpEvent *HttpEvent::m_ins = NULL;

HttpEvent::HttpEvent()
{	
	
}

HttpEvent::~HttpEvent()
{
	
}


HttpEvent* HttpEvent::getIns(){
	if (!m_ins){
		m_ins = new HttpEvent();
	}
	return m_ins;
}

size_t read_data(void* buffer, size_t size, size_t nmemb, void *stream)
{
	vector<char> *vec = (vector<char> *)stream;
	for (int i = 0; i < size*nmemb;i++){
		vec->push_back(((char*)buffer)[i]);
	}
	return size*nmemb;
}


//处理模块
void httpd_handler(struct evhttp_request *req, void *arg) {

	//HTTP header
	evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");
	//输出的内容

	string ip = req->remote_host;
	int port = req->remote_port;
	printf("requset:%s:%d\n", ip.c_str(), port);

	struct evbuffer *buffer = req->input_buffer;
	int sz = EVBUFFER_LENGTH(buffer);

	char *buff = (char *)EVBUFFER_DATA(buffer);
	if (buff){
		char *out = new char[sz+1];
		HttpLogic::getIns()->aes_decrypt(buff, sz, out);
		YMSocketData sd = HttpEvent::getIns()->getSocketDataByStr(out, sz);
		delete out;
		HttpEvent::getIns()->SendMsg(sd, req);
	}
}

char *HttpEvent::getData(vector<char> vec){
	int sz = vec.size();
	char *data = new char[sz+1];
	for (int i = 0; i < sz;i++){
		data[i] = vec[i];
	}
	data[sz] = '\0';
	char *out = new char[sz+1];
	HttpLogic::getIns()->aes_decrypt(data, sz,out);
	delete data;
	return out;
}

void HttpEvent::requestData(string url, YMSocketData sd){
	CURL *curl = curl_easy_init();
	if (curl == NULL)
	{
		printf("cannot curl");
		curl_easy_cleanup(curl);
	}
	//ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
	vector<char> vec;
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, read_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &vec);
	//跟踪到的协议信息、libcurl版本、libcurl的客户代码、操作系统名称、版本、编译器名称、版本等等。
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

	char buff[4096];
	int sz = 0;
	sd.serializer(buff, &sz);
	//加密
	char *data = new char[sz+1];
	HttpLogic::getIns()->aes_encrypt(buff, sz, data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, sz);
	CURLcode code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10000);
	if (code != CURLE_OK) {
		printf("time out\n");
	}
	code = curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10000);
	if (code != CURLE_OK) {
		printf("connect time out\n");
	}

	curl_easy_perform(curl);
	int responseCode = 0;
	delete data;
	code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
	if (code != CURLE_OK || !(responseCode >= 200 && responseCode < 300)) {
		printf("Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(code));
	}
	else{
		char *out = getData(vec);
		YMSocketData sd = getSocketDataByStr(out, vec.size());
		delete out;
		HttpLogic::getIns()->respondleLogic(sd);
	}
	curl_easy_cleanup(curl);
}

void HttpEvent::SendMsg(YMSocketData &sd, struct evhttp_request *req){
	struct evbuffer *buf;
	buf = evbuffer_new();
	char *packBuffer = (char *)malloc(4096);
	int packSize = 0;
	HttpLogic::getIns()->HandleLogic(sd, packBuffer, packSize);
	char *out = new char[packSize+1];
	HttpLogic::getIns()->aes_encrypt(packBuffer,packSize,out);
	evbuffer_add(buf, out, packSize);
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
	delete out;
	free(packBuffer);
}

void HttpEvent::init(){
	//默认参数
	char *httpd_option_listen = "0.0.0.0";
	int httpd_option_port = 8082;
	int httpd_option_timeout = 120; //in seconds

	/* 使用libevent创建HTTP Server */

	//初始化event API
	event_init();

	//创建一个http server
	struct evhttp *httpd;
	httpd = evhttp_start(httpd_option_listen, httpd_option_port);
	evhttp_set_timeout(httpd, httpd_option_timeout);

	//指定generic callback
	evhttp_set_gencb(httpd, httpd_handler, NULL);

	//如果大厅是短连接，则需要短连接带上token

	//也可以为特定的URI指定callback
	//evhttp_set_cb(httpd, "/", specific_handler, NULL);

	//循环处理events
	event_dispatch();

	evhttp_free(httpd);
}

YMSocketData HttpEvent::getSocketDataByStr(char* str, int sz){
	YMSocketData sd;
	if (!str){
		return sd;
	}
	sd.parse(str, sz);

	return sd;
}