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
#include "src/XRandom.h"
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
//    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);

    //tcp->startClient(net::_server(2347,"180.97.33.107"));
//    tcp->startHttpClient("www.w3.org");
//    tcp->Send(httpRequest.c_str(), httpRequest.size());
    
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
    auto fun = [](int wTime)
    {
        std::thread::id tid = std::this_thread::get_id();
        std::this_thread::sleep_for(std::chrono::seconds(wTime));
        auto t = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8),TIME_F::T_DEFAULT);
        LOG_I("thread_id=%s,%s,%s",XString::convert<std::string>(tid).c_str(),t.c_str(),XRand::getRandomString(10).c_str());
    };
    fun(1);
    fun(2);
//    auto pool = std::unique_ptr<XThreadPool>(new XThreadPool(6,8,true));
//    
//    pool->addTask(fun);
//    pool->addTask(fun);
//    pool->addTask(fun);
    
    /**
     7. timer test
     */
//    XTime::doPertime(-1, 1,[]
//    {
//        LOG_D("this is a timer,1 second 1 times");
//    } );
//    auto loop = []
//    {
//        while (true)
//        {
//            XTimer::getInstance()->mainLoop();
//        }
//    };
//    pool->addTask(loop);
    
    /**
     8. test udp
     */
    
//    auto udpServer = []
//    {
//        net::XSocketUDP server;
//        server.startServer(8888);
//    };
//    
//    pool->addTask(udpServer);
//    pool->addTask([]
//    {
//        net::XSocketUDP client;
//        client.startClient("127.0.0.1", 8888);
//    });
    
//    pool->detach();
//    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    std::string utf8 = XUtf8::unicodeToUtf8("0x8fd9");
    LOG_I(utf8.c_str());
    byte buffer[] = {0350, 0377, 0231};
    bool isValid = XUtf8::isValidUtf8Buffer(buffer, 3);
    LOG_I("🌸%d",isValid);
    for(byte b = 0 ; b< byte_max ; b++){
        LOG_I(" %d len %d",b,XUtf8::getUtf8ByteLen(b));
    }
    return 0;
}
