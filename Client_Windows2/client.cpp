#include "client.h"

#include <QDebug>

void Client::InitClient()
{
    portocol.InitPortocol();
}

int Client::GetLoginStatus()
{
    //在登录未完成的阶段，检查登录状态
    //返回值：-1为登录失败，0为登录未完成，1为登录成功
    char flag = 0;
    char data[1000];
    memset(data, 0, sizeof(char) * 1000);
    int data_len = portocol.GetDataFromServer(&flag, data);
    if (data_len == -1)
        return 0; //接收不到数据，说明登录尚未完成
    switch(flag)
    {
    case LOGIN_CALLBACK:
        qDebug() << ("登录成功.\n");
        return 1;
    case LOGIN_FAIL_CALLBACK:
        qDebug() << ("登录失败.\n");
        return -1;
    default:
        return 0;
    }
}

int Client::GetGameStartStatus()
{
    //在登录成功之后，检查游戏开始的状态
    //返回值：1为游戏已开始，0为游戏尚未开始
    char flag = 0;
    char data[1000];
    memset(data, 0, sizeof(char) * 1000);
    int data_len = portocol.GetDataFromServer(&flag, data);
    if (data_len == -1)
        return 0; //接收不到数据，说明游戏尚未开始
    switch(flag)
    {
    case GAME_START_CALLBACK:
        qDebug() << ("游戏已开始.\n");
        return 1;
    default:
        return 0;
    }
}

int Client::GetGameData(GameData* game_data)
{
    //游戏开始之后，实时获取当前的游戏数据
    //返回值：1为游戏数据有更新，0为游戏数据未更新
    char flag = 0;
    char data[1000];
    memset(data, 0, sizeof(char) * 1000);
    int data_len = portocol.GetDataFromServer(&flag, data);
    if (data_len == -1)
        return 0; //接收不到数据，说明游戏数据没有更新
    if (flag != GAME_MSG_CALLBACK)
        return 0; //数据类型不正确，说明游戏数据没有更新
    UnpackGameData(game_data, data); //解包游戏数据
    return 1;
}

void Client::UnpackGameData(GameData* game_data, char* msg)
{
    //从Char数组（msg）中还原出游戏数据（game_data）
    game_data->game_alive = msg[0]; //游戏是否在运行
    game_data->game_pause = msg[1]; //游戏是否暂停
    game_data->left_player_alive = msg[2]; //四个玩家状态
    game_data->top_player_alive = msg[3];
    game_data->right_player_alive = msg[4];
    game_data->bottom_player_alive = msg[5];
    game_data->left_player_score = ((unsigned char)(msg[6]) << 8) + (unsigned char)(msg[7]); //四个玩家分数
    game_data->top_player_score = ((unsigned char)(msg[8]) << 8) + (unsigned char)(msg[9]);
    game_data->right_player_score = ((unsigned char)(msg[10]) << 8) + (unsigned char)(msg[11]);
    game_data->bottom_player_score = ((unsigned char)(msg[12]) << 8) + (unsigned char)(msg[13]);
    game_data->left_player_pos = ((unsigned char)(msg[14]) << 8) + (unsigned char)(msg[15]); //四个玩家位置
    game_data->top_player_pos = ((unsigned char)(msg[16]) << 8) + (unsigned char)(msg[17]);
    game_data->right_player_pos = ((unsigned char)(msg[18]) << 8) + (unsigned char)(msg[19]);
    game_data->bottom_player_pos = ((unsigned char)(msg[20]) << 8) + (unsigned char)(msg[21]);
    game_data->ball_x = ((unsigned char)(msg[22]) << 8) + (unsigned char)(msg[23]); //球的位置
    game_data->ball_y = ((unsigned char)(msg[24]) << 8) + (unsigned char)(msg[25]);
}

void Client::SendLogin(unsigned short room_id, unsigned char pos)
{
    //发送登录请求
    //首先组包
    char msg[3];
    msg[0] = room_id >> 8;
    msg[1] = room_id & 0xff;
    msg[2] = pos;
    //然后将登录请求发出去
    portocol.SendDataToServer(FLAG_LOGIN_ASK, msg, 3);
}

void Client::SendOperation(unsigned short room_id, unsigned char pos, char operation)
{
    //发送游戏操作信息
    //首先组包
    char msg[4];
    msg[0] = room_id >> 8;
    msg[1] = room_id & 0xff;
    msg[2] = pos;
    msg[3] = operation;
    //然后将操作请求发出去
    portocol.SendDataToServer(OPERATE_ASK, msg, 4);
}
