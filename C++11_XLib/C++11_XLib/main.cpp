//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include <iomanip>
#include "Classes/XTime.h"
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    XTime::getInstance()->doPertime(10, 1,[]
    {
        auto t = XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8);
        std::cout<<std::setw(2)<<std::setfill('0')<<t->tm_hour<<":"<<std::setw(2)<<std::setfill('0')<<t->tm_min<<":"<<std::setw(2)<<std::setfill('0')<<t->tm_sec<<std::endl;
    } );
    while (true)
    {
        XTimer::getInstance()->mainLoop();
    }
    return 0;
}
