#include "portocol.h"

//#include <sys/socket.h>
//#include <arpa/inet.h>
//#include <fcntl.h>
//#include <unistd.h>

#include <QDebug>

void Portocol::SetNoBlocking()
{
    //int opts = fcntl(socket_fd, F_GETFL);
    //fcntl(socket_fd, F_SETFL, opts | O_NONBLOCK);
    ULONG noblock = 1;            //非阻塞参数
    ioctlsocket(sock ,FIONBIO, &noblock);
}

void Portocol::InitPortocol()
{
    connect_status = false;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 0);
    err = WSAStartup(wVersionRequested, &wsaData);

    //初始化一个socket, 服务器地址等
    /*socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        qDebug() << ("Error: Could not initialize socket!\n");
        getchar();
        abort();
    }*/
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        qDebug() << ("Error: Could not initialize socket!\n");
        getchar();
        abort();
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    //server_addr.sin_addr.s_addr = inet_addr(HOST.c_str());
    server_addr.sin_addr.S_un.S_addr = inet_addr(HOST.c_str());

    //连接服务器
    //while (connect(socket_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    while (connect(sock, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) != 0)
    {
        qDebug() << ("连接至服务器失败，准备重新连接.\n");
        //sleep(1);
        _sleep(1000);
    }

    //设置为非阻塞
    SetNoBlocking();
    connect_status = true;
}

int Portocol::GetDataFromServer(char* flag, char* data)
{
    //char* recv_msg = new char[];
    if (!connect_status)
    {
        qDebug() << ("还没有连接到服务器，暂不能接收数据.\n");
        return -1; //服务器还没有连接成功，当然不能接收数据
    }
    unsigned short data_len;
    int read_len;
    memset(recv_buf, 0, sizeof(char) * 1000);

    //首先获取一段报文的长度
    //read_len = read(socket_fd, recv_buf, 2);
    read_len = recv(sock, recv_buf, 2, 0);
    if (read_len == 2)
        data_len = ((unsigned char)(recv_buf[0]) << 8) + (unsigned char)(recv_buf[1]);
    else
        return -1;
    //然后读取长度个字符
    //read_len = read(socket_fd, recv_buf, data_len);
    read_len = recv(sock, recv_buf, data_len, 0);
    if (read_len == data_len)
    {
        *flag = recv_buf[0];
        if (data_len >= 3)
            memcpy(data, &recv_buf[1], data_len - 2);
        return (read_len - 2); //返回真实的数据长度
    }else{
        return -1;
    }
}

void Portocol::SendDataToServer(char flag, char *msg, unsigned short len)
{
    // flag: 数据类型； msg: 正文； len: 长度
    if (!connect_status)
    {
        qDebug() << ("还没有连接到服务器，暂不能发送数据.\n");
        return; //服务器还没有连接成功，当然不能发送数据
    }
    //向服务器发送内容。报文内容为长度（长度包括类型和'\0'的长度）+类型+正文+'\0'
    char* buf = new char[len + 4];
    buf[0] = (len + 2) >> 8;
    buf[1] = (len + 2) & 0xff;
    buf[2] = flag;
    if (len != 0)
        memcpy(buf + 3, msg, len);
    buf[len + 3] = 0;

    //int n = send(socket_fd, buf, len + 4, 0);
    int n = send(sock, buf, len + 4, 0);
    if (n < 0)
        qDebug() << ("数据发送失败.\n");
}
