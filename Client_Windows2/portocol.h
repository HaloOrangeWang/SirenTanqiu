#ifndef PORTOCOL_H
#define PORTOCOL_H

#include "variables.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")

class Portocol
{
public:
    void InitPortocol();  //TCP相关的初始化
    int GetDataFromServer(char* flag, char* data);  //获取服务器发来的消息
    void SendDataToServer(char flag, char* msg, unsigned short len); //向服务器发送消息
private:
    void SetNoBlocking(); //设置TCP连接方式为非阻塞
    bool connect_status; //是否已经连接成功
    int read_cnt; //已经从接收缓冲区中读取的字节数量
    char recv_buf[1000]; //

    //int socket_fd; //socket的文件id
    SOCKET sock;
};

#endif
