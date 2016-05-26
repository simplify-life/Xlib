//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include <iomanip>
#include "Classes/XDefine.h"
#include "Classes/XThread.h"
int main(int argc, const char * argv[]) {
    
    auto fun = []
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%50));
        std::thread::id tid = std::this_thread::get_id();
        auto t = XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8);
        LOG_I("thread_id=%d,%02d:%02d:%02d",tid,t->tm_hour,t->tm_min,t->tm_sec);
    };
    auto pool = new XThreadPool(5,5,true);
    
    pool->addTask(fun);
    pool->addTask(fun);
    pool->addTask(fun);
    pool->addTask(fun);
    
    XTime::getInstance()->doPertime(-1, 1,[]
    {
        auto t = XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8);
        LOG_D("%02d:%02d:%02d",t->tm_hour,t->tm_min,t->tm_sec);
    } );
    auto loop = []
    {
        while (true)
        {
            XTimer::getInstance()->mainLoop();
        }
    };
    pool->addTask(loop);
    pool->join();
    return 0;
}
