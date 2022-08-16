//
// Created by yangfu on 2022/8/9.
//

#ifndef SKIPLIST_THREADPOOL_H
#define SKIPLIST_THREADPOOL_H
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <queue>
#include <assert.h>
using namespace std;

class ThreadPool {
public:
    explicit ThreadPool(int threadnums=10):_pool(make_shared<Pool>()){
        assert(threadnums>0);
        for(int i=0;i<threadnums;i++){
            thread([pool=_pool]{
                unique_lock<mutex> locker(pool->mut);
                while(true){
                    if(!pool->tasks.empty()){
                        auto task= move(pool->tasks.front());
                        pool->tasks.pop();
                        locker.unlock();
                        task();
                        locker.lock();
                    }
                    else if(pool->isclose){
                        break;
                    }
                    else{
                        pool->con.wait(locker);
                    }
                }
            }).detach();
        }
    }
    ThreadPool()=default;
    ThreadPool(ThreadPool&&)=default;
    ~ThreadPool(){
        if(static_cast<bool>(_pool)){
            lock_guard<mutex>   locker(_pool->mut);
            _pool->isclose=true;
        }
        _pool->con.notify_all();
    }

    template<class F>
    void Addtask(F&& task){
        lock_guard<mutex>   locker(_pool->mut);
        _pool->tasks.emplace(forward<F>(task));
        _pool->con.notify_one();
    }
private:
    struct Pool{
    queue<function<void()>> tasks;
    bool isclose;
    mutex mut;
    condition_variable con;
};
    shared_ptr<Pool> _pool;
};


#endif //SKIPLIST_THREADPOOL_H
