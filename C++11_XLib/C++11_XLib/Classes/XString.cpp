//----------------------------------------------//
//  XString.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "XString.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
using namespace std;

string XString::format(const char * fmt, ...)
{
#define X_MAX_STRING_LENGTH (1024*100)
    
    string ret;
    
    va_list ap;
    va_start(ap, fmt);
    
    char* buf = (char*)malloc(X_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, X_MAX_STRING_LENGTH, fmt, ap);
        ret = buf;
        free(buf);
    }
    va_end(ap);
    
    return ret;
}
