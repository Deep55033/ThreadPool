#include "prthread.h"
#include <thread>
#include <iostream>
#include "task.h"

#ifndef _WIN32 
#include <unistd.h>
#endif

using namespace std;

void pipe_cb(evutil_socket_t sock, short event, void *arg)
{ 
    if (event & EV_READ)
    {
        static char buf[2] = {0};
        int read_len = read(sock, buf, 2);
        PrThread *pr = (PrThread *) arg;
        if (pr->task_queue_.empty()) return;
        // 从队列中取出任务
        Task *task = pr->task_queue_.front();
        pr->task_queue_.pop();
        task->Init(pr->base_);
    }
}

PrThread::PrThread(int id) 
{
    this->id_ = id;
}

PrThread::~PrThread()
{

}
void PrThread::Start() 
{
    // 创建一个线程
    thread th(&PrThread::Main, this);
    // 与当前线程断开, 该线程在task_queue_后台独立运行
    th.detach();
}

bool PrThread::Setup()
{
    // 创建上下文
    base_ = event_base_new();
    if (!base_)
    {
        cerr << "Event base new error" << endl;
        return false;
    }

#ifdef _WIN32 // Windows

#else // Linux
    if (pipe(fd) == -1)
    {
        cerr << "Create pipe error!" << endl;
        return false;
    }
#endif

    event *ev = event_new(base_, fd[0], EV_READ | EV_PERSIST, pipe_cb, this);
    event_add(ev, 0);
    return true;
}

bool PrThread::AddTask(Task *task)
{
    if (task == nullptr) return false;
    task_queue_.push(task);
    return true;
}

bool PrThread::Activate()
{
#ifdef _WIN32
#else
    if (write(fd[1], "a", 1) == -1) 
        return false;
    return true;
#endif
    return false;
}

void PrThread::Main() 
{
    cout << "PrThread begin id = " << this->id_ << endl;
    if (!base_) 
    {   
        cerr << "Thread is not init!" << endl;
        return;
    }
    // 开启调度
    cout << event_base_dispatch(base_) << endl;

    cout << "PrThread end id = " << this->id_ << endl;
}


