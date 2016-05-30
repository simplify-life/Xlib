//----------------------------------------------//
//  XThread.h
//  C++11_XLib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/5/26.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef XThread_h
#define XThread_h

#include <thread>
#include <atomic>
#include <list>
#include <functional>
#include <limits>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <map>

//////TaskQueue class
template <class T>
class XTaskQueue : private std::mutex
{
public:
    static const int wait_default = std::numeric_limits<int>::max();
    
    XTaskQueue(size_t capacity=0):mCapacity(capacity),exit_(false){};
    
    bool push(T&& task);
    T popWait(int waitTime=wait_default);
    bool popWait(T* v,int waitTime=wait_default);
    size_t capacity();
    void exit();
    inline bool isExited(){return exit_;};
private:
    XTaskQueue(const XTaskQueue&)=delete;
    XTaskQueue& operator=(const XTaskQueue&)=delete;
    
    void waitReady(std::unique_lock<std::mutex>&,int);
private:
    std::atomic<bool> exit_;
    size_t mCapacity;
    std::list<T> mTasks;
    std::condition_variable ready_;
};

//////////XThreadPool class
//using Task_void = std::map<std::function<void()>,bool>;
using Task_void = std::function<void()>;
extern template class XTaskQueue<Task_void>;

class XThreadPool
{
private:
    XThreadPool(const XThreadPool&)=delete;
    XThreadPool& operator=(const XThreadPool&)=delete;
public:
    XThreadPool(int threads,int taskCapacity=0,bool start=true);
    ~XThreadPool();
    XThreadPool& exit(){mTaskQueue.exit(); return *this;};
    void start();
    void join();
    void detach();
    bool addTask(Task_void&&);
    bool addTask(Task_void& task){ return addTask(Task_void(task));};
    size_t taskSize() {return mTaskQueue.capacity();};
private:
    XTaskQueue<Task_void> mTaskQueue;
    std::vector<std::thread> mThreads;
};

//////////////////////////////////////////
template <class T>
size_t XTaskQueue<T>::capacity()
{
    std::lock_guard<std::mutex> lk(*this);
    return mTasks.size();
}
template<class T>
void XTaskQueue<T>::exit()
{
    std::lock_guard<mutex> lk(*this);
    exit_=true;
    ready_.notify_all();
}
template<class T>
bool XTaskQueue<T>::push(T &&task)
{
    std::lock_guard<mutex> lk(*this);
    if(exit_||(mCapacity&&mTasks.size()>=mCapacity))
    {
        return false;
    }
    mTasks.push_back(std::move(task));
    ready_.notify_one();
    return true;
}
template<class T>
void XTaskQueue<T>::waitReady(std::unique_lock<std::mutex> & lk, int waitTime)
{
    if(exit_||!mTasks.empty()) return;
    if(wait_default==waitTime)
       ready_.wait(lk,[this]{return exit_||!mTasks.empty();});
    else if(0<waitTime)
    {
        auto tp=std::chrono::steady_clock::now()+std::chrono::milliseconds(waitTime);
        while (ready_.wait_until(lk, tp)!=std::cv_status::timeout&&mTasks.empty()&&!exit_)
        {
            
        }
    }
}
template<class T>
bool XTaskQueue<T>::popWait(T *v,int waitTime)
{
    std::unique_lock<mutex> lk(*this);
    waitReady(lk,waitTime);
    if(mTasks.empty()) return false;
    *v = std::move(mTasks.front());
    mTasks.pop_front();
    return true;
}

template<class T>
T XTaskQueue<T>::popWait(int waitTime)
{
    std::unique_lock<mutex> lk(*this);
    waitReady(lk,waitTime);
    if(mTasks.empty()) return T();
    T t = std::move(mTasks.front());
    mTasks.pop_front();
    return t;
}


#endif /* XThread_h */
