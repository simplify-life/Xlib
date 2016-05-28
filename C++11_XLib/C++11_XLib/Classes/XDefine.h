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
#include "XUtil.h"
#include "XString.h"
#include "XTime.h"


//define cross-platform base data
using uint=unsigned int;

#ifdef _MSC_VER
using int8=__int8;
using int16=__int16;
using int32=__int32;
using int64=__int64;

using uint8=unsigned __int8;
using uint16=unsigned __int16;
using uint32=unsigned __int32;
using uint64=unsigned __int64;
#else
using int8=int8_t;
using int16=int16_t;
using int32=int32_t;
using int64=int64_t;

using uint8=uint8_t;
using uint16=uint16_t;
using uint32=uint32_t;
using uint64=uint64_t;
#endif

//log base define
#define XLLOG(log_level,fmt,...) \
do \
{ \
if(LOG_LEVEL::L_ERROR==log_level||LOG_LEVEL::L_ALL==log_level||LOG_LEVEL::L_FATAL==log_level) \
std::cout<<"("<<__FILE__ <<":"<<__FUNCTION__<<":"<<__LINE__<<")";\
XUtil::log(log_level,fmt,##__VA_ARGS__); \
}while(0)

#define XLOG(fmt,...) XUtil::log(fmt,##__VA_ARGS__)

/////////////////////////////logs define////////////////////////
#define LOG_SET(log_level) XUtil::setLevel(log_level)

#define LOG_I(fmt,...) XLLOG(LOG_LEVEL::L_INFO,fmt,##__VA_ARGS__)
#define LOG_D(fmt,...) XLLOG(LOG_LEVEL::L_DEBUG,fmt,##__VA_ARGS__)
#define LOG_W(fmt,...) XLLOG(LOG_LEVEL::L_WARN,fmt,##__VA_ARGS__)
#define LOG_E(fmt,...) XLLOG(LOG_LEVEL::L_ERROR,fmt,##__VA_ARGS__)
#define LOG_A(fmt,...) XLLOG(LOG_LEVEL::L_ALL,fmt,##__VA_ARGS__)
#define LOG_F(fmt,...) XLLOG(LOG_LEVEL::L_FATAL,fmt,##__VA_ARGS__)


#define XSTRING(fmt,...) XString::format(fmt,##__VA_ARGS__)

#endif /* XDefine_h */
