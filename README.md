
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

###  [log](#log)
###  [yaml parser](#yaml)
###  [sgf parser](#sgf)
###  [json parser](#json)
###  [Serializer](#Serializer)
###  [thread](#thread)


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
    auto v = parser->getmoveList();
    for(auto s:v){
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

