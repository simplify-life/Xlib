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
#include <string.h>

using namespace std;
XLIB_BEGAIN
LOG_LEVEL XLog::mLog_level = LOG_LEVEL::L_ALL;

int XLog::mTimeZone = 8;//默认东八区,default GMT+8 Time Zone

bool XLog::mWrite = false;

std::string XLog::logFile = "";

void XLog::log(LOG_LEVEL level, const char * fmt, ...)
{
    if(level<mLog_level||level==LOG_LEVEL::L_OFF) return;
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
	cout << attr;
	if (mWrite)
	{
	   writeLog(XString::format("%s%s", attr.c_str(), fmt));
	}
    va_list args;
    va_start(args, fmt);
    _log(fmt, args);
    va_end(args);
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


void XLog::_log(const char *format, va_list args)
{
    int bufferSize = X_MAX_LOG_LENGTH;
    char* buf = nullptr;
    
    do
    {
        buf = new (std::nothrow) char[bufferSize];
        if (buf == nullptr)
            return; // not enough memory
        
        int ret = vsnprintf(buf, bufferSize - 3, format, args);
        if (ret < 0)
        {
            bufferSize *= 2;
            
            delete [] buf;
        }
        else
            break;
        
    } while (true);
    
    strcat(buf, "\n");
    fprintf(stdout, "%s", buf);
    fflush(stdout);
    delete [] buf;
}


void XLog::setLevel(LOG_LEVEL level)
{
    mLog_level = level;
}

string XLog::logTime()
{
	string timelog = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(), mTimeZone), xlib::TIME_F::LOG_TIME);
 	cout<<"["<<timelog<<"] ";
	return "[" + timelog + "]";
}

void XLog::writeLog(const string& logmsg)
{
	if(logFile.empty()||logFile=="") return;
	XFileUtil::getInstance()->writeTxtLineToFile(logmsg, logFile);
}

XLIB_END
