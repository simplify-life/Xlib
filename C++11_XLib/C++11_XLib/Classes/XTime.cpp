//----------------------------------------//
//  XTime.cpp
//  Copyright (c) 2016/5/24 Ximena
//
//  Created by Ximena on 2016/5/24.
//  Email: xiaominghe2014@gmail.com
//----------------------------------------//

#include "XTime.h"

using namespace std;
using namespace std::chrono;

//std::timed_mutex mtx_time;

XTime* XTime::mTime=nullptr;
XTimer* XTimer::tSelf=nullptr;
XTime::XTime():mTimer()
{
}

XTime::~XTime()
{
//    if(mTimer)
//    {
//        delete mTimer;
//        mTimer = 0;
//    }
}

XTime* XTime::getInstance()
{
    if(!mTime) mTime = new XTime;
    return mTime;
}

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


tm* XTime::getTimeFromTimestamp_milliseconds(time_t t,int timeInterval)
{
    time_t t_t = t + (time_t)timeInterval*60*60*1000;
    auto mTime = milliseconds(t_t);
    auto tp=time_point<system_clock,milliseconds>(mTime);
    auto tt=system_clock::to_time_t(tp);
    return gmtime(&tt);
}


tm* XTime::getTimeFromTimestamp_seconds(time_t t,int timeInterval)
{
    time_t tt = t + (time_t)timeInterval*60*60;
    return gmtime(&tt);
}


void XTime::doPertime(int count,float interval, const std::function<void ()>& call)
{
    if(!mTimer)
    {
        mTimer = XTimer::getInstance();
        mTimer->start();
    }
    mTimer->addTask(timerData(count,interval,call));
}

////////////////////////////////////////////////////////////////////////

XTimer::XTimer():tThread(),tTask()
{

}

XTimer::~XTimer()
{

}

XTimer* XTimer::getInstance()
{
    if(!tSelf) tSelf = new XTimer;
    return tSelf;
}

void XTimer::start()
{
    tThread=thread(std::bind(&XTimer::threadLoop,this));
    tThread.detach();
//    thread(std::bind(&XTimer::threadLoop,this)).detach();
}

void XTimer::mainLoop()
{

    lock_guard<mutex> lck(tTaskMutex);
    for(auto &e:tTask)
    {
        if(e.t_handler&&e.t_call)
        {
            e.t_handler = false;
            e.t_call();
        }
    }
}

void XTimer::threadLoop()
{
    while (true)
    {
        this_thread::sleep_for(milliseconds(1));
        lock_guard<mutex> lck(tTaskMutex);
        for(auto itr=tTask.begin();itr!=tTask.end();itr++)
        {
            auto tNow = time_point_cast<milliseconds>(steady_clock::now()).time_since_epoch().count();
            auto interval = tNow-itr->t_point;
            if(interval>=itr->t_interval*1000&&itr->t_call&&itr->t_count)
            {
                itr->t_point = tNow;
                itr->t_handler = true;
                itr->t_count--;
            }
            else
            {
                if(itr->t_count==0&&itr!=tTask.end())
                {
                    itr->t_call=nullptr;
                    itr=tTask.erase(itr);
                }
            }
        }
    }
}

void XTimer::addTask(const timerData &data)
{
    lock_guard<mutex> lck(tTaskMutex);
    tTask.emplace_back(data);
}
