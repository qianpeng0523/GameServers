﻿#include "HttpEvent.h"
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
	string *content = (string *)stream;
	content->append((char*)buffer, size * nmemb);
	return size*nmemb;
}

void HttpEvent::curlPost(string url, Object *target, Http_CallFun selector){
	CURL *curl = curl_easy_init();
	if (curl == NULL)
	{
		
		printf( "cannot curl");
		curl_easy_cleanup(curl);
	}
	struct curl_httppost *formPost = NULL;
	
	string content;

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formPost);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, read_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	//跟踪到的协议信息、libcurl版本、libcurl的客户代码、操作系统名称、版本、编译器名称、版本等等。
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);


	curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_formfree(formPost);

	YMSocketData sd = HttpEvent::getIns()->getSocketDataByStr(content,content.length());
	printf("content:%s\n", sd.getJsonString().c_str());
	
	(target->*selector)(sd);
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
	std::vector<string> vecs = Common::getStrings(uri, "/");
	string str = uri;
	str = str.substr(1, str.length());
	HttpEvent::getIns()->EventDispath(req, str);
}

void HttpEvent::EventDispath(struct evhttp_request *&req, string uri){
	YMSocketData sd = getSocketDataByStr(uri, uri.length());
	//通过url分写逻辑
	SendMsg(sd, req);
	
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
	evbuffer_free(buf);
	free(packBuffer);
}

void HttpEvent::init(){
	//默认参数
	char *httpd_option_listen = "0.0.0.0";
	int httpd_option_port = 8080;
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

YMSocketData HttpEvent::getSocketDataByStr(string str, int sz){
	YMSocketData sd;
	if (str.empty()){
		return sd;
	}
	sd.parse((char *)str.c_str(), sz);

	return sd;
}