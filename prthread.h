#ifndef PRTHREAD_H
#define PRTHREAD_H

#include <event2/event.h>
#include <queue>

class Task;

/// @brief 基于Libevent的线程
class PrThread
{
public:

    /// @brief 构造函数
    /// @param id 线程ID
    explicit PrThread(int id);

    /// @brief 清理资源
    ~PrThread();

    /// @brief 开启线程
    void Start();

    /// @brief 设置线程运行环境
    bool Setup();

    /// @brief // 添加任务
    /// @param task 需要执行的任务 
    /// @return 添加是否成功
    bool AddTask(Task *task);

    /// @brief 将添加的任务激活
    /// @return 成功激活
    bool Activate();

private:

    /// @brief libevent 上下文
    event_base *base_;

    /// @brief 任务队列
    std::queue<Task *> task_queue_;

    /// @brief 线程ID
    int id_;

    /// @brief 进程间通信
    int fd[2];

    /// @brief 线程回调函数
    void Main();

    friend void pipe_cb(evutil_socket_t sock, short event, void *arg);
};

#endif