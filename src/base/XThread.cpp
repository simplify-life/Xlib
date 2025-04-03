//----------------------------------------------//
//  XThread.cpp
// xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "base/XThread.h"
#include <assert.h>
#include <utility>

#include "macro/XDefine.h"
using namespace std;

XLIB_BEGAIN

XThreadPool::XThreadPool() : XThreadPool(thread::hardware_concurrency()) {}
XThreadPool::XThreadPool(uint threadCount)
: activeThreads(0), exitFlag(false) {
    mThreads.reserve(threadCount);
    for (uint t = 0; t < threadCount; t++) {
        mThreads.emplace_back([&] {
            while (true) {
                PackagedTask task;
                {
                    unique_lock<mutex> LockGuard(queueLock);
                    queueCondition.wait(LockGuard,
                                        [&] { return exitFlag || !mTaskQueue.empty(); });
                    if (exitFlag && mTaskQueue.empty()) return;
                    {
                        unique_lock<mutex> LockGuard(readyLock);
                        activeThreads++;
                    }
                    task = move(mTaskQueue.front());
                    mTaskQueue.pop();
                }
                task();
                {
                    unique_lock<mutex> LockGuard(readyLock);
                    --activeThreads;
                }
                readyCondition.notify_all();
            }
        });
    }
}

void XThreadPool::wait() {
    unique_lock<mutex> LockGuard(readyLock);
    readyCondition.wait(LockGuard,
                             [&] { return !activeThreads && mTaskQueue.empty(); });
}
shared_future<void> XThreadPool::asyncImpl(Task task) {
    PackagedTask PackagedTask(move(task));
    auto Future = PackagedTask.get_future();
    {
        unique_lock<mutex> LockGuard(queueLock);
        if(exitFlag){
            LOG_E("XThreadPool exited:线程池已经退出");
        }
        mTaskQueue.push(move(PackagedTask));
    }
    readyCondition.notify_one();
    return Future.share();
}

XThreadPool::~XThreadPool() {
    {
        unique_lock<mutex> LockGuard(queueLock);
        exitFlag = true;
    }
    if(mTaskQueue.size()){
        LOG_W("%lu tasks not processed when thread pool exited\n",mTaskQueue.size());
    }
    queueCondition.notify_all();
    for (auto &e : mThreads){
        if(e.joinable()) e.join();
    }
}
XLIB_END
