#include "game.h"


void GameMsg1Room::game_start()
{
    //游戏开始 先初始化相关参数
    game_alive = true; //这个房间的游戏是否正在进行
    game_pause = false;
    pause_cnt = 0;
    left_player_alive = true; //四个玩家是否仍在游戏中
    right_player_alive = true;
    top_player_alive = true;
    bottom_player_alive = true;
    players_in_game = 4;
    left_player_pos = (WINDOW_X - PLAT_LEN) / 2; //四个玩家的位置
    right_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    top_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    bottom_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    left_player_score = 0; //四个玩家的得分
    right_player_score = 0;
    top_player_score = 0;
    bottom_player_score = 0;
    ball_x = WINDOW_X / 2;
    ball_y = WINDOW_X / 2;
    ball_speed_x = BALL_SPEED_X_AT_BEGINNING;
    ball_speed_y = BALL_SPEED_Y_AT_BEGINNING;
}

void GameMsg1Room::game_restart()
{
    game_pause = false;
    pause_cnt = 0;
    left_player_pos = (WINDOW_X - PLAT_LEN) / 2; //四个玩家的位置
    right_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    top_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    bottom_player_pos = (WINDOW_X - PLAT_LEN) / 2;
    ball_x = WINDOW_X / 2;
    ball_y = WINDOW_X / 2;
    // 如果上下两方玩家都已经出局，则向下发球的开局会造成死局，改为向右发球
    if (top_player_alive == false && bottom_player_alive == false){
        ball_speed_x = BALL_SPEED_Y_AT_BEGINNING;
        ball_speed_y = BALL_SPEED_X_AT_BEGINNING;
    }else {
        ball_speed_x = BALL_SPEED_X_AT_BEGINNING;
        ball_speed_y = BALL_SPEED_Y_AT_BEGINNING;
    }
}

void GameMsg1Room::update_player_pos_spd()
{
    switch(player_operation.top_player_stat)
    {
    case MOVING_LEFT_OR_TOP:
        top_player_pos -= PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        top_player_spd = -PLAYER_SPEED;
        if (top_player_pos < 0)
            top_player_pos = 0;
        break;
    case MOVING_RIGHT_OR_BOTTOM:
        top_player_pos += PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        top_player_spd = PLAYER_SPEED;
        if (top_player_pos > WINDOW_X - PLAT_LEN)
            top_player_pos = WINDOW_X - PLAT_LEN;
        break;
    default:
        top_player_spd = 0;
        break;
    }

    switch(player_operation.left_player_stat)
    {
    case MOVING_LEFT_OR_TOP:
        left_player_pos -= PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        left_player_spd = -PLAYER_SPEED;
        if (left_player_pos < 0)
            left_player_pos = 0;
        break;
    case MOVING_RIGHT_OR_BOTTOM:
        left_player_pos += PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        left_player_spd = PLAYER_SPEED;
        if (left_player_pos > WINDOW_X - PLAT_LEN)
            left_player_pos = WINDOW_X - PLAT_LEN;
        break;
    default:
        left_player_spd = 0;
        break;
    }

    switch(player_operation.right_player_stat)
    {
    case MOVING_LEFT_OR_TOP:
        right_player_pos -= PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        right_player_spd = -PLAYER_SPEED;
        if (right_player_pos < 0)
            right_player_pos = 0;
        break;
    case MOVING_RIGHT_OR_BOTTOM:
        right_player_pos += PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        right_player_spd = PLAYER_SPEED;
        if (right_player_pos > WINDOW_X - PLAT_LEN)
            right_player_pos = WINDOW_X - PLAT_LEN;
        break;
    default:
        right_player_spd = 0;
        break;
    }

    switch(player_operation.bottom_player_stat)
    {
    case MOVING_LEFT_OR_TOP:
        bottom_player_pos -= PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        bottom_player_spd = -PLAYER_SPEED;
        if (bottom_player_pos < 0)
            bottom_player_pos = 0;
        break;
    case MOVING_RIGHT_OR_BOTTOM:
        bottom_player_pos += PLAYER_SPEED * (RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);
        bottom_player_spd = PLAYER_SPEED;
        if (bottom_player_pos > WINDOW_X - PLAT_LEN)
            bottom_player_pos = WINDOW_X - PLAT_LEN;
        break;
    default:
        bottom_player_spd = 0;
        break;
    }
}

void GameMsg1Room::update_game_data(double time_left)
{
    //printf("top player alive = %d, top player score = %d, bottom player alive = %d, bottom player score = %d\n", top_player_alive, top_player_score, bottom_player_alive, bottom_player_score);
    //首先确定球撞上四周墙壁剩余的时间(数组四项分别为左上右下)
    double col_time[4] = {time_left + 1, time_left + 1, time_left + 1, time_left + 1};
    if (ball_speed_x < 0)
        col_time[0] = ball_x * 1.0 / (-ball_speed_x);
    else if (ball_speed_x > 0)
        col_time[2] = (WINDOW_X - ball_x) * 1.0 / ball_speed_x;
    if (ball_speed_y < 0)
        col_time[1] = ball_y * 1.0  / (-ball_speed_y);
    else if (ball_speed_y > 0)
        col_time[3] = (WINDOW_X - ball_y) * 1.0 / ball_speed_y;
    // 根据撞上四周墙壁剩余的时间，确定球将最先撞上哪个墙壁
    int col_dx = 0;
    double col_time_1st = col_time[0];
    for (int t = 1; t <= 3; t++)
    {
        if (col_time[t] < col_time_1st)
        {
            col_time_1st = col_time[t];
            col_dx = t;
        }
    }
    if (col_time_1st > time_left)
    {
        // 如果在刷新时间内没有撞墙，则只需更新球的位置即可
        ball_x += ball_speed_x * time_left;
        ball_y += ball_speed_y * time_left;
    }
    else {
        // 首先更新撞墙时刻球的位置
        ball_x += ball_speed_x * col_time_1st;
        ball_y += ball_speed_y * col_time_1st;
        ball_x = (ball_x > 0) ? ball_x : 0;
        ball_x = (ball_x < WINDOW_X) ? ball_x : WINDOW_X;
        ball_y = (ball_y > 0) ? ball_y : 0;
        ball_y = (ball_y < WINDOW_X) ? ball_y : WINDOW_X;

        // 然后判断撞墙之后的动作
        switch (col_dx)
        {
        case 0:
            if (left_player_alive == false){
                ball_speed_x = -ball_speed_x;
                update_game_data(time_left - col_time_1st);
            }else{
                if (left_player_pos >= ball_y - PLAT_LEN && left_player_pos <= ball_y){
                    //玩家接住了球
                    ball_speed_x = -ball_speed_x;
                    ball_speed_y = (ball_speed_y + left_player_spd) / 2;
                    left_player_score += 1;
                    update_game_data(time_left - col_time_1st);
                }else{
                    //玩家没有接住球，则该玩家出局，游戏暂停
                    game_pause = true;
                    left_player_alive = false;
                    players_in_game -= 1;
                    if (players_in_game <= 1) //如果场上只剩下最后一个玩家，则游戏结束
                        game_alive = false;
                }
            }
            break;
        case 1:
            if (top_player_alive == false){
                ball_speed_y = -ball_speed_y;
                update_game_data(time_left - col_time_1st);
            }else{
                if (top_player_pos >= ball_x - PLAT_LEN && top_player_pos <= ball_x){
                    //玩家接住了球
                    ball_speed_y = -ball_speed_y;
                    ball_speed_x = (ball_speed_x + top_player_spd) / 2;
                    top_player_score += 1;
                    update_game_data(time_left - col_time_1st);
                }else{
                    //玩家没有接住球，则该玩家出局，游戏暂停
                    game_pause = true;
                    top_player_alive = false;
                    players_in_game -= 1;
                    if (players_in_game <= 1) //如果场上只剩下最后一个玩家，则游戏结束
                        game_alive = false;
                }
            }
            break;
        case 2:
            if (right_player_alive == false){
                ball_speed_x = -ball_speed_x;
                update_game_data(time_left - col_time_1st);
            }else{
                if (right_player_pos >= ball_y - PLAT_LEN && right_player_pos <= ball_y){
                    //玩家接住了球
                    ball_speed_x = -ball_speed_x;
                    ball_speed_y = (ball_speed_y + right_player_spd) / 2;
                    right_player_score += 1;
                    update_game_data(time_left - col_time_1st);
                }else{
                    //玩家没有接住球，则该玩家出局，游戏暂停
                    game_pause = true;
                    right_player_alive = false;
                    players_in_game -= 1;
                    if (players_in_game <= 1) //如果场上只剩下最后一个玩家，则游戏结束
                        game_alive = false;
                    else
                        printf("房间号%d, 因有玩家出局，游戏需要暂停\n", room_id);
                }
            }
            break;
        case 3:
            if (bottom_player_alive == false){
                ball_speed_y = -ball_speed_y;
                update_game_data(time_left - col_time_1st);
            }else{
                if (bottom_player_pos >= ball_x - PLAT_LEN && right_player_pos <= ball_x){
                    //玩家接住了球
                    ball_speed_y = -ball_speed_y;
                    ball_speed_x = (ball_speed_x + bottom_player_spd) / 2;
                    bottom_player_score += 1;
                    update_game_data(time_left - col_time_1st);
                }else{
                    //玩家没有接住球，则该玩家出局，游戏暂停
                    game_pause = true;
                    bottom_player_alive = false;
                    players_in_game -= 1;
                    if (players_in_game <= 1) //如果场上只剩下最后一个玩家，则游戏结束
                        game_alive = false;
                }
            }
            break;
        }
    }
}

unsigned char* GameMsg1Room::packet_game_data()
{
    //需要发送给客户端的内容包括：游戏状态（是否正在运行，是否暂停），玩家状态（顺序为左上右下），玩家分数，玩家位置，球的位置
    unsigned char* msg = new unsigned char[26];
    memset(msg, 0, sizeof(unsigned char) * 26);
    msg[0] = game_alive;
    msg[1] = game_pause;
    msg[2] = left_player_alive;
    msg[3] = top_player_alive;
    msg[4] = right_player_alive;
    msg[5] = bottom_player_alive;
    msg[6] = left_player_score >> 8;
    msg[7] = left_player_score & 0xff;
    msg[8] = top_player_score >> 8;
    msg[9] = top_player_score & 0xff;
    msg[10] = right_player_score >> 8;
    msg[11] = right_player_score & 0xff;
    msg[12] = bottom_player_score >> 8;
    msg[13] = bottom_player_score & 0xff;
    msg[14] = left_player_pos >> 8;
    msg[15] = left_player_pos & 0xff;
    msg[16] = top_player_pos >> 8;
    msg[17] = top_player_pos & 0xff;
    msg[18] = right_player_pos >> 8;
    msg[19] = right_player_pos & 0xff;
    msg[20] = bottom_player_pos >> 8;
    msg[21] = bottom_player_pos & 0xff;
    msg[22] = ball_x >> 8;
    msg[23] = ball_x & 0xff;
    msg[24] = ball_y >> 8;
    msg[25] = ball_y & 0xff;
    return msg;
}
