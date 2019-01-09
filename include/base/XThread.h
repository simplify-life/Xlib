//----------------------------------------------//
//  XThread.h
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XThread_h
#define XThread_h

#include "macro/XBase.h"
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <future>
#include <queue>

XLIB_BEGAIN

class XThreadPool{
public:
    using Task = std::function<void()>;
    using PackagedTask = std::packaged_task<void()>;
    XThreadPool();
    XThreadPool(uint threadCount);
     ~XThreadPool();
    template <typename Function, typename... Args>
    inline std::shared_future<void> async(Function &&F, Args &&... ArgList) {
        auto task =
        std::bind(std::forward<Function>(F), std::forward<Args>(ArgList)...);
        return asyncImpl(std::move(task));
    }
    template <typename Function>
    inline std::shared_future<void> async(Function &&F) {
        return asyncImpl(std::forward<Function>(F));
    }
    void wait();
private:
    std::shared_future<void> asyncImpl(Task F);
    std::vector<std::thread> mThreads; ///<所有线程
    std::queue<PackagedTask> mTaskQueue; ///<任务队列
    std::mutex queueLock;  ///<任务队列锁
    std::condition_variable queueCondition;
    std::mutex readyLock;  ///<准备条件锁
    std::condition_variable readyCondition;
    std::atomic<unsigned> activeThreads; ///<工作中的线程
    bool exitFlag; ///<是否退出
};

XLIB_END
#endif /* XThread_h */
