#ifndef GAME_H
#define GAME_H

#include "variables.h"

struct PlayerOperation1Room
{
    char left_player_stat = MOVING_STOP;
    char top_player_stat = MOVING_STOP;
    char right_player_stat = MOVING_STOP;
    char bottom_player_stat = MOVING_STOP;
};

class GameMsg1Room //一个房间的游戏信息
{
public:
    void game_start();
    void game_restart(); //因玩家出局而重新开始游戏
    void update_player_pos_spd(); //更新四个玩家的位置和速度
    void update_game_data(double time_left); //更新游戏数据. 参数为距离刷新剩余的时间
    unsigned char* packet_game_data(); //打包游戏实时数据

    bool game_alive = false; //这个房间的游戏是否正在进行
    bool game_pause = false; //这个房间的游戏是否因为有玩家刚刚出局而暂停
    int pause_cnt = 0; //游戏暂停的计数
    int room_id;
    PlayerOperation1Room player_operation; //玩家当前正在执行什么操作
private:
    bool left_player_alive = false; //四个玩家是否仍在游戏中
    bool right_player_alive = false;
    bool top_player_alive = false;
    bool bottom_player_alive = false;
    short players_in_game = 0;
    short left_player_pos = 0; //四个玩家的位置
    short right_player_pos = 0;
    short top_player_pos = 0;
    short bottom_player_pos = 0;
    short left_player_spd = 0; //四个玩家的速度
    short right_player_spd = 0;
    short top_player_spd = 0;
    short bottom_player_spd = 0;
    short left_player_score = 0; //四个玩家的得分
    short right_player_score = 0;
    short top_player_score = 0;
    short bottom_player_score = 0;
    short ball_x = 0;
    short ball_y = 0;
    short ball_speed_x = 0;
    short ball_speed_y = 0;
};

#endif
