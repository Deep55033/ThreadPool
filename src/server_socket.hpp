#ifndef SERVER_SCOKET_H
#define SERVER_SCOKET_H

#include "task.h"
#include <event.h>
#include <event2/bufferevent.h>
#include <iostream>

void client_read_cb(struct bufferevent *bev, void *ctx)
{
    static char buf[1024];
    static int read_len = 0;

    while ((read_len = bufferevent_read(bev, buf, 1024)) != 0)
    {
        buf[read_len] = '\0';
        std::cout << buf << std::endl;
    }
}

void client_write_cb(struct bufferevent *bev, void *ctx)
{

}

void client_event_cb(struct bufferevent *bev, short what, void *ctx)
{
    // 链接断开处理
}


class ServerSocket : public Task
{
public:

    ServerSocket(int fd)
    {
        this->fd_ = fd;
        this->bev = NULL;
    }

    void Init(event_base *base) override
    {
        evutil_make_socket_nonblocking(fd_);
        bev = bufferevent_socket_new(base, fd_, BEV_OPT_CLOSE_ON_FREE);
        if (bev == NULL)
        {
            std::cerr << "buffer event new error" << std::endl;
            return;
        }
        timeval read_timeval = {10, 0};
        bufferevent_set_timeouts(bev, &read_timeval, NULL);
        bufferevent_setcb(bev, client_read_cb, client_write_cb, client_event_cb, this);
        bufferevent_enable(bev, EV_READ);
    }

private:
    /// @brief 客户端的Socket FD
    int fd_;

    /// @brief buffer event 
    bufferevent* bev;
};

#endif //  SERVER_SCOKET_H