//----------------------------------------------//
//  XUtil.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XUtil_h
#define XUtil_h

enum class LOG_LEVEL
{
    L_DEBUG = 0,
    L_INFO,
    L_WARN,
    L_ERROR,
    L_FATAL,
    L_ALL,
    L_OFF
};
class XUtil
{
public:
    static void log(const char*,...);
    static void setLevel(LOG_LEVEL);
private:
    static LOG_LEVEL mLog_level;
};



#endif /* XUtil_h */
