#include <memory>
#include <cstdlib>
#include <sstream>
#include "xlib.h"
US_NS_X;
const std::string originPath = XFileUtil::getCurrentPathWithPrefix();

void setLog(){
    std::string xlib_log_dir = std::string();
    const char* dir = getenv("xlib_log_dir");
    if(nullptr != dir){
        xlib_log_dir = std::string(dir);
    }
    if(xlib_log_dir.empty()){
        xlib_log_dir = originPath;
        setenv("xlib_log_dir", xlib_log_dir.c_str(), 1);
    }
    LOG_SET(LOG_LEVEL::L_ALL);
    XLog::setWrite(true, xlib_log_dir.append("xlib.log"));
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
    auto fun = [](uint32 count,float wTime)
    {
        std::thread::id tid = std::this_thread::get_id();
        XTime::startTimer(count, wTime,[=]{
            std::string des = "";
            LOG_I("this is a %f seconds timer ,thread_id=%s",wTime,XString::convert<std::string>(tid).c_str());
        });
        return wTime;
    };
    auto pool = std::unique_ptr<XThreadPool>(new XThreadPool(4));
    pool->async([=]{fun(3,1);});
    pool->async([=]{fun(2,0.001);});
//    pool->async([=]{fun(10,0.00001,XTime::TIMER_LEVEL::L_MICRO);});
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

void testSHA(){
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


// 测量排序时间的函数模板
template<typename T>
double measureSortTime(std::vector<T>& arr, void (*sort_func)(std::vector<T>&)) {
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << "-> ";
    auto start_time = std::chrono::high_resolution_clock::now();
    sort_func(arr);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return elapsed_time.count();
}


// 测量排序时间的函数模板
template<typename T>
double measureSortTime2(std::vector<T>& arr, void (*sort_func)(std::vector<T>&,int arg0, int arg1)) {
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << "-> ";
    auto start_time = std::chrono::high_resolution_clock::now();
    sort_func(arr,0,arr.size()-1);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return elapsed_time.count();
}

void testSort(){
    std::vector<int> my_vector = { 114115, 13, 1, 4, 126, 1, 5, 9, 45, 2, 6, 5, 3 };

    // 测量冒泡排序时间
    std::vector<int> bubble_sort_data = my_vector;
    double bubble_sort_time = measureSortTime(bubble_sort_data, sort::bubbleSort<int>);
    LOG_I("冒泡排序时间：%f 秒", bubble_sort_time);

    // 测量选择排序时间
    std::vector<int> select_sort_data = my_vector;
    double selection_sort_time = measureSortTime(select_sort_data, sort::selectionSort<int>);
    LOG_I("选择排序时间：%f 秒", selection_sort_time);

    // 测量插入排序时间
    std::vector<int> insert_sort_data = my_vector;
    double insertion_sort_time = measureSortTime(insert_sort_data, sort::insertionSort<int>);
    LOG_I("插入排序时间：%f 秒", insertion_sort_time);
    
    // 测量希尔排序时间
    std::vector<int> shell_sort_data = my_vector;
    double shell_sort_time = measureSortTime(shell_sort_data, sort::shellSort<int>);
    LOG_I("希尔排序时间：%f 秒", shell_sort_time);
    
    // 测量快速排序时间
    std::vector<int> quick_sort_data = my_vector;
    double quick_sort_time = measureSortTime2(quick_sort_data, sort::quickSort<int>);
    LOG_I("快速排序时间：%f 秒", quick_sort_time);
    
    // 测量归并排序时间
    std::vector<int> merge_sort_data = my_vector;
    double merge_sort_time = measureSortTime2(merge_sort_data, sort::mergeSort<int>);
    LOG_I("归并排序时间：%f 秒", merge_sort_time);
    
    // 测量堆排序时间
    std::vector<int> heap_sort_data = my_vector;
    double heap_sort_time = measureSortTime(heap_sort_data, sort::heapSort<int>);
    LOG_I("堆排序时间：%f 秒", heap_sort_time);
    
    // 测量计数排序时间
    std::vector<int> counting_sort_data = my_vector;
    double counting_sort_time = measureSortTime(counting_sort_data, sort::countingSort<int>);
    LOG_I("计数排序时间：%f 秒", counting_sort_time);
    
    // 测量桶排序时间
    std::vector<int> bucket_sort_data = my_vector;
    double bucket_sort_time = measureSortTime(bucket_sort_data, sort::bucketSort<int>);
    LOG_I("桶排序时间：%f 秒", bucket_sort_time);
    
    // 测量基数排序时间
    std::vector<int> radix_sort_data = my_vector;
    double radix_sort_time = measureSortTime(radix_sort_data, sort::radixSort<int>);
    LOG_I("基数排序时间：%f 秒", radix_sort_time);
}

void testJson(){
        std::string jsonStr = "{ \"name\": \"Alice\", \"age\": 25,\n\t \"arr\":[1,\n" \
    "2,3], own:null, b1:false, b2:true }";
        xlib::JSON json = xlib::JSON::parse(jsonStr);
        std::string name = json["name"].asString();
        int age = json["age"].asInt();
        std::vector<JSON> arr = json["arr"].asArray();
        LOG_I("Name: %s", name.c_str());
        LOG_I("Age: %d", age);
        LOG_I("Arr[0]: %d", arr.at(0).asInt());
        LOG_I("Arr[1]: %d", arr.at(1).asInt());
        LOG_I("Arr[2]: %d", arr.at(2).asInt());
        LOG_I("B1: %d", json["b1"].asBoolean());
        LOG_I("B2: %d", json["b2"].asBoolean());
}

void testSerializer(){
    // FIXME free(): invalid pointer
    struct Person {
        std::string name;
        int age;
        bool isMarried;
    };
    Person p1{"Alice", 25, false};
    
    std::string binFile = std::string(originPath).append("person.bin");
    
    // Serialize to file
    xlib::Serializer::serialize(p1, binFile);

    // Deserialize from file
    Person p2;
    xlib::Serializer::deserialize(p2, binFile);

    // Serialize to buffer
    std::vector<char> buffer = xlib::Serializer::serialize(p1);

    // Deserialize from buffer
    Person p3 = xlib::Serializer::deserialize<Person>(buffer);

    // Print results
    LOG_I("p1: %s, %d, %d", p1.name.c_str(), p1.age, p1.isMarried);
    LOG_I("p2: %s, %d, %d", p2.name.c_str(), p2.age, p2.isMarried);
    LOG_I("p3: %s, %d, %d", p3.name.c_str(), p3.age, p3.isMarried);
    buffer.clear();
}

void testYml(){
    std::string yml = "key1:\n"
                             "  key2:\n"
                             "    key3:\n"
                             "      key4:\n"
                             "        key5: value\n"
                             "  key6:\n"
                             "    key7: haha\n"
                             "    key8: 8\n"
                             " # below is arr value"
                             "k:\n"
                             "  array:\n"
                             "    - item1\n"
                             "    - item2\n"
                             "    - item3\n";
    YamlParser parser = YamlParser::fromString(yml);
    parser.print();
}

void testSgf(){
    std::string sgfStr ="(;SZ[19]AP[MultiGo:3.6.0]AB[pb][pc][oc][od][ne][nf][og][pg][qg][rg][rf]AW[qf][pf][of][oe][re][qd][qc][pd]\n"
    "(;B[sd](;W[rb]    ;B[qe](;W[pe]    ;B[rd]    ;W[se]    ;B[sf]    ;W[qe]    ;B[qb]    ;W[rc]    ;B[ra])\n"
    "(;W[rd]    ;B[sc]    ;W[se]    ;B[pe]    ;W[qb]    ;B[qa]    ;W[ra]    ;B[sb])\n"
    ")\n"
    "(;W[se]    ;B[rb]    ;W[rc]    ;B[sc]    ;W[qb]    ;B[qa])\n"
    ")\n"
    "(;B[rb]    ;W[rc]    ;B[sd]    ;W[sc]    ;B[se]    ;W[qb]    ;B[qa]    ;W[ra]    ;B[sa]    ;W[sb])\n"
    ")";
    auto parser = std::unique_ptr<sgf::Parser>(new sgf::Parser());
    parser->parseSgf(sgfStr);
    auto v = parser->getmoveList();
    for(auto &s:v){
        LOG_I("%s",s.c_str());
    }
}

void testRegex(){
    std::vector<XRegex::MatchInfo> result = XRegex::getMatch("helloo123world456", "\\d+", false);
    for (const auto& info : result) {
        LOG_I("%d,%s",info.pos,info.str.c_str());
    }
    std::string r = XRegex::replace("hello123world456", "\\d+", "数",-1);
    LOG_I("%s",r.c_str());
}

void testMatrix(){
    std::vector<int> arr= {0,1,2,3,4};
    auto perm = permutation(arr, 2);
    std::stringstream ss;
    for(auto& e:perm){
        for(auto& e1:e){
            ss<<e1;
            ss<<" ";
        }
        ss<<"\n";
    }
    LOG_I("permutation:\n%s",ss.str().c_str());
    
    auto com = combination(arr, 2);
    std::stringstream ssc;
    for(auto& e:com){
        for(auto& e1:e){
            ssc<<e1;
            ssc<<" ";
        }
        ssc<<"\n";
    }
    LOG_I("combination:\n%s",ssc.str().c_str());
    
    Matrix  A(5,5);
    Matrix  B(5,5);
    A(2,2) = 3;
    B(2,2) = 2;
    LOG_I("A:\n%s",A.toString().c_str());
    LOG_I("B:\n%s",B.toString().c_str());
    auto C = A+B;
    LOG_I("A+B:\n%s",C.toString().c_str());
    std::vector<std::vector<int>> matrixD = {{5,2,4},{3,8,2},{6,0,4},{0,1,6}};
    Matrix D(matrixD);
    std::vector<std::vector<int>> matrixE = {{2,4},{1,3},{3,2}};
    Matrix E = matrixE;
    LOG_I("D:\n%s",D.toString().c_str());
    LOG_I("E:\n%s",E.toString().c_str());
    Matrix F = D*E;
    LOG_I("D*E:\n%s",F.toString().c_str());
    
    std::vector<std::vector<int>> matrixG = {
        {1,2,3,4},
        {5,-1,7,8},
        {8,10,11,12},
        {17,14,15,16}
    };
    
    Matrix G = matrixG;
    
    LOG_I("G.det():\n%d",G.det());
    
    std::vector<std::vector<int>> matrixH = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    
    Matrix H = matrixH;
    
    LOG_I("H:\n%s",H.toString().c_str());
    
    auto subArr = H.submatrix(2);
    
    for(auto& m:subArr){
        LOG_I("H.submatrix 2:\n%s",m.toString().c_str());
    }
    
    LOG_I("H rank: %d",H.rank());
}

int main(int argc, char* argv[])
{
    setLog();
    testMatrix();
    testRegex();
    testYml();
    testSgf();
    // testSerializer();
    testJson();
    testThreadPool();
    testSHA();
    testUtf8();
    testFile();
    testUrl();
    testSort();
    return 0;
}
