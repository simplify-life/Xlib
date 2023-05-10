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
#include "macro/XBase.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <ctime>
#include <atomic>
XLIB_BEGAIN

using steady_point = std::chrono::steady_clock::time_point;
using system_point = std::chrono::system_clock::time_point;

class XTimer;
namespace XTime
{
    struct tm_million{
        std::tm* tm;
        uint32 million;
    };
    
    enum class TIMER_LEVEL{
        L_SECOND,
        L_MILLION,
        L_MICRO
    };
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
    std::time_t getTimestamp_steady_mills();
    std::time_t getTimestamp_steady_seconds();
    /**
     @param t       timestamp
     @param timeInterval        time zone diff
     */
    tm_million* getTimeFromTimestamp_milliseconds(std::time_t t,int timeInterval=0);
    std::tm* getTimeFromTimestamp_seconds(std::time_t t,int timeInterval=0);
    
    uint64 getNanosecondsCount();
    
    void startTimer(uint32 count, float interval, const std::function<void ()> &call);
};



XLIB_END
#endif /* XTime_h */
