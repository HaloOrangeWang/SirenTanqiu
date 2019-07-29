#include "variables.h"
#include "portocol.h"
#include "server.h"

void Portocol::AcceptCallback(evconnlistener *p_listener, evutil_socket_t fd, sockaddr *p_sa, int sock_len, void* udata)
{
    //接受一个新连接
    //为这个新连接设置接收数据的回调函数
    event_base* p_base = evconnlistener_get_base(p_listener);
    bufferevent* p_bev = bufferevent_socket_new(p_base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(p_bev, GetDataCallback, NULL, SendDataCallback, NULL);
    bufferevent_enable(p_bev, EV_READ | EV_WRITE);
}

void Portocol::TimerCallback(evutil_socket_t fd, short events, void* udata)
{
    //timeval newtime;
    //evutil_gettimeofday(&newtime, NULL);
    //printf("timeout_cb called at %d.\n", (int)newtime.tv_sec);
    GServer.TimerCallback();
}

void Portocol::GetDataCallback(bufferevent* p_bev, void* udata)
{
    char buf[1000];
    char flag;
    unsigned short len;
    memset(buf, 0, sizeof(char) * 1000);

    //首先获取一段报文的长度
    bufferevent_read(p_bev, buf, 2);
    len = ((unsigned char)(buf[0]) << 8) + (unsigned char)(buf[1]);

    //然后读取长度个字符
    bufferevent_read(p_bev, buf, len);
    flag = buf[0];
    switch(flag)
    {
    case FLAG_LOGIN_ASK:
        GServer.LoginCallback(p_bev, &buf[1], len - 2);
        break;
    case OPERATE_ASK:
        GServer.OperationCallback(p_bev, &buf[1], len - 2);
        break;
    default:
        break;
    }
}

void Portocol::SendDataCallback(bufferevent *p_bev, short events, void *udata)
{
    if (events & BEV_EVENT_ERROR)
     {
         printf("Got an error on the connection.");
     }
     if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
     {
         printf("Connection closed.\n");
         bufferevent_free(p_bev);
     }
}

void Portocol::SendDataToClient(bufferevent* p_bev, char flag, char* msg, unsigned short len)
{
    //向客户端发送内容。报文内容为长度（长度包括类型和'\0'的长度）+类型+正文+'\0'
    char* buf = new char[len + 4];
    buf[0] = (len + 2) >> 8;
    buf[1] = (len + 2) & 0xff;
    buf[2] = flag;
    if (len != 0)
        memcpy(buf + 3, msg, len);
    buf[len + 3] = 0;

    struct evbuffer *output = bufferevent_get_output(p_bev);
    evbuffer_add(output, buf,  len + 4);
}

void Portocol::InitPortocol()
{
    //初始化libevent的event_base连接监听器
    static evconnlistener* p_listener;
    static sockaddr_in s_in;

    Base = event_base_new();
    if (!Base)
    {
        printf("Error: Could not initialize libevent!\n");
        getchar();
        abort();
    }

    memset(&s_in, 0, sizeof(s_in));
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(PORT);

    p_listener = evconnlistener_new_bind(Base, AcceptCallback, NULL, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1, (sockaddr*)&s_in, sizeof(s_in));

    //初始化一个定时器，并为这个定时器添加回调
    static event ev_time;
    event_assign(&ev_time, Base, -1, EV_PERSIST | EV_TIMEOUT, TimerCallback, (void*)&ev_time);
    event_add(&ev_time, &RefreshInterval);
}
