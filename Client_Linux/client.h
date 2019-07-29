#ifndef CLIENT_H
#define CLIENT_H

#include "portocol.h"

class Client
{
public:
    void InitClient();
    int GetLoginStatus(); //在登录尚未完成的时候，需要先从服务器查看登录状态
    int GetGameStartStatus(); //在登录成功，等待其他玩家连入的时候，需要从服务器查看游戏是否已经开始
    int GetGameData(GameData* game_data); //游戏开始之后，从服务器获取游戏数据
    void SendLogin(unsigned short room_id, unsigned char pos); //发送登录请求
    void SendOperation(unsigned short room_id, unsigned char pos, char operation); //发送游戏操作请求
private:
    void UnpackGameData(GameData* game_data, char* msg); //解包当前的游戏数据
    Portocol portocol;
};

#endif
