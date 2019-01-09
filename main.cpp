#include <memory>
#include "xlib.h"
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
//    net::epoll::Epoll server;
//    server.startServer(4435);
}

void testUtf8(){
    std::string chutf8 = "这是一个字符串";
    auto s = XUtf8::utf8ToUnicode(chutf8);
    LOG_I(s.c_str());
    std::string utf8 = XUtf8::unicodeToUtf8("0x8fd9");
    LOG_I(utf8.c_str());
    byte buffer[] = {0350, 0377, 0231};
    bool isValid = XUtf8::isValidUtf8Buffer(buffer, 3);
    LOG_I(u8"🌸%d",isValid);
    for(byte b = 0 ; b< byte_max ; b++){
        LOG_I(" %d len %d",b,XUtf8::getUtf8ByteLen(b));
    }
    //
//    auto pre0 = XUtf8::findPrefixUtf8("你瞧�瞧");
//    auto pre1 = XUtf8::findPrefixUtf8("你瞧瞧332");
//    LOG_I(XUtf8::findPrefixUtf8("你瞧�瞧").c_str());
//    LOG_I(XUtf8::findPrefixUtf8("你瞧瞧332").c_str());
}

void testThreadPool(){
    auto fun = [](float wTime, XTime::TIMER_LEVEL level)
    {
        std::thread::id tid = std::this_thread::get_id();
        XTime::startTimer(5, wTime,[=]{
            std::string des = "";
            switch (level) {
                case XTime::TIMER_LEVEL::L_SECOND:
                    des = "seconds";
                    break;
                case XTime::TIMER_LEVEL::L_MILLION:
                    des = "milliseconds";
                    break;
                case XTime::TIMER_LEVEL::L_MICRO:
                    des = "microseconds";
                    break;
                default:
                    break;
            }
            LOG_I("this is a %s timer ,thread_id=%s",des.c_str(),XString::convert<std::string>(tid).c_str());
        },XTime::TIMER_LEVEL::L_MILLION);
        return wTime;
    };
    auto pool = std::unique_ptr<XThreadPool>(new XThreadPool(2));
    pool->async([=]{fun(1,XTime::TIMER_LEVEL::L_SECOND);});
    pool->async([=]{fun(0.001,XTime::TIMER_LEVEL::L_MILLION);});
    pool->async([=]{fun(0.001,XTime::TIMER_LEVEL::L_MICRO);});
    pool->async([=](int x, int y){
        LOG_I("%d + %d = %d",x,y,x+y);
        return x+y;
    }, 10,11);
}


void testFile(){
    const std::string originFile = std::string(originPath).append("img_test_result.png");
    const std::string encodeFile = std::string(originPath).append("demo-encode");
    const std::string decodeFile = std::string(originPath).append("demo-decode.png");
    const std::string password = u8"蛤🐸,这是🔐";
    LOG_I(originFile.c_str());
    LOG_I(encodeFile.c_str());
    LOG_I(decodeFile.c_str());
    XFileUtil::encryptoFile(originFile, encodeFile,password);
    XFileUtil::decryptoFile(encodeFile, decodeFile,password);
    LOG_I("%d",XFileUtil::allSameFile(originFile, decodeFile));
    const std::string readme = std::string(originPath).append("README.MD");
    LOG_I("%d",XFileUtil::allSameFile(encodeFile, "readme"));
    LOG_I("%llu",XFileUtil::getFileBytesLength(originFile));
    LOG_I("%llu",XFileUtil::getFileBytesLength(encodeFile));
    LOG_I("%llu",XFileUtil::getFileBytesLength(decodeFile));
    std::string md5OriginFile = XFileUtil::md5(originFile);
    std::string md5DecodeFile = XFileUtil::md5(decodeFile);
    LOG_I("origin md5=%s",md5OriginFile.c_str());
    LOG_I("decodeFile md5=%s",md5DecodeFile.c_str());
}

void testSHA1(){
    crypto::SHA1 sha1;
    sha1.update("360#as");
    LOG_I("sha1(360#as)=%s",sha1.final().c_str());
    const std::string originFile = std::string(originPath).append("img_test_result.png");
    LOG_I("sha1(originFile)=%s",sha1.from_file(originFile).c_str());
}

void testUrl(){
    std::string url1 = "123@$%$*언서ハングルКирилл үсэг你瞧瞧332";
    std::string url1encode = crypto::urlencode(url1);
    LOG_I("%s",url1encode.c_str());
    std::string url1decode = crypto::urldecode(url1encode);
    LOG_I("%s",url1decode.c_str());
    if(0==url1.compare(url1decode)){
        LOG_I("url1 crypto succuess");
    }else{
        LOG_I("url1 crypto failed");
    }
    std::string url2 = "https://www.example.com/s?key0=你好&&key1=测试";
    std::string url2encode = crypto::urlencode(url2,true,false);
    LOG_I("%s",url2encode.c_str());
    std::string url2decode = crypto::urldecode(url2encode);
    LOG_I("%s",url2decode.c_str());
    if(0==url2.compare(url2decode)){
        LOG_I("url2 crypto succuess");
    }else{
        LOG_I("url2 crypto failed");
    }
}

int main()
{
    setLog();
    testThreadPool();
    testSHA1();
    testUtf8();
    testFile();
    testUrl();
    return 0;
}
