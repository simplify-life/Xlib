//----------------------------------------------//
//  XThread.cpp
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//    Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//
#include "XThread.h"
#include <assert.h>
#include <utility>

#include "macro/XDefine.h"
using namespace std;

XLIB_BEGAIN

template class XTaskQueue<Task_void>;

XThreadPool::XThreadPool(int threads,int maxWaiting,bool start):
mTaskQueue(maxWaiting),mThreads(threads)
{
    if(start)
    {
        this->start();
    }
}

XThreadPool::~XThreadPool()
{
    mTaskQueue.exit();
    assert(mTaskQueue.isExited());
    if(mTaskQueue.capacity())
    {
        LOG_E("%lu tasks not processed when thread pool exited\n",mTaskQueue.capacity());
    }
}

void XThreadPool::start()
{
    for(auto &th: mThreads)
    {
        thread t([this]
        {
            while (!mTaskQueue.isExited())
            {
                Task_void task;
                if(mTaskQueue.popWait(&task))
                {
                    task();
                }
            }
        });
        th.swap(t);
    }
}

void XThreadPool::join()
{
    for(auto &e:mThreads)
        e.join();
}

void XThreadPool::detach()
{
    for(auto &e:mThreads)
        e.detach();
}

bool XThreadPool::addTask(Task_void && task)
{
    return mTaskQueue.push(move(task));
}

XLIB_END
