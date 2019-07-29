#ifndef SERVER_H
#define SERVER_H

#include "variables.h"
#include "game.h"
#include "portocol.h"

class GameServer
{
public:
    void InitServer();
    void MainLoop();

    void LoginCallback(bufferevent* p_bev, char* msg, int len);
    void OperationCallback(bufferevent* p_bev, char* msg, int len);
    void TimerCallback(); //定期循环

    void SendLoginSuccessToClient(bufferevent* p_bev); //告诉用户登录成功
    void SendLoginFailToClient(bufferevent* p_bev); //告诉用户登录失败
    void SendGameStartToClient(bufferevent* p_bev); //告诉用户游戏开始
    void SendGameDataToClient(bufferevent* p_bev, char* game_data, unsigned short len); //给用户发送游戏数据
private:
    GameMsg1Room GameMsg[MAX_ROOM_NUM];
    Portocol portocol;
    bufferevent* ConnTable[MAX_ROOM_NUM][4]; //记录每个房间中，每个玩家位置分别对应哪一条连接
    bool ActiveTable[MAX_ROOM_NUM][4]; //记录每个房间中，每个玩家位置是否已经被连接
};

extern GameServer GServer;

#endif
