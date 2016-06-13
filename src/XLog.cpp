//----------------------------------------------//
//  XUtil.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "XLog.h"
#include "XString.h"
#include "XTime.h"
#include "XFileUtil.h"
#include "xplatform.h"
#include <string.h>
#if X_PLATFORM==X_P_ANDROID
#include <android/log.h>
#endif 
using namespace std;
XLIB_BEGAIN
LOG_LEVEL XLog::mLog_level = LOG_LEVEL::L_ALL;

int XLog::mTimeZone = 8;//默认东八区,default GMT+8 Time Zone

bool XLog::mWrite = false;

std::string XLog::logFile = "";

void XLog::log(LOG_LEVEL level, const char * fmt, ...)
{
    if(level<mLog_level||level==LOG_LEVEL::L_OFF) return;
    string logstr = "";
    string attr = logTime();
    switch (level)
    {
        case LOG_LEVEL::L_INFO:
	    attr += "info:    ";
            break;
        case LOG_LEVEL::L_DEBUG:
            attr += "debug:   ";
            break;
        case LOG_LEVEL::L_WARN:
            attr += "warn:    ";
            break;
        case LOG_LEVEL::L_ERROR:
            attr += "error:   ";
            break;
        case LOG_LEVEL::L_ALL:
            attr += "all:   ";
            break;
        case LOG_LEVEL::L_FATAL:
            attr += "fatal:   ";
            break;
        default:
            break;
    }
    cout<<attr;
    va_list args;
    va_start(args, fmt);
    logstr.append(_log(fmt, args));
    va_end(args);
    
    if (mWrite)
    {
        writeLog(XString::format("%s%s", attr.c_str(), logstr.c_str()));
    }
}


void XLog::androidLog(LOG_LEVEL level, const char * fmt, ...)
{
    if(level<mLog_level||level==LOG_LEVEL::L_OFF) return;
    string logstr = "";
    string attr = logTime();
    
#if X_PLATFORM==X_P_ANDROID
    int priority = 0;
    switch (level)
    {
        case LOG_LEVEL::L_INFO:
            priority = ANDROID_LOG_INFO;
            break;
        case LOG_LEVEL::L_DEBUG:
            priority = ANDROID_LOG_DEBUG;
            break;
        case LOG_LEVEL::L_WARN:
            priority = ANDROID_LOG_WARN;
            break;
        case LOG_LEVEL::L_ERROR:
            priority = ANDROID_LOG_ERROR;
            break;
        case LOG_LEVEL::L_ALL:
            priority = ANDROID_LOG_VERBOSE;
            break;
        case LOG_LEVEL::L_FATAL:
            priority = ANDROID_LOG_FATAL;
            break;
        default:
            break;
    }
    cout<<attr;
    va_list args;
    va_start(args, fmt);
    char* buf = (char*)malloc(X_MAX_LOG_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, X_MAX_LOG_LENGTH, fmt, args);
        logstr.append(buf);
        free(buf);
    }
    va_end(args);
    
    __android_log_print(priority, "Xlib", "%s", logstr.c_str());
    
    if (mWrite)
    {
        writeLog(XString::format("%s%s", attr.c_str(), logstr.c_str()));
    }
#endif
}




void XLog::log(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char* buf = (char*)malloc(X_MAX_LOG_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, X_MAX_LOG_LENGTH, fmt, ap);
        cout<<buf;
        free(buf);
    }
    va_end(ap);
    cout<<endl;
}


string XLog::_log(const char *format, va_list args)
{
    string str = "";
    int bufferSize = X_MAX_LOG_LENGTH;
    char* buf = nullptr;
    
    do
    {
        buf = new (std::nothrow) char[bufferSize];
        if (buf == nullptr)
            return str; // not enough memory
        
        int ret = vsnprintf(buf, bufferSize - 3, format, args);
        if (ret < 0)
        {
            bufferSize *= 2;
            
            delete [] buf;
        }
        else
            break;
        
    } while (true);
   
    cout<<buf<<endl; 
    str = buf;
    delete [] buf;
    return str;
}


void XLog::setLevel(LOG_LEVEL level)
{
    mLog_level = level;
}

string XLog::logTime()
{
	string timelog = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(), mTimeZone), xlib::TIME_F::LOG_TIME);
	return "[" + timelog + "]";
}

void XLog::writeLog(const string& logmsg)
{
	if(logFile.empty()||logFile=="") return;
	XFileUtil::getInstance()->writeTxtLineToFile(logmsg, logFile);
}

XLIB_END
