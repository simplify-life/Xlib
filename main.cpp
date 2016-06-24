//
//  main.cpp
//  C++11_XLib
//
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//
#include <memory>
#include "src/XDefine.h"
#include "src/XThread.h"
#include "src/XFileUtil.h"
#include "src/XUtf8.h"
#include "src/net/XSocket.h"
//#include "src/net/XEpoll.h"
#include "src/net/http.h"
int main()
{
     US_NS_X;
    
    /**
     0. http test
     */
   
    auto httpRequest = net::http::getRequest();
    std::cout<<httpRequest<<std::endl;

    /**
     1. Below is tcp server test code
     */
    
     //xlib::net::XSocketTCP server;
     //server.startServer(4435);
    
    
    /**
     2. tcp client test
     */
   // struct hostent *hostinfo = nullptr;
   // hostinfo = gethostbyname("www.baidu.com");
    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);

    //tcp->startClient(net::_server(2347,"180.97.33.107"),true);
    tcp->startHttpClient("www.baidu.com");
    tcp->Send(httpRequest.c_str(), httpRequest.size());
    
    /**
     3. Epoll tcp server test
     */
    //xlib::net::epoll::Epoll server;
    //server.startServer(4435);
    
   
    /**
     4. LOG test
     */
    LOG_SET(LOG_LEVEL::L_ALL);
    XLog::setWrite(true, XFileUtil::getCurrentPathWithPrefix().append("xliblog"));
    
    /**
     5. utf8 test
     */
    std::string chutf8 = "这是一个字符串";
    auto s = XUtf8::utf8ToUnicode(chutf8);
    LOG_I(s.c_str());

    /**
     6. thread pool test
     */
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
    
    /**
     7. timer test
     */
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
