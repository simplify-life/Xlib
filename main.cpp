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
#include "src/crypt/md5.h"

US_NS_X;
const std::string originPath = XFileUtil::getCurrentPathWithPrefix();

void setLog(){
    LOG_SET(LOG_LEVEL::L_ALL);
    XLog::setWrite(true, std::string(originPath).append("xlib.log"));
}

void testHttp(){
    auto httpRequest = net::http::getRequest();
    LOG_I(httpRequest.c_str());
    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);
    tcp->startHttpClient("www.w3.org");
    tcp->Send(httpRequest.c_str(), httpRequest.size());
}

void testTCPServer(){
    net::XSocketTCP server;
    server.startServer(4435);
}

void testTCPClient(){
     struct hostent *hostinfo = nullptr;
     hostinfo = gethostbyname("www.baidu.com");
    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);
    tcp->startClient(net::_server(4435,"127.0.0.1"));
}

void testEpollServer(){
    net::epoll::Epoll server;
    server.startServer(4435);
}

void testUtf8(){
    std::string chutf8 = "这是一个字符串";
    auto s = XUtf8::utf8ToUnicode(chutf8);
    LOG_I(s.c_str());
    std::string utf8 = XUtf8::unicodeToUtf8("0x8fd9");
    LOG_I(utf8.c_str());
    byte buffer[] = {0350, 0377, 0231};
    bool isValid = XUtf8::isValidUtf8Buffer(buffer, 3);
    LOG_I("🌸%d",isValid);
    for(byte b = 0 ; b< byte_max ; b++){
        LOG_I(" %d len %d",b,XUtf8::getUtf8ByteLen(b));
    }
}

void testThreadPool(){
    auto fun = [](int wTime)
    {
        std::thread::id tid = std::this_thread::get_id();
        std::this_thread::sleep_for(std::chrono::seconds(wTime));
        auto t = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8),TIME_F::T_DEFAULT);
        LOG_I("thread_id=%s,%s,%s",XString::convert<std::string>(tid).c_str(),t.c_str(),XRand::getRandomString(10).c_str());
    };
    auto pool = std::unique_ptr<XThreadPool>(new XThreadPool(6,8,true));
    pool->addTask([=]{fun(0);});
    pool->addTask([=]{fun(1);});
    pool->addTask([=]{fun(2);});
}

void testTimer(){
    XTime::doPertime(-1, 1,[]
                     {
                         LOG_D("this is a timer,1 second 1 times");
                     } );
    auto loop = []
    {
        while (true)
        {
            
        }
    };
    std::unique_ptr<XThreadPool>(new XThreadPool(6,8,true))->addTask(loop);
}

void testFile(){
    const std::string originFile = std::string(originPath).append("img_test_result.png");
    const std::string encodeFile = std::string(originPath).append("demo-encode");
    const std::string decodeFile = std::string(originPath).append("demo-decode.png");
    const std::string password = "蛤🐸,这是🔐";
    LOG_I(originFile.c_str());
    LOG_I(encodeFile.c_str());
    LOG_I(decodeFile.c_str());
    XFileUtil::encryptFile(originFile, encodeFile,password);
    XFileUtil::decryptFile(encodeFile, decodeFile,password);
    //    XFileUtil::copyFile(originFile, decodeFile);
    LOG_I("%d",XFileUtil::allSameFile(originFile, decodeFile));
    const std::string readme = std::string(originPath).append("README.MD");
    LOG_I("%d",XFileUtil::allSameFile(encodeFile, "readme"));
    LOG_I("%llu",XFileUtil::getFileBytesLength(originFile));
    LOG_I("%llu",XFileUtil::getFileBytesLength(encodeFile));
    LOG_I("%llu",XFileUtil::getFileBytesLength(decodeFile));
}

int main()
{
    setLog();

    testUtf8();

    testFile();
//    testHttp();
    return 0;
}
