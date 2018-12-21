//----------------------------------------//
//  XTime.cpp
//  Copyright (c) 2016/5/24 Ximena
//
//  Created by Ximena on 2016/5/24.
//  Email: xiaominghe2014@gmail.com
//----------------------------------------//

#include "base/XTime.h"

using namespace std;
using namespace std::chrono;

//std::timed_mutex mtx_time;
XLIB_BEGAIN

steady_point XTime::getTimePoint_steady()
{
    return steady_clock::now();
}

system_point XTime::getTimePoint_system()
{
    return system_clock::now();
}

time_t XTime::getTimeInterval_steady(steady_point p0, steady_point p1)
{
    return (p1-p0).count()*steady_clock::period::num/steady_clock::period::den;
}

time_t XTime::getTimeInterval_system(system_point p0, system_point p1)
{
    return (p1-p0).count()*system_clock::period::num/system_clock::period::den;
}

time_t XTime::getTimestamp_milliseconds()
{
    time_point<system_clock,milliseconds> tp = time_point_cast<milliseconds>(system_clock::now());
    auto tmp=duration_cast<milliseconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}

time_t XTime::getTimestamp_seconds()
{
    time_point<system_clock,seconds> tp = time_point_cast<seconds>(system_clock::now());
    return  system_clock::to_time_t(tp);
}

time_t XTime::getTimestamp_steady_mills()
{
    time_point<steady_clock,milliseconds> tp = time_point_cast<milliseconds>(steady_clock::now());
    auto tmp=duration_cast<milliseconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}

time_t XTime::getTimestamp_steady_seconds()
{
    time_point<steady_clock,seconds> tp = time_point_cast<seconds>(steady_clock::now());
    auto tmp=duration_cast<seconds>(tp.time_since_epoch());
    time_t timestamp = tmp.count();
    return timestamp;
}


XTime::tm_million* XTime::getTimeFromTimestamp_milliseconds(time_t t,int timeInterval)
{
    uint32 million = t%1000;
    time_t t_t = t + (time_t)timeInterval*60*60*1000;
    auto mTime = milliseconds(t_t);
    auto tp=time_point<system_clock,milliseconds>(mTime);
    auto tt=system_clock::to_time_t(tp);
    tm* tm =  gmtime(&tt);
    XTime::tm_million* t_m = (XTime::tm_million*)malloc(sizeof(XTime::tm_million));
    t_m->tm = tm;
    t_m->million = million;
    return t_m;
}


tm* XTime::getTimeFromTimestamp_seconds(time_t t,int timeInterval)
{
    time_t tt = t + (time_t)timeInterval*60*60;
    return gmtime(&tt);
}


////////////////////////////////////////////////////////////////////////

XTimer::XTimer():_execute(false){}

XTimer::~XTimer()
{
    if(_execute.load(std::memory_order_acquire))
    stop();
}

void XTimer::start(uint32 count, float interval, const std::function<void ()> &call)
{
    if( _execute.load(std::memory_order_acquire) ) {
        stop();
    };
    _execute.store(true, std::memory_order_release);
    
    tThread = std::thread([this, interval, call, count]()
                       {
                           while (_execute.load(std::memory_order_acquire)) {
                               for (uint32 i = 0 ; i< count ; i++){
                                   this_thread::sleep_for(nanoseconds((uint64)(interval*1000*1000*1000)));
                                   call();
                               }
                               _execute.store(false, std::memory_order_release);
                           }
                       });
    tThread.join();
}


void XTimer::stop()
{
    _execute.store(false, std::memory_order_release);
    if( tThread.joinable() )
    tThread.join();
}

bool XTimer::is_running() const noexcept{
    return (_execute.load(std::memory_order_acquire)) &&
    (tThread.joinable());
}

XLIB_END
