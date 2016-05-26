//----------------------------------------------//
//  XUtil.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#include "XUtil.h"
#include <iostream>

using namespace std;
LOG_LEVEL XUtil::mLog_level = LOG_LEVEL::L_INFO;
void XUtil::log(const char * fmt, ...)
{
    
    switch (mLog_level)
    {
        case LOG_LEVEL::L_INFO:
            cout<<"log info:    ";
            break;
        case LOG_LEVEL::L_DEBUG:
            cout<<"log debug:   ";
            break;
        case LOG_LEVEL::L_WARN:
            cout<<"log warn:    ";
            break;
        case LOG_LEVEL::L_ERROR:
            cout<<"log error:   ";
            break;
        case LOG_LEVEL::L_ALL:
            cout<<"log all:   ";
            break;
        case LOG_LEVEL::L_FATAL:
            cout<<"log fatal:   ";
            break;
        case LOG_LEVEL::L_OFF:
            return;
        default:
            break;
    }
#define X_MAX_LOG_LENGTH 1024*16
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

void XUtil::setLevel(LOG_LEVEL level)
{
    mLog_level = level;
}