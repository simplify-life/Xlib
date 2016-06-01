//----------------------------------------------//
//  XLog.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XLog_h
#define XLog_h

#include "XBase.h"
#include <iostream>
#include <stdarg.h>

XLIB_BEGAIN
// The max length of log message.
static const int X_MAX_LOG_LENGTH = 16*1024;
enum class LOG_LEVEL
{
    L_ALL = 0,
    L_DEBUG,
    L_INFO,
    L_WARN,
    L_ERROR,
    L_FATAL,
   
    L_OFF
};
class XLog
{
public:
    static void log(LOG_LEVEL,const char*,...);
    static void log(const char*,...);
    static void setLevel(LOG_LEVEL);
private:
    static void _log(const char*,va_list);
private:
    static LOG_LEVEL mLog_level;
};

XLIB_END

#endif /* XLog_h */
