#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>

class PrThread;
class Task;

class ThreadPool
{
public:
    static ThreadPool * instance();

    bool init(int thread_size = 10);


    /// @brief 任务调度
    /// @param task 任务
    bool Dispatch(Task *task);
private:    

    int thread_size_;

    std::vector<PrThread *> vec_thread_;

    explicit ThreadPool();

    ~ThreadPool();
};

#endif