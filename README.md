
[![XLIB](https://travis-ci.org/xiaominghe2014/C-11-XLib.svg)](https://travis-ci.org/xiaominghe2014/C-11-XLib/jobs/139774823)
##What is XLIB?

---
**Xlib is a collection of c + + commonly used functions and basic components.   
It is for the purpose of cross-platform and reuse.   
Including the logging system, thread pool, timer, TCP communications, and so on.  
You can easily use it in your program.**

##Usage
---

###1. [log](#log)
###2. [thread](#thread)
###3. [timer](#timer)
###4. [socket](#socket)

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
<h4 id='thread'> thread pool</h4>

```C++
auto fun = []
{
std::this_thread::sleep_for(std::chrono::seconds(rand()%50));
std::thread::id tid = std::this_thread::get_id();
auto t = XString::formatTime(XTime::getTimeFromTimestamp_milliseconds(XTime::getTimestamp_milliseconds(),8),TIME_F::T_DEFAULT);
LOG_I("thread_id=%d,%s",tid,t.c_str());
};
/** XThreadPool's first constructor parameter means thread max number,
the second is task max number,
the last is Whether begin immediately.
*/
auto pool = std::shared_ptr<XThreadPool>(new XThreadPool(5,5,true));

pool->addTask(fun);
pool->addTask(fun);
pool->addTask(fun);
pool->addTask(fun);
pool->detach();
```
<h4 id='timer'>timer</h4>

```C++
/**at first,you defined a timer's attributes:
1.repeate number
2.the number of seconds 
3.the callback
*/
XTime::doPertime(-1, 0.5,[]
{
LOG_D("this is a timer,1 second 2 times");
} );

//then ,you add a loop to do it.
auto loop = []
{
while (true)
{
XTimer::getInstance()->mainLoop();
}
};
```

<h4 id='socket'> Socket</h4>

```C++

/** Below is the tcp server test code*/
xlib::net::XSocketTCP server;
server.startServer(4435);

//if you take client,you can do as follow

auto tcp = std::shared_ptr<net::XSocketTCP>(new net::XSocketTCP);
tcp->startClient(net::_server(2347,"120.27.94.221"),true);
std::string chutf8 = "这是一个字符串";
auto s = XUtf8::utf8ToUnicode(chutf8);
LOG_I(s.c_str());
tcp->Send(chutf8.c_str(), sizeof(chutf8));
```
##Notes
---
1. *Compile environment*: need GCC 4.8 or above, more than clang3.1.     
2. *Test*: the whole project, on the MAC and centos test has no problem. Perform the make, then run the generated script.

##Q&A
---
You have any question, welcome to discuss.  
And I also hope that we can put forward valuable Suggestions and comments. You can contact me through the following :

*Email:xiaominghe2014@gmail.com
* QQ: 229827701

