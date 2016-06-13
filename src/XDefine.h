//----------------------------------------------//
//  XDefine.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XDefine_h
#define XDefine_h
#include "XLog.h"
#include "XString.h"
#include "XTime.h"
#include "xplatform.h"
namespace xlib {
    

#ifdef _MSC_VER
    #define X_ASSERT(f) //\
//    do \
//    { \
//        if (!(f) && AfxAssertFailedLine(THIS_FILE, __LINE__)) \
//        AfxDebugBreak(); \
//    } while (0) \
//    #define _X_ASSERT(expr) \
//    do { if (!(expr) && \
//        (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, NULL))) \
//        _CrtDbgBreak(); } while (0)
#else
    #define X_ASSERT(x) //((x) || (dbg_printf("assertion failed ("__FILE__":%d): \"%s\"\n",__LINE__,#x), break_point(), FALSE))
#endif
    
    
    
    
    
#define X_DEBUG


//assert macro definitions
#ifdef X_DEBUG
#define ASSERT(x) X_ASSERT(x)
#else
#define ASSERT(x) (void(0))
#endif

//make info
#define MAKE_PAIR(text) std::pair<std::string, decltype(text)>{#text, text}

#define META(...)
    

// string format macro
#define XSTRING(fmt,...) xlib::XString::format(fmt,##__VA_ARGS__)

//log base macro definitions
#if X_PLATFORM==X_P_ANDROID
#define XLIB_LOG(log_level,lfmt,...) xlib::XLog::androidLog(log_level, lfmt.c_str(), ##__VA_ARGS__);
#else
#define XLIB_LOG(log_level,lfmt,...) xlib::XLog::log(log_level, lfmt.c_str(), ##__VA_ARGS__);
#endif
    
#define XLLOG(log_level,fmt,...) \
do \
{ \
if (xlib::LOG_LEVEL::L_INFO < log_level || xlib::LOG_LEVEL::L_ALL == log_level)\
	{ \
		std::string lfmt = XSTRING("In %s ->%s->%d:\t %s", __FILE__, __FUNCTION__, __LINE__, fmt);	\
		XLIB_LOG(log_level,lfmt,##__VA_ARGS__); \
	} \
else \
xlib::XLog::log(log_level,fmt,##__VA_ARGS__); \
}while(0)

#define XLOG(fmt,...) xlib::XLog::log(fmt,##__VA_ARGS__)

/////////////////////////////logs macro definitions////////////////////////
#define LOG_SET(log_level) xlib::XLog::setLevel(log_level)

#define LOG_I(fmt,...) XLLOG(xlib::LOG_LEVEL::L_INFO,fmt,##__VA_ARGS__)
#define LOG_D(fmt,...) XLLOG(xlib::LOG_LEVEL::L_DEBUG,fmt,##__VA_ARGS__)
#define LOG_W(fmt,...) XLLOG(xlib::LOG_LEVEL::L_WARN,fmt,##__VA_ARGS__)
#define LOG_E(fmt,...) XLLOG(xlib::LOG_LEVEL::L_ERROR,fmt,##__VA_ARGS__)
#define LOG_A(fmt,...) XLLOG(xlib::LOG_LEVEL::L_ALL,fmt,##__VA_ARGS__)
#define LOG_F(fmt,...) XLLOG(xlib::LOG_LEVEL::L_FATAL,fmt,##__VA_ARGS__)

    
} //end xlib

#endif /* XDefine_h */
