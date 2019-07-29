#ifndef VARIABLES_H
#define VARIABLES_H

#include <event2/event.h>
#include <math.h>
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

extern const unsigned short PORT; //监听哪个端口
extern const short PLAYER_SPEED; //玩家移动的速度（一秒钟可以移动几个单位）
extern const short WINDOW_X; //游戏界面的宽度
extern const short PLAT_LEN; //弹球平台的宽度
extern const short BALL_SPEED_X_AT_BEGINNING; //最开始的球速
extern const short BALL_SPEED_Y_AT_BEGINNING;

extern timeval RefreshInterval; //游戏内容刷新周期
extern const int RestartWaitCnt; //有一个玩家出局之后，等待多少轮循环之后恢复游戏

//extern event_base* Base;

#endif // VARIABLES_H
