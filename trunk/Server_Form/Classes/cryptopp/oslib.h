//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _OSLIB_H_
#define _OSLIB_H_

/// common includes
//#include <string.h>
#include "unreal_string.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

/// os includes
#ifdef _WIN32

typedef unsigned long DWORD;

    //DWORD
#elif defined(__GNUC__)

#    include <strings.h>
#    include <pthread.h>

#else

#    error not support this OS

#endif /* end of os include */

/// all os exist functions
#if defined(_WIN32)

#    define __threadlocal __declspec(thread)
#    define thread_native_type DWORD
#    define this_thread GetCurrentThreadId

#elif defined(__linux)

#    define __threadlocal __thread
#    define thread_native_type pthread_t
#    define this_thread pthread_self
#    define stricmp strcasecmp

#endif /* end of os exist functions */

/// extends functions
#if defined(_WIN32)

#if _MSC_VER <= 1700
extern long long atoll(const char* p);
#endif

struct timezone2
{  
  int tz_minuteswest;  
  int tz_dsttime;  
};  
 
struct timeval;
struct timezone2;
namespace oslib {
    int
        gettimeofday(struct timeval *tv, struct timezone2 *tz = nullptr);
};

#else /* unix like systems */

namespace oslib {
    using ::gettimeofday;
}

#endif /* end of os functions */

extern long long millitime(void);
extern long long microtime(void);

extern bool xstrtob(const purelib::unmanaged_string& str);
extern int xstrtoi(const purelib::unmanaged_string& str);
extern long long xstrtoll(const purelib::unmanaged_string& str);
extern double xstrtod(const purelib::unmanaged_string& str);

extern int strtoi_s(const char* p, size_t n);
extern long long strtoll_s(const char* p, size_t n);
extern double strtolf_s(const char* p, size_t n);
extern char* strchr_s(const char * string, size_t n,int ch);
extern char*  strstr_s(const char * str1, size_t n1, const char * str2, size_t n2 = -1);
//#ifdef __cplusplus
//}
//#endif
template<size_t _Size> inline
void xxstrcpy(char* _Dest, const char(&_Src)[_Size])
{
    ::memcpy(_Dest, _Src, _Size);
}

#endif

