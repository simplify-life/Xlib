//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include "src/XDefine.h"
#include "src/XThread.h"
#include "src/XFileUtil.h"
#include "src/XUtf8.h"
#include "src/net/XSocket.h"
int main()
{
    
    /** Below is the server test code*/
     //xlib::net::XSocketTCP server;
     //server.startServer(4435);
    
    US_NS_X;
    LOG_SET(LOG_LEVEL::L_ALL);
    XLog::setWrite(true, XFileUtil::getCurrentPathWithPrefix().append("xliblog"));
    
    auto tcp = std::shared_ptr<net::XSocketTCP>(new net::XSocketTCP);
    tcp->startClient(net::_server(2347,"120.27.94.221"),true);
    std::string chutf8 = "这是一个字符串";
    auto s = XUtf8::utf8ToUnicode(chutf8);
    LOG_I(s.c_str());
    tcp->Send(chutf8.c_str(), sizeof(chutf8));
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
    
    XTime::doPertime(-1, 0.5,[]
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
