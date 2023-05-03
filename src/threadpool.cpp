#include "ThreadPool.h"
#include "prthread.h"
#include <thread>

using namespace std;

ThreadPool::ThreadPool() {
    this->thread_size_ = 0;
}

ThreadPool::~ThreadPool() {
    for (auto &i: this->vec_thread_) {
        delete i;
    }
}

bool ThreadPool::init(int thread_size) {
    thread_size_ = thread_size;
    for (int i = 0; i < thread_size_; i++) {
        // 初始化所有线程
        auto th = new PrThread(i + 1);
        th->Setup();
        th->Start();
        vec_thread_.push_back(th);
        // 等待10ms防止中断打印异常
        this_thread::sleep_for(10ms);
    }
    return true;
}

bool ThreadPool::Dispatch(Task *task)
{
    static int cur_thread_index = 0;
    if (task == NULL) return false;
    PrThread * th = vec_thread_[cur_thread_index % thread_size_];
    th->AddTask(task);
    th->Activate();
    cur_thread_index++;    
    return true;
}

ThreadPool * ThreadPool::instance() {
    static ThreadPool thread_pool;
    return &thread_pool;
}




