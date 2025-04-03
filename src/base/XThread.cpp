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
    if (shuttingDown) {
        throw std::runtime_error("Thread pool is shutting down");
    }
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
    shutdown();
}
void XThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queueLock);
        if (shuttingDown) return;
        shuttingDown = true;
        exitFlag = true;
    }
    
    // 等待所有任务完成
    wait();
    
    // 通知所有线程退出
    queueCondition.notify_all();
    
    // 等待所有线程结束
    for (auto& thread : mThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    // 记录未处理的任务
    if (!mTaskQueue.empty()) {
        LOG_W("%lu tasks not processed when thread pool exited\n", mTaskQueue.size());
    }
}

void XThreadPool::workerThread() {
    while (!exitFlag) {
        PackagedTask task;
        {
            std::unique_lock<std::mutex> lock(queueLock);
            queueCondition.wait(lock, [this] {
                return exitFlag || !mTaskQueue.empty();
            });
            
            if (exitFlag && mTaskQueue.empty()) {
                break;
            }
            
            if (!mTaskQueue.empty()) {
                task = std::move(mTaskQueue.front());
                mTaskQueue.pop();
            }
        }
        
        if (task.valid()) {
            {
                std::unique_lock<std::mutex> lock(readyLock);
                ++activeThreads;
            }
            
            try {
                task();
            } catch (const std::exception& e) {
                LOG_E("Task execution failed: %s", e.what());
            }
            
            {
                std::unique_lock<std::mutex> lock(readyLock);
                --activeThreads;
            }
            readyCondition.notify_all();
        }
    }
}

XLIB_END
