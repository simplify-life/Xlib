
[![CMake](https://github.com/simplify-life/Xlib/actions/workflows/cmake.yml/badge.svg)](https://github.com/simplify-life/Xlib/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/simplify-life/Xlib/actions/workflows/codeql.yml/badge.svg)](https://github.com/simplify-life/Xlib/actions/workflows/codeql.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT) 
## What is XLIB?

[![Join the chat at https://gitter.im/xiaominghe2014/C-11-XLib](https://badges.gitter.im/xiaominghe2014/C-11-XLib.svg)](https://gitter.im/xiaominghe2014/C-11-XLib?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

---
**Xlib is a collection of c + + commonly used functions and basic components.     
It is for the purpose of cross-platform and reuse.     
Including the logging system, thread pool, markup language parser(JSON,YAML,SGF),object serialize and deserialize, timer, TCP communications, and so on.  
You can easily use it in your program.**

## Usage
---

###  [log print](#log)
###  [yaml parser](#yaml)
###  [sgf parser](#sgf)
###  [json parser](#json)
###  [Serializer Object](#Serializer)
###  [thread use](#thread)
###  [math method](#math)
###  [matrix about](#matrix)
### [LightsOutPuzzle slover](#LightsOutPuzzle)
### [Sudoku slover](#Sudoku)

<h4 id='log'>log </h4>

```C++
//set log level
LOG_SET(LOG_LEVEL::L_ALL); 
//you can set the specified log file to record
XLog::setWrite(true, XFileUtil::getCurrentPathWithPrefix().append("xliblog"));
/**usage example.
LOG_I------'I' means info. 
And by that analogy:
LOG_D------'D' means debug.
LOG_E------'E' means error.
...
*/
std::string s="this is a log test!";
LOG_I("%s",s.c_str());

```

<h4 id='yaml'>yaml parser</h4>

```C++
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

```

<h4 id='sgf'>sgf parser </h4>

```C++
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
    auto v = parser->getSingleSgf();
    for(auto &s:v){
        LOG_I("%s",s.c_str());
    }
    sgfStr ="(;GM[1]FF[4]SZ[19]HA[0]KM[0]GN[Cho L&D (abc)]\
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
    parser->parseSgf(sgfStr);
    v = parser->getSingleSgf();
    for(auto &s:v){
        LOG_I("%s",s.c_str());
    }

```

<h4 id='json'>json parser </h4>

```C++
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

```

<h4 id='Serializer'>Serializer </h4>

```C++
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

```

<h4 id='thread'> thread pool</h4>

```C++
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
```


<h4 id='math'>math </h4>

```C++

    int a = 15,b=36,c=6,d=42;
    int g = gcd({a,b,c,d});
    LOG_I("gcd(%d,%d,%d,%d)=%d",a,b,c,d,g);
    int cm = lcm({a,b,c,d});
    LOG_I("lcm(%d,%d,%d,%d)=%d",a,b,c,d,cm);
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

```

<h4 id='matrix'>matrix </h4>

```C++

    Matrix  A(5,5);
    Matrix  B(5,5);
    A(2,2) = 3;
    B(2,2) = 2;
    LOG_I("A:\n%s",A.toString().c_str());
    LOG_I("B:\n%s",B.toString().c_str());
    auto C = A+B;
    LOG_I("A+B:\n%s",C.toString().c_str());
    std::vector<std::vector<double>> matrixD = {{5,2,4},{3,8,2},{6,0,4},{0,1,6}};
    Matrix D(matrixD);
    std::vector<std::vector<double>> matrixE = {{2,4},{1,3},{3,2}};
    Matrix E = matrixE;
    LOG_I("D:\n%s",D.toString().c_str());
    LOG_I("E:\n%s",E.toString().c_str());
    Matrix F = D*E;
    LOG_I("D*E:\n%s",F.toString().c_str());
    
    std::vector<std::vector<double>> matrixG = {
        {1,2,3,4},
        {5,-1,7,8},
        {8,10,11,12},
        {17,14,15,16}
    };
    
    Matrix G = matrixG;
    
    LOG_I("G.det():\n%d",G.det());
    
    std::vector<std::vector<double>> matrixH = {
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
    
    std::vector<std::vector<double>> matrixI = {
        {-16,4,3},
        {-2,1,0},
        {7,-2,-1},
    };
    Matrix I = matrixI;
    Matrix IV = I.inverse();
    LOG_I("I inverse:\n %s",IV.toString().c_str());
    
    
    // 创建矩阵 J
 
    std::vector<std::vector<double>> matrixJ = {
        {1,-3,3},
        {3,-5,3},
        {6,-6,4}
    };
    Matrix J = matrixJ;
    // 输出矩阵 J
    std::cout << "J = " << std::endl << J << std::endl;

    // 计算矩阵 J 的特征多项式
    double lambda = 2;
    auto f = J.charPoly(lambda);
    std::cout << "charPoly(" << lambda << ") = " << std::endl<< f << std::endl;
        
    // 使用牛顿迭代法求解矩阵 J 的特征值
    double epsilon = 1e-6;
    int max_iterations = 100;
    std::vector<double> eigenvalues_newton = J.eigenvaluesNewton(epsilon, max_iterations);
    std::cout << "eigenvalues (Newton): ";
    for (double eigenvalue : eigenvalues_newton) {
        std::cout << eigenvalue << " ";
    }
    std::cout << std::endl;

    // 使用二分法求解矩阵 J 的特征值
    double left = -10;
    double right = 10;
    std::vector<double> eigenvalues_binary_search = J.eigenvaluesBinarySearch(left, right, epsilon);
    std::cout << "eigenvalues (binary search): ";
    for (double eigenvalue : eigenvalues_binary_search) {
        std::cout << eigenvalue << " ";
    }
    std::cout << std::endl;

```

<h4 id='LightsOutPuzzle'>LightsOutPuzzle solover</h4>

点灯游戏的解决方案,
点灯游戏网页例子可以参考:https://xiaominghe2014.github.io/game/flipGame.html

```C++

    int lightSize = 10;
    LOG_I("solveLightsOutPuzzle %dx%d : \n%s",lightSize,lightSize,Matrix::solveLightsOutPuzzle(lightSize).toString().c_str());
    //expect output:
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

```


<h4 id='Sudoku'>Sudoku solover</h4>

数独游戏的解决方案,
数独游戏网页例子可以参考:https://xiaominghe2014.github.io/game/sudoku.html

```C++

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

```


## Test
---
1. cmake: you can test use test.sh shell script;
    ```shell
        sh test.sh
    ```
2. make:
    ```shell
        make clean && make && ./xlib
    ```

## Notes
---
1. *Compile environment*: need GCC 8.0 or above.     
2. *TDD*: the whole project, on the MAC and centos test has no problem. Perform the make, then run the generated script.And specific usage see main.cpp.

## Q&A
---
You have any question, welcome to discuss.  
And I also hope that we can put forward valuable Suggestions and comments. You can contact me through the following :

 *Email:xiaominghe2014@gmail.com  

 *QQ: 229827701

