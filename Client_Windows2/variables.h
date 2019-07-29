#ifndef VARIABLES_H
#define VARIABLES_H

#include <math.h>
#include <stdio.h>
#include <string>
#include <string.h>

#define MOVING_LEFT_OR_TOP -1
#define MOVING_STOP 0
#define MOVING_RIGHT_OR_BOTTOM 1

#define MAX_ROOM_NUM 100 //最多可以同时存在几个房间
#define USEC 0.000001 //一微秒是多少秒

#define FLAG_LOGIN_ASK 1 //登录请求
#define OPERATE_ASK 2 //游戏内部操作请求
#define LOGIN_CALLBACK 11 //加入游戏的回调
#define LOGIN_FAIL_CALLBACK 12 //拒绝其加入游戏的回调
#define GAME_START_CALLBACK 13 //游戏开始的回调
#define GAME_MSG_CALLBACK 14 //更新游戏内容的回调

#define PERIOD_NOT_LOGGED_IN 0 //尚未完成登录的状态
#define PERIOD_LOGGING_IN 1 //尚未完成登录的状态
#define PERIOD_WAITING_GAME_START 2 //登录已成功，等待游戏开始
#define PERIOD_IN_GAME 3 //正在进行游戏的状态
#define PERIOD_ELIMINATED 4 //已经出局的状态
#define PERIOD_GAME_OVER 5 //游戏已经结束的状态

using namespace std;

extern const string HOST; //服务器端的IP地址
extern const unsigned short PORT; //监听哪个端口
extern const short WINDOW_X; //游戏界面的宽度
extern const short PLAT_LEN; //弹球平台的宽度

//extern timeval RefreshInterval; //游戏内容刷新周期

//extern event_base* Base;

struct GameData //一个房间的游戏信息
{
    bool game_alive; //这个房间的游戏是否正在进行
    bool game_pause; //这个房间的游戏是否因为有玩家刚刚出局而暂停
    bool left_player_alive; //四个玩家是否仍在游戏中
    bool right_player_alive;
    bool top_player_alive;
    bool bottom_player_alive;
    short left_player_pos; //四个玩家的位置
    short right_player_pos;
    short top_player_pos;
    short bottom_player_pos;
    short left_player_score; //四个玩家的得分
    short right_player_score;
    short top_player_score;
    short bottom_player_score;
    short ball_x;
    short ball_y;
};

#endif // VARIABLES_H
