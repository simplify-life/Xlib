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
#include "Classes/XUtf8.h"
int main()
{
    
    US_NS_X;
    LOG_SET(LOG_LEVEL::L_ALL);
    XLog::setWrite(true, XFileUtil::getInstance()->getCurrentPathWithPrefix().append("xliblog"));
    auto s = XUtf8::getInstance()->utf8ToUnicode("这是一个字符串");
    LOG_I(s.c_str());
    auto fun = []
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand()%50));
        std::thread::id tid = std::this_thread::get_id();
        auto t = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8),TIME_F::T_DEFAULT);
        LOG_I("thread_id=%d,%s",tid,t.c_str());
    };
    auto pool = std::shared_ptr<XThreadPool>(new XThreadPool(5,5,true));
    
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
