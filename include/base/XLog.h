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

#include "macro/XBase.h"
#include <iostream>
#include <stdarg.h>

XLIB_BEGAIN
// The max length of log message.
static const int X_MAX_LOG_LENGTH = 16*1024;
enum class LOG_LEVEL
{
    L_ALL = 0,
    L_INFO,
    L_DEBUG,
    L_WARN,
    L_ERROR,
    L_FATAL,
   
    L_OFF
};
class XLog
{
public:
    static void log(LOG_LEVEL,const char*,...);
    static void androidLog(LOG_LEVEL,const char*,...);
    static void log(const char*,...);
    static void setLevel(LOG_LEVEL);
    inline static void setTimeZone(int timeZone){ mTimeZone = timeZone;};
    inline static void setWrite(bool isWrite,const std::string& fullLogfile){ mWrite = isWrite; logFile = fullLogfile;};
private:
    static std::string _log(const char*,va_list);
    static std::string logTime();
    static void writeLog(const std::string& logmsg);
private:
    static LOG_LEVEL mLog_level;
    static int mTimeZone;
    static bool mWrite;
    static std::string logFile;
};

XLIB_END

#endif /* XLog_h */
