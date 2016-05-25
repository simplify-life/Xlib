//----------------------------------------//
//  XTime.h
//  Copyright (c) 2016/5/24 Ximena
//
//  Created by Ximena on 2016/5/24.
//
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------//

#ifndef XTime_h
#define XTime_h
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>

using steady_point = std::chrono::steady_clock::time_point;
using system_point = std::chrono::system_clock::time_point;
struct timerData
{
    bool t_handler;
    int t_count;
    std::time_t t_interval;
    std::function<void()> t_call;
    timerData(int count,std::time_t interval,const std::function<void()>& call)
    {
        t_handler = false;
        t_count = count;
        t_interval = interval;
        t_call = call;
    };
};

class XTimer;
class XTime
{
public:
    static XTime* getInstance();
    
    /**
     @brief     get time point
     */
    steady_point getTimePoint_steady();
    system_point getTimePoint_system();
    /**
     @brief     get time interval
     */
    std::time_t getTimeInterval_steady(steady_point p0,steady_point p1);
    std::time_t getTimeInterval_system(system_point p0,system_point p1);
    /**
     @brief     get timestamp
     */
    std::time_t getTimestamp_milliseconds();
    std::time_t getTimestamp_seconds();
    /**
     @param t       timestamp
     @param timeInterval        time zone diff
     */
    std::tm* getTimeFromTimestamp_milliseconds(std::time_t t,int timeInterval=0);
    std::tm* getTimeFromTimestamp_seconds(std::time_t t,int timeInterval=0);
    
    /**
     @brief     this thread timer
     */
    void doPertime(int count,std::time_t interval,const std::function<void()>& call);
    
    XTime();
    virtual ~XTime();
private:
    static XTime* mTime;
    XTimer* mTimer;
};

class XTimer
{
public:
    static XTimer* getInstance();
    XTimer();
    virtual ~XTimer();
    void start();
    void addTask(const timerData& data);
    /**
     @note     you need add mainLoop in your engine mainLoop function
     */
    void mainLoop();
protected:
    void threadLoop();
private:
    XTimer(const XTimer& timer) = delete;
    XTimer& operator=(XTimer& timer) = delete;
private:
    static XTimer* tSelf;
    std::thread tThread;
    std::vector<timerData> tTask;
    std::mutex tTaskMutex;
};

#endif /* XTime_h */
