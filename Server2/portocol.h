#ifndef PORTOCOL_H
#define PORTOCOL_H

#include <stdio.h>
#include <stdlib.h>

#include "variables.h"

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event_struct.h>

class Portocol
{
public:
    void InitPortocol();  //libevent相关的初始化
    void MainLoop();
    static void AcceptCallback(evconnlistener *p_listener, evutil_socket_t fd, sockaddr *p_sa, int sock_len, void* udata); //接受新的连接
    static void GetDataCallback(bufferevent* p_bev, void* udata);  //获取客户端发来的消息
    static void SendDataCallback(bufferevent* p_bev, short events, void* udata);
    static void TimerCallback(evutil_socket_t fd, short events, void* udata); //定时器触发
    void SendDataToClient(bufferevent* p_bev, char flag, char* msg, unsigned short len); //向客户端发送消息
    event_base* Base;
};

#endif
