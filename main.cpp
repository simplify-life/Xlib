#include <memory>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include "xlib.h"
US_NS_X;
const std::string originPath = XFileUtil::getCurrentPathWithPrefix();

void setLog()
{
    std::string xlib_log_dir = std::string();
    const char *dir = getenv("xlib_log_dir");
    if (nullptr != dir)
    {
        xlib_log_dir = std::string(dir);
    }
    if (xlib_log_dir.empty())
    {
        xlib_log_dir = originPath;
        setenv("xlib_log_dir", xlib_log_dir.c_str(), 1);
    }
    LOG_SET(LOG_LEVEL::L_ALL);
    LOG_I("log dir:%s", xlib_log_dir.c_str());
    XLog::setWrite(true, xlib_log_dir.append("xlib.log"));
    
    int numThreads = std::thread::hardware_concurrency();
    
    LOG_I("numThreads = %d",numThreads);
    
}

void testHttp()
{
    auto httpRequest = net::http::getRequest();
    LOG_I(httpRequest.c_str());
    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);
    tcp->startHttpClient("www.w3.org");
    tcp->Send(httpRequest.c_str(), httpRequest.size());
}

void testTCPServer()
{
    net::XSocketTCP server;
    server.startServer(4435);
}

void testTCPClient()
{
    struct hostent *hostinfo = nullptr;
    hostinfo = gethostbyname("www.baidu.com");
    auto tcp = std::unique_ptr<net::XSocketTCP>(new net::XSocketTCP);
    tcp->startClient(net::_server(4435, "127.0.0.1"));
}

void testEpollServer()
{
    //    net::epoll::Epoll server;
    //    server.startServer(4435);
}

void testUtf8()
{
    std::string chutf8 = "这是一个字符串";
    auto s = XUtf8::utf8ToUnicode(chutf8);
    LOG_I(s.c_str());
    std::string utf8 = XUtf8::unicodeToUtf8(s);
    LOG_I(utf8.c_str());
    xbyte buffer[] = {0350, 0377, 0231};
    bool isValid = XUtf8::isValidUtf8Buffer(buffer, 3);
    LOG_I(u8"🌸%d", isValid);
    for (xbyte b = 0; b < byte_max; b++)
    {
        LOG_I(" %d len %d", b, XUtf8::getUtf8ByteLen(b));
    }
    //
    //    auto pre0 = XUtf8::findPrefixUtf8("你瞧�瞧");
    //    auto pre1 = XUtf8::findPrefixUtf8("你瞧瞧332");
    //    LOG_I(XUtf8::findPrefixUtf8("你瞧�瞧").c_str());
    //    LOG_I(XUtf8::findPrefixUtf8("你瞧瞧332").c_str());
}

void testThreadPool()
{
    auto fun = [](uint32 count, float wTime)
    {
        std::thread::id tid = std::this_thread::get_id();
        XTime::startTimer(count, wTime, [=]
                          {
            std::string des = "";
            LOG_I("this is a %f seconds timer ,thread_id=%s",wTime,XString::convert<std::string>(tid).c_str()); });
        return wTime;
    };
    auto pool = std::unique_ptr<XThreadPool>(new XThreadPool(4));
    pool->async([=]
                { fun(3, 1); });
    pool->async([=]
                { fun(2, 0.001); });
    //    pool->async([=]{fun(10,0.00001,XTime::TIMER_LEVEL::L_MICRO);});
    pool->async([=](int x, int y)
                {
        LOG_I("%d + %d = %d",x,y,x+y);
        return x+y; },
                10, 11);
}

void testFile()
{
    const std::string originFile = std::string(originPath).append("img_test_result.png");
    const std::string encodeFile = std::string(originPath).append("demo-encode");
    const std::string decodeFile = std::string(originPath).append("demo-decode.png");
    const std::string password = u8"蛤🐸,这是🔐";
    LOG_I(originFile.c_str());
    LOG_I(encodeFile.c_str());
    LOG_I(decodeFile.c_str());
    XFileUtil::encryptoFile(originFile, encodeFile, password);
    XFileUtil::decryptoFile(encodeFile, decodeFile, password);
    LOG_I("%d", XFileUtil::allSameFile(originFile, decodeFile));
    const std::string readme = std::string(originPath).append("README.MD");
    LOG_I("%d", XFileUtil::allSameFile(encodeFile, "readme"));
    LOG_I("%llu", XFileUtil::getFileBytesLength(originFile));
    LOG_I("%llu", XFileUtil::getFileBytesLength(encodeFile));
    LOG_I("%llu", XFileUtil::getFileBytesLength(decodeFile));
    std::string md5OriginFile = XFileUtil::md5(originFile);
    std::string md5DecodeFile = XFileUtil::md5(decodeFile);
    LOG_I("origin md5=%s", md5OriginFile.c_str());
    LOG_I("decodeFile md5=%s", md5DecodeFile.c_str());
}

void testPatch(){
    const std::string newFile = std::string(originPath).append("newFile");
    const std::string oldFile = std::string(originPath).append("oldFile");
    const std::string patchFile = std::string(originPath).append("patch.bin");
    const std::string outPutFile = std::string(originPath).append("patch_out_put");
    auto diff = patch::binary_difference(oldFile, newFile);
    patch::create_patch(patchFile, diff);
    patch::apply_patch(oldFile, patchFile, outPutFile);
}

void testSHA()
{
    crypto::SHA1 sha1;
    sha1.update("360#as");
    LOG_I("sha1(360#as)=%s", sha1.final().c_str());
    const std::string originFile = std::string(originPath).append("img_test_result.png");
    LOG_I("sha1(originFile)=%s", sha1.from_file(originFile).c_str());
}

void testUrl()
{
    std::string url1 = "123@$%$*언서ハングルКирилл үсэг你瞧瞧332";
    std::string url1encode = crypto::urlencode(url1);
    LOG_I("%s", url1encode.c_str());
    std::string url1decode = crypto::urldecode(url1encode);
    LOG_I("%s", url1decode.c_str());
    if (0 == url1.compare(url1decode))
    {
        LOG_I("url1 crypto succuess");
    }
    else
    {
        LOG_I("url1 crypto failed");
    }
    std::string url2 = "https://www.example.com/s?key0=你好&&key1=测试";
    std::string url2encode = crypto::urlencode(url2, true, false);
    LOG_I("%s", url2encode.c_str());
    std::string url2decode = crypto::urldecode(url2encode);
    LOG_I("%s", url2decode.c_str());
    if (0 == url2.compare(url2decode))
    {
        LOG_I("url2 crypto succuess");
    }
    else
    {
        LOG_I("url2 crypto failed");
    }
}

// 测量排序时间的函数模板
template <typename T>
double measureSortTime(std::vector<T> &arr, void (*sort_func)(std::vector<T> &))
{
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
template <typename T>
double measureSortTime2(std::vector<T> &arr, void (*sort_func)(std::vector<T> &, int arg0, int arg1))
{
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << "-> ";
    auto start_time = std::chrono::high_resolution_clock::now();
    sort_func(arr, 0, arr.size() - 1);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    for (auto i : arr)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return elapsed_time.count();
}

void testSort()
{
    std::vector<int> my_vector = {114115, 13, 1, 4, 126, 1, 5, 9, 45, 2, 6, 5, 3};

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

void testJson()
{
    std::string jsonStr = "{ \"name\": \"Alice\", \"age\": 25,\n\t \"arr\":[1,\n"
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

void testSerializer()
{
    // FIXME free(): invalid pointer
    struct Person
    {
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

void testYml()
{
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

void testSgf()
{
    std::string sgfStr = "(;SZ[19]AP[MultiGo:3.6.0 \\[escaped\\] tail ]AB[pb:pc][oc][od][ne][nf][og][pg][qg][rg][rf]AW[qf][pf][of][oe][re][qd][qc][pd]\n"
                         "(;B[sd](;W[rb]    ;B[qe](;W[pe]    ;B[rd]    ;W[se]    ;B[sf]    ;W[qe]    ;B[qb]    ;W[rc]    ;B[ra])\n"
                         "(;W[rd]    ;B[sc]    ;W[se]    ;B[pe]    ;W[qb]    ;B[qa]    ;W[ra]    ;B[sb])\n"
                         ")\n"
                         "(;W[se]    ;B[rb]    ;W[rc]    ;B[sc]    ;W[qb]    ;B[qa])\n"
                         ")\n"
                         "(;B[rb]    ;W[rc]    ;B[sd]    ;W[sc]    ;B[se]    ;W[qb]    ;B[qa]    ;W[ra]    ;B[sa]    ;W[sb])\n"
                         ")";
    sgf::SgfParser parser;
    auto game = parser.parse(sgfStr);
    game.print();
    sgfStr = "(;GM[1]FF[4]SZ[19]HA[0]KM[0]GN[Cho L&D (abc)]\
        AB[bb][cb][db][fb]\
        AW[ea][eb][bc][cc][dc]C[Advanced]\
            (;B[ec];W[fc];B[ed];W[gb]\
                    (;B[fd];W[gc]\
                        (;B[ab];W[ba]\
                            (;B[bd];W[cd];B[ce];W[be]\
                                (;B[dd];W[ad];B[ac]C[Correct.])\
                                (;B[ac];W[ad];B[dd]C[Correct.])\
                            )\
                            (;B[ce]WV[];W[ac]C[Wrong.])\
                        )\
                        (;B[da]WV[];W[fa];B[ab];W[ba]C[Wrong.])\
                    )\
                (;B[ab];W[ba];B[fd];W[gc]\
                    (;B[bd];W[cd];B[ce];W[be]\
                        (;B[dd];W[ad];B[ac]C[Correct.])\
                        (;B[ac];W[ad];B[dd]C[Correct.])\
                    )\
                    (;B[ce]WV[];W[ac]C[Wrong.])\
                )\
                (;B[da]WV[];W[fa];B[ab];W[ba]C[Wrong.])\
            )\
            (;B[da]WV[];W[fc];B[ab];W[ba]C[Wrong.])\
    )";
    game = parser.parse(sgfStr);
    game.print();
}

void testRegex()
{
    std::vector<XRegex::MatchInfo> result = XRegex::getMatch("helloo123world456", "\\d+", false);
    for (const auto &info : result)
    {
        LOG_I("%d,%s", info.pos, info.str.c_str());
    }
    std::string r = XRegex::replace("hello123world456", "\\d+", "数", -1);
    LOG_I("%s", r.c_str());
}

void testMatrix()
{
    Matrix A(5, 5);
    Matrix B(5, 5);
    A(2, 2) = 3;
    B(2, 2) = 2;
    LOG_I("A:\n%s", A.toString().c_str());
    LOG_I("B:\n%s", B.toString().c_str());
    auto C = A + B;
    LOG_I("A+B:\n%s", C.toString().c_str());
    std::vector<std::vector<double>> matrixD = {{5, 2, 4}, {3, 8, 2}, {6, 0, 4}, {0, 1, 6}};
    Matrix D(matrixD);
    std::vector<std::vector<double>> matrixE = {{2, 4}, {1, 3}, {3, 2}};
    Matrix E = matrixE;
    LOG_I("D:\n%s", D.toString().c_str());
    LOG_I("E:\n%s", E.toString().c_str());
    Matrix F = D * E;
    LOG_I("D*E:\n%s", F.toString().c_str());

    std::vector<std::vector<double>> matrixG = {
        {1, 2, 3, 4},
        {5, -1, 7, 8},
        {8, 10, 11, 12},
        {17, 14, 15, 16}};

    Matrix G = matrixG;

    LOG_I("G.det():\n%d", G.det());

    std::vector<std::vector<double>> matrixH = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    Matrix H = matrixH;

    LOG_I("H:\n%s", H.toString().c_str());

//    auto subArr = H.submatrix(2);
    auto subArr = SubmatrixGenerator(H,2);
    for (auto m : subArr)
    {
        LOG_I("H.submatrix 2:\n%s", m.toString().c_str());
    }

    LOG_I("H rank: %d", H.rank());

    std::vector<std::vector<double>> matrixI = {
        {-16, 4, 3},
        {-2, 1, 0},
        {7, -2, -1},
    };
    Matrix I = matrixI;
    Matrix IV = I.inverse();
    LOG_I("I inverse:\n %s", IV.toString().c_str());

    // 创建矩阵 J

    std::vector<std::vector<double>> matrixJ = {
        {1, -3, 3},
        {3, -5, 3},
        {6, -6, 4}};
    Matrix J = matrixJ;
    // 输出矩阵 J
    std::cout << "J = " << std::endl
              << J << std::endl;

    // 计算矩阵 J 的特征多项式
    double lambda = 2;
    auto f = J.charPoly(lambda);
    std::cout << "charPoly(" << lambda << ") = " << std::endl
              << f << std::endl;

    // 使用牛顿迭代法求解矩阵 J 的特征值
    double epsilon = 1e-6;
    int max_iterations = 100;
    std::vector<double> eigenvalues_newton = J.eigenvaluesNewton(epsilon, max_iterations);
    std::cout << "eigenvalues (Newton): ";
    for (double eigenvalue : eigenvalues_newton)
    {
        std::cout << eigenvalue << " ";
    }
    std::cout << std::endl;

    // 使用二分法求解矩阵 J 的特征值
    double left = -10;
    double right = 10;
    std::vector<double> eigenvalues_binary_search = J.eigenvaluesBinarySearch(left, right, epsilon);
    std::cout << "eigenvalues (binary search): ";
    for (double eigenvalue : eigenvalues_binary_search)
    {
        std::cout << eigenvalue << " ";
    }
    std::cout << std::endl;
    
    std::vector<std::vector<double>> LU_A = {{2, -1, 3},
                                          {4, 2, -2},
                                          {-2, 3, 1}};
    std::vector<std::vector<double>> LU_L, LU_U;

    luDecomposition(LU_A, LU_L, LU_U);
    
    Matrix mL = LU_L;
    Matrix mU = LU_U;
    
    Matrix mA = LU_A;
    LOG_I("LU_A:\n%s",mA.toString().c_str());
    LOG_I("L:\n%s,\nU:\n%s",mL.toString().c_str(),mU.toString().c_str());
    auto LU_A_NEW = mL*mU;
    LOG_I("LU_A_NEW:\n%s",LU_A_NEW.toString().c_str());
    LOG_I("L*U %s LU_A",mA==LU_A_NEW?"equals":"not equals");
}

void testMath()
{
    int a = 15, b = 36, c = 6, d = 42;
    int g = gcd({a, b, c, d});
    LOG_I("gcd(%d,%d,%d,%d)=%d", a, b, c, d, g);
    int cm = lcm({a, b, c, d});
    LOG_I("lcm(%d,%d,%d,%d)=%d", a, b, c, d, cm);
    std::vector<int> arr = {0, 1, 2, 3, 4};
    auto perm = permutation(arr, 2);
    LOG_I("permutation:\n%s", XString::toString(perm).c_str());

    auto com = combination(arr, 2);

    LOG_I("combination:\n%s", XString::toString(com).c_str());

     std::vector<int> nums = {};
     for(int i = 0 ; i< 1000000; i++){
        nums.push_back(i);
     }
    auto start_time = std::chrono::high_resolution_clock::now();
    int t = 0;
    for(auto e: nums){
        t+=e;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - start_time;

    LOG_I("Single compute:time:%f 毫秒,result:%d",elapsed_time.count(),t);

    auto parallel_start_time = std::chrono::high_resolution_clock::now();
    int parallel_total = parallelSum(nums);
    auto parallel_end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> parallel_elapsed_time = parallel_end_time - parallel_start_time;
    LOG_I("Parallel compute sum :time:%f 毫秒,result:%d",parallel_elapsed_time.count(),parallel_total);
}

void testGaussianElimination()
{
    // 高斯消元解方程组
    std::vector<std::vector<double>> matrix = {
        {1, 1},
        {3, -3},
    };
    std::vector<double> B = {
        1,1};
    Matrix A = matrix;
    auto result = A.gaussianElimination(B);
    LOG_I("GaussianElimination result:%s", XString::toString(result).c_str());
}

void testSolveLightsOutPuzzle()
{
    int lightSize = 10;
    LOG_I("solveLightsOutPuzzle %dx%d : \n%s", lightSize, lightSize, Matrix::solveLightsOutPuzzle(lightSize).toString().c_str());
    // expect output:
    /**
         solveLightsOutPuzzle 10x10 :
         1 0 1 0 0 0 0 1 0 1
         0 1 0 0 1 1 0 0 1 0
         1 0 1 0 1 1 0 1 0 1
         0 0 0 1 0 0 1 0 0 0
         0 1 1 0 1 1 0 1 1 0
         0 1 1 0 1 1 0 1 1 0
         0 0 0 1 0 0 1 0 0 0
         1 0 1 0 1 1 0 1 0 1
         0 1 0 0 1 1 0 0 1 0
         1 0 1 0 0 0 0 1 0 1
     */
}

void testXML()
{
    std::string xmlStr = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                         "<bookstore>\n"
                         "  <book category=\"COOKING\">\n"
                         "    <title lang=\"en\">Everyday Italian</title>\n"
                         "    <author>Giada De Laurentiis</author>\n"
                         "    <year>2005</year>\n"
                         "    <price>30.00</price>\n"
                         "  </book>\n"
                         "  <book category=\"CHILDREN\">\n"
                         "    <title lang=\"en\">Harry Potter</title>\n"
                         "    <author>J.K. Rowling</author>\n"
                         "    <year>2005</year>\n"
                         "    <price>29.99</price>\n"
                         "  </book>\n"
                         "  <book category=\"WEB\">\n"
                         "    <title lang=\"en\">Learning XML</title>\n"
                         "    <author>Erik T. Ray</author>\n"
                         "    <year>2003</year>\n"
                         "    <price>39.95</price>\n"
                         "  </book>\n"
                         "</bookstore>";
    xlib::XMLParser parser;
    xlib::XMLNode *rootNode = parser.parse(xmlStr);
    if (rootNode != nullptr)
    {
        std::cout << "XML parsing successful!" << std::endl;
        std::string nodeStr = nodeToString(rootNode);
        std::cout << nodeStr << std::endl;
    }
    else
    {
        std::cout << "XML parsing failed!" << std::endl;
    }
}

void testAStar()
{
    std::vector<int> mapArr = {
        0, 0, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 0, 0, 1,
        0, 0, 0, 0, 0};

    int w = 5;     // Map width
    int start = 0; // Start position
    int end = 24;  // End position

    // Find the shortest path using A* Search Algorithm
    std::vector<int> shortestPath = xlib::AStarSearch(mapArr, w, start, end);

    // Print the shortest path
    if (shortestPath.empty())
    {
        LOG_I("Shortest path");
    }
    else
    {
        LOG_I("Shortest path:%s", XString::toString(shortestPath).c_str());
    }
}

std::string sudokuGrid(const std::string &subject)
{
    int size = std::sqrt(subject.size());
    std::string result;
    // 构建字符串表示的数独网格
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int index = i * size + j;
            int digit = subject[index] - '0';
            result += std::to_string(digit) + " ";
        }
        result += "\n";
    }
    return result;
}

void testSolveStandardSudoku()
{
    std::string subject = "500002000400708001083000900000000390040070010065000000001000720800605009000900008";
    LOG_I("subject is \n%s", sudokuGrid(subject).c_str());
    std::string answer = solveStandardSudoku(subject);
    /**
     expect output:
     5 1 6 4 9 2 8 7 3
     4 9 2 7 3 8 5 6 1
     7 8 3 5 6 1 9 4 2
     1 7 8 2 5 6 3 9 4
     2 4 9 8 7 3 6 1 5
     3 6 5 1 4 9 2 8 7
     9 5 1 3 8 4 7 2 6
     8 2 7 6 1 5 4 3 9
     6 3 4 9 2 7 1 5 8
     */
    LOG_I("answer is \n%s", sudokuGrid(answer).c_str());
}

void testDecodeAndEncode()
{
    const std::string originStr = "Stay up-to-date on emerging technologies.(so.~!@#$%^&*)_+-=,';:\"<>?\\`";
    std::string encodeStr = crypto::encode85(originStr);
    LOG_I("encode85 is %s", encodeStr.c_str());
    std::string decodeStr = crypto::decode85(encodeStr);
    LOG_I("decode85 is %s", decodeStr.c_str());
    if (originStr == decodeStr)
    {
        std::cout << "Right !!! The originStr and decodeStr are equal." << std::endl;
    }
    else
    {
        std::cout << "Error !!! The originStr and decodeStr are not equal." << std::endl;
    }
}

void testString(){
    std::string word1 = "word hehe";
    std::string word2 = "word hhh";
    int distance = XString::levenshteinDistance(word1, word2);
    LOG_I("%s => %s = %d", word1.c_str(),word2.c_str(),distance);
}

void testFraction() {
    LOG_I("=== Start Testing Fraction Class ===");
    
    // 1. 测试构造函数和基本表示
    {
        LOG_I("Testing constructors and basic representation:");
        Fraction f1(1, 2);
        Fraction f2(-1, 2);
        Fraction f3(2, -4);  // 应该被规范化为 -1/2
        Fraction f4(0, 5);   // 应该被简化为 0/1
        
        LOG_I("f1(1,2) = %s", f1.toString().c_str());
        LOG_I("f2(-1,2) = %s", f2.toString().c_str());
        LOG_I("f3(2,-4) = %s", f3.toString().c_str());
        LOG_I("f4(0,5) = %s", f4.toString().c_str());
        
        // 测试分母为0的情况
        try {
            Fraction f5(1, 0);
            LOG_E("Failed: Should throw exception for denominator = 0");
        } catch (const std::runtime_error& e) {
            LOG_I("Successfully caught exception: %s", e.what());
        }
    }
    
    // 2. 测试四则运算
    {
        LOG_I("\nTesting arithmetic operations:");
        Fraction f1(1, 2);
        Fraction f2(1, 3);
        
        Fraction sum = f1 + f2;
        Fraction diff = f1 - f2;
        Fraction prod = f1 * f2;
        Fraction quot = f1 / f2;
        
        LOG_I("%s + %s = %s", f1.toString().c_str(), f2.toString().c_str(), sum.toString().c_str());
        LOG_I("%s - %s = %s", f1.toString().c_str(), f2.toString().c_str(), diff.toString().c_str());
        LOG_I("%s * %s = %s", f1.toString().c_str(), f2.toString().c_str(), prod.toString().c_str());
        LOG_I("%s / %s = %s", f1.toString().c_str(), f2.toString().c_str(), quot.toString().c_str());
        
        // 测试除以0的情况
        try {
            Fraction zero(0, 1);
            Fraction result = f1 / zero;
            LOG_E("Failed: Should throw exception for division by zero");
        } catch (const std::runtime_error& e) {
            LOG_I("Successfully caught exception: %s", e.what());
        }
    }
    
    // 3. 测试约分功能
    {
        LOG_I("\nTesting fraction reduction:");
        Fraction f1(2, 4);    // 应该被约分为 1/2
        Fraction f2(15, 25);  // 应该被约分为 3/5
        Fraction f3(-6, 8);   // 应该被约分为 -3/4
        
        LOG_I("2/4 reduces to %s", f1.toString().c_str());
        LOG_I("15/25 reduces to %s", f2.toString().c_str());
        LOG_I("-6/8 reduces to %s", f3.toString().c_str());
    }
    
    // 4. 测试相等性比较
    {
        LOG_I("\nTesting equality:");
        Fraction f1(1, 2);
        Fraction f2(2, 4);
        Fraction f3(3, 6);
        Fraction f4(2, 3);
        
        LOG_I("%s == %s : %d", f1.toString().c_str(), f2.toString().c_str(), f1 == f2);
        LOG_I("%s == %s : %d", f1.toString().c_str(), f3.toString().c_str(), f1 == f3);
        LOG_I("%s == %s : %d", f1.toString().c_str(), f4.toString().c_str(), f1 == f4);
    }
    
    // 5. 测试转换功能
    {
        LOG_I("\nTesting conversions:");
        Fraction f1(1, 2);
        Fraction f2(3, 4);
        Fraction f3(-5, 6);
        
        LOG_I("%s as double = %f", f1.toString().c_str(), f1.toDouble());
        LOG_I("%s as double = %f", f2.toString().c_str(), f2.toDouble());
        LOG_I("%s as double = %f", f3.toString().c_str(), f3.toDouble());
    }
    
    // 6. 测试大数运算
    {
        LOG_I("\nTesting large numbers:");
        Fraction::IntType large1 = 1000000000; // 10亿
        Fraction::IntType large2 = 2000000000; // 20亿
        
        try {
            Fraction f1(large1, large2);
            Fraction f2(large2, large1);
            Fraction result = f1 * f2;
            LOG_I("Large number multiplication: %s * %s = %s",
                f1.toString().c_str(), f2.toString().c_str(), result.toString().c_str());
        } catch (const std::exception& e) {
            LOG_E("Error with large numbers: %s", e.what());
        }
    }
    
    // 7. 测试连续运算
    {
        LOG_I("\nTesting chained operations:");
        Fraction f1(1, 2);
        Fraction f2(1, 3);
        Fraction f3(1, 4);
        
        Fraction result = (f1 + f2) * f3;
        LOG_I("(%s + %s) * %s = %s",
            f1.toString().c_str(), f2.toString().c_str(),
            f3.toString().c_str(), result.toString().c_str());
    }
    
    LOG_I("=== Fraction Testing Complete ===\n");
}

void testRankScore() {
    LOG_I("=== 开始测试 ELO 和 Glicko2 评分算法 ===");
    
    // 测试 ELO 评分系统
    {
        LOG_I("\n测试 ELO 评分系统:");
        
        // 测试预期胜率计算
        double playerRating = 1500.0;
        double opponentRating = 1600.0;
        double expectedScore = xlib::algorithm::EloRating::expectedScore(playerRating, opponentRating);
        LOG_I("玩家评分: %.2f, 对手评分: %.2f, 预期胜率: %.4f", 
              playerRating, opponentRating, expectedScore);
        
        // 测试评分变化计算
        double result = 1.0; // 胜利
        double kFactor = 10.0;
        double ratingChange = xlib::algorithm::EloRating::calculateRatingChange(
            playerRating, opponentRating, result, kFactor);
        LOG_I("比赛结果: 胜利, 评分变化: %.2f", ratingChange);
        
        // 测试使用分数的评分变化计算
        Fraction fractionRatingChange = xlib::algorithm::EloRating::calculateRatingChangeFraction(
            playerRating, opponentRating, result, kFactor);
        LOG_I("使用分数的评分变化: %s", fractionRatingChange.toString().c_str());
        
        // 测试批量评分变化计算
        std::vector<double> opponentRatings = {1600.0, 1550.0, 1450.0};
        std::vector<double> results = {1.0, 0.5, 0.0}; // 胜、平、负
        double batchRatingChange = xlib::algorithm::EloRating::calculateRatingChangeBatch(
            playerRating, opponentRatings, results, kFactor);
        LOG_I("批量比赛后的评分变化: %.2f", batchRatingChange);
    }    
    LOG_I("=== ELO 评分算法测试完成 ===\n");

    {
        LOG_I("\n测试 Glicko2 评分系统:");
        algorithm::Glicko2Player player(1500.0, 200.0, 0.06);
        
        std::vector<algorithm::Glicko2Player> opponents = {
            algorithm::Glicko2Player(1400.0, 30.0),
            algorithm::Glicko2Player(1550.0, 100.0),
            algorithm::Glicko2Player(1700.0, 300.0)
        };
        
        std::vector<double> scores = {1.0, 0.0, 0.0}; // Win, Loss, Loss
        
        algorithm::Glicko2System system(0.5);
        system.updatePlayer(player, opponents, scores);
        
        // std::cout << std::fixed << std::setprecision(2);
        LOG_I("Updated Rating: %f", player.rating);
        LOG_I("Updated RD: %f",player.rd );
        LOG_I("Updated Volatility: %.5f",player.volatility);
        LOG_I("=== Glicko2 评分算法测试完成 ===\n");
    }

}


int main(int argc, char *argv[])
{
    std::cout <<XString::toStringAddEnter(XString::lettersShape("xlib-test"))<< std::endl;
    setLog();
    testFraction();
    testString();
    testRankScore();
//    testPatch();
    testAStar();
    testMath();
    testMatrix();
    testGaussianElimination();
    testSolveLightsOutPuzzle();
    testRegex();
    testYml();
    testSgf();
    // testSerializer();
    testJson();
    testXML();
    testThreadPool();
    testSHA();
    testUtf8();
    testFile();
    testUrl();
    testSort();
    testSolveStandardSudoku();
    testDecodeAndEncode();
    return 0;
}
