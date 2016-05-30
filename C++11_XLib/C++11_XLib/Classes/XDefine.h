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

//log base define
#define XLLOG(log_level,fmt,...) \
do \
{ \
if(LOG_LEVEL::L_ERROR==log_level||LOG_LEVEL::L_ALL==log_level||LOG_LEVEL::L_FATAL==log_level) \
std::cout<<"("<<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__<<")";\
XLog::log(log_level,fmt,##__VA_ARGS__); \
}while(0)

#define XLOG(fmt,...) XLog::log(fmt,##__VA_ARGS__)

/////////////////////////////logs define////////////////////////
#define LOG_SET(log_level) XLog::setLevel(log_level)

#define LOG_I(fmt,...) XLLOG(LOG_LEVEL::L_INFO,fmt,##__VA_ARGS__)
#define LOG_D(fmt,...) XLLOG(LOG_LEVEL::L_DEBUG,fmt,##__VA_ARGS__)
#define LOG_W(fmt,...) XLLOG(LOG_LEVEL::L_WARN,fmt,##__VA_ARGS__)
#define LOG_E(fmt,...) XLLOG(LOG_LEVEL::L_ERROR,fmt,##__VA_ARGS__)
#define LOG_A(fmt,...) XLLOG(LOG_LEVEL::L_ALL,fmt,##__VA_ARGS__)
#define LOG_F(fmt,...) XLLOG(LOG_LEVEL::L_FATAL,fmt,##__VA_ARGS__)


#define XSTRING(fmt,...) XString::format(fmt,##__VA_ARGS__)

#endif /* XDefine_h */
