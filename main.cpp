//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include "Classes/XDefine.h"
#include "Classes/XThread.h"
#include "Classes/XFileUtil.h"
int main()
{
    
    US_NS_X;
    LOG_SET(LOG_LEVEL::L_ALL);
    
    //NOTE: you'd better set the fileFullName in a full path for easy to find it
    const std::string fileFullName = "xlib_testtxt";
    XFileUtil::getInstance()->writeTxtLineToFile("this is a test--------1", fileFullName);
    XFileUtil::getInstance()->writeTxtLineToFile("this is a test-------2", fileFullName);
    auto exist = XFileUtil::getInstance()->readStringByLine(fileFullName);
    for(auto e:exist)
    {
        LOG_I("%s",e.c_str());
    }
    auto fun = []
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%50));
        std::thread::id tid = std::this_thread::get_id();
        auto t = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8),TIME_F::T_DEFAULT);
        LOG_I("thread_id=%d,%s",tid,t.c_str());
    };
    auto pool = new XThreadPool(5,5,true);
    
    pool->addTask(fun);
    pool->addTask(fun);
    pool->addTask(fun);
    pool->addTask(fun);
    
    XTime::getInstance()->doPertime(-1, 0.5,[]
    {
        LOG_D("this is a timer,1 second 2 times");
    } );
    auto loop = []
    {
        while (true)
        {
            XTimer::getInstance()->mainLoop();
        }
    };
    pool->addTask(loop);
    pool->detach();
    std::this_thread::sleep_for(std::chrono::seconds(60));
    return 0;
}
