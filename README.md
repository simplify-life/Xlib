# C-11-XLib
this's a custom lib with C++11,it contains such as timer ,log ,thread pool

1. 
You can use timer like the code main.cpp.
* ## step1: `XTime::getInstance()->doPertime(...)`
* ## step2: add loop 
`     while (true)`
    `{`
        `XTimer::getInstance()->mainLoop();`
    `}`

this my test result:

![C++11 Timer test_result](https://github.com/xiaominghe2014/C-11-Timer/blob/master/img_test_result.png?raw=true)

///////
2. 
## and now you can test just execute make command as which i do on CentOS.

**## Test result on mac Xcode as follow:**
![test on mac Xcode](https://github.com/xiaominghe2014/C-11-XLib/blob/master/res/img_test_on_mac.png?raw=true)



**## Test result on CentOS as follow:**
![test on CentOS](https://github.com/xiaominghe2014/C-11-XLib/blob/master/res/img_test_on_centos.png?raw=true)
