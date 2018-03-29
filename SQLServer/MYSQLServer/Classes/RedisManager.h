#ifndef __LYNX_GAME_SERVER_REDISMANAGER_H__
#define __LYNX_GAME_SERVER_REDISMANAGER_H__


#include "hiredis.h"
extern "C"
{
#include <win32fixes.h>
}
#include <string>
using namespace std;


	class RedisManager 
	{
	   public:

	    RedisManager();
		virtual ~RedisManager();
		//初始化函数
		bool initial();
		//释放函数
		void release();
		//封装的set接口
        void set(std::string key, std::string value);
		//封装的get接口
        std::string get(std::string key);
		
        private:
		
		//连接的缓存指针
        redisContext* m_pConnect;
		//请求处理结果指针
        redisReply* m_pReply;
	};


#endif // __LYNX_GAME_SERVER_REDISMANAGER_H__

