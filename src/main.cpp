//
// Created by yjh on 23-4-28.
//

#include "threadpool.h"
#include "server_socket.hpp"
#include <event2/listener.h>

#ifdef _WIN32
#include <winsock2.h>
#endif

using namespace std;


void accept_cb(struct evconnlistener *evc, evutil_socket_t client_fd, struct sockaddr *client_addr, int socklen, void * arg)
{
    auto in = reinterpret_cast<sockaddr_in *>(client_addr); 
    char ip[INET_ADDRSTRLEN] = {0};
    evutil_inet_ntop(AF_INET, &in->sin_addr, ip, INET_ADDRSTRLEN);
    cout << "Client ip addr = " << ip << ", port = " << ntohs(in->sin_port) << endl;
    ServerSocket *server = new ServerSocket(client_fd);
    threadpool::instance()->Dispatch(server);
}

int main()
{
// WINDOWS 平台对套接字操作需要
#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        perror("WSAStartup");
        return 1;
    }
#endif

    // 初始化线程池
    auto pool_ptr = threadpool::instance();
    pool_ptr->init();

    sockaddr_in server_addr = {0};
    server_addr.sin_port = htons(9999);
    server_addr.sin_family = AF_INET;

    event_base * base = event_base_new();
    evconnlistener_new_bind(base, accept_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                                                10, (sockaddr *)&server_addr, sizeof(server_addr));

    event_base_dispatch(base);                  
    return 0;
}