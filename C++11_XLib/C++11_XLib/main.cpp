//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include <iomanip>
#include "Classes/XDefine.h"
int main(int argc, const char * argv[]) {
    
    XTime::getInstance()->doPertime(10, 1,[]
    {
        auto t = XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8);
        LOG_D("%02d:%02d:%02d",t->tm_hour,t->tm_min,t->tm_sec);
    } );
    while (true)
    {
        XTimer::getInstance()->mainLoop();
    }
    return 0;
}
