#include "server.h"

GameServer GServer;

void GameServer::InitServer()
{
    for (int rid = 0; rid <= MAX_ROOM_NUM - 1; rid++)
    {
        GameMsg[rid].room_id = rid;
        for (int pos = 0; pos <= 3; pos++)
        {
            ActiveTable[rid][pos] = false;
            ConnTable[rid][pos] = nullptr;
        }
    }
    portocol.InitPortocol();
}

void GameServer::LoginCallback(bufferevent* p_bev, char *msg, int len)
{
    //登录回调信息报文的格式是：房间号+位置
    if (len == 3)
    {
        unsigned short room_id = ((unsigned char)(msg[0]) << 8) + (unsigned char)msg[1];
        unsigned char pos = (unsigned char)msg[2];
        if (room_id >= MAX_ROOM_NUM || pos >= 4)
        {
            //请求加入游戏的信息不正确，拒绝其加入游戏
            printf("房间号%d, 位置%d, 登录被拒绝\n", room_id, pos);
            SendLoginFailToClient(p_bev);
        }else if (ActiveTable[room_id][pos] == true){
            //这个位置已经有玩家了。拒绝新玩家的连入
            printf("房间号%d, 位置%d, 登录被拒绝\n", room_id, pos);
            SendLoginFailToClient(p_bev);
        }else{
            //成功连入游戏
            printf("房间号%d, 位置%d, 登录成功\n", room_id, pos);
            ConnTable[room_id][pos] = p_bev;
            ActiveTable[room_id][pos] = true;
            SendLoginSuccessToClient(p_bev);
        }
    }
}

void GameServer::OperationCallback(bufferevent* p_bev, char* msg, int len)
{
    //操作回调信息报文的格式是：房间号+位置+操作内容
    if (len == 4)
    {
        unsigned short room_id = ((unsigned char)(msg[0]) << 8) + (unsigned char)msg[1];
        unsigned char pos = (unsigned char)msg[2];
        char operation = msg[3];
        if (operation == MOVING_LEFT_OR_TOP || operation == MOVING_STOP || operation == MOVING_RIGHT_OR_BOTTOM)
        {
            printf("房间号%d, 位置%d, 玩家执行了操作%d\n", room_id, pos, operation);
            switch(pos)
            {
            case 0:
                GameMsg[room_id].player_operation.left_player_stat = operation;
                break;
            case 1:
                GameMsg[room_id].player_operation.top_player_stat = operation;
                break;
            case 2:
                GameMsg[room_id].player_operation.right_player_stat = operation;
                break;
            case 3:
                GameMsg[room_id].player_operation.bottom_player_stat = operation;
                break;
            default:
                break;
            }
        }
    }
}

void GameServer::TimerCallback()
{
    //遍历所有房间，进行相应操作
    for (int rid = 0; rid <= MAX_ROOM_NUM - 1; rid++)
    {
        //如果一个房间已经人满，但游戏仍未开始，则立即开始此房间的游戏
        if (GameMsg[rid].game_alive == false){
            if (ActiveTable[rid][0] == true && ActiveTable[rid][1] == true && ActiveTable[rid][2] == true && ActiveTable[rid][3] == true){
                printf("房间号%d, 游戏开始\n", rid);
                GameMsg[rid].game_start();
                for (int pos = 0; pos <= 3; pos++)
                    SendGameStartToClient(ConnTable[rid][pos]);
            }
        }
        //如果一个房间正在运行游戏，则更新游戏内容
        else if (GameMsg[rid].game_alive == true && GameMsg[rid].game_pause == false)
        {
            GameMsg[rid].update_player_pos_spd();
            GameMsg[rid].update_game_data(RefreshInterval.tv_sec + RefreshInterval.tv_usec * USEC);

            char game_data_to_send[26];
            memcpy(game_data_to_send, GameMsg[rid].packet_game_data(), 26);
            for (int pos = 0; pos <= 3; pos++)
                SendGameDataToClient(ConnTable[rid][pos], game_data_to_send, 26);

            if (GameMsg[rid].game_alive == false)
            {
                //一种特殊的情况：执行完这一步之后，游戏就应当结束了 --- t掉这个房间的所有连接
                printf("房间号%d, 游戏结束\n", rid);
                for (int pos = 0; pos <= 3; pos++)
                {
                    ActiveTable[rid][pos] = false;
                    ConnTable[rid][pos] = nullptr;
                }
            }
        }
        //如果一个房间的游戏正在暂停中，则判断是否恢复
        else if (GameMsg[rid].game_alive == true && GameMsg[rid].game_pause == true)
        {
            if (GameMsg[rid].pause_cnt <= RestartWaitCnt - 1)
                GameMsg[rid].pause_cnt += 1;
            else{
                GameMsg[rid].pause_cnt = 0;
                GameMsg[rid].game_restart();

                char game_data_to_send[26];
                memcpy(game_data_to_send, GameMsg[rid].packet_game_data(), 26);
                printf("房间号%d, 游戏恢复\n", rid);
                for (int pos = 0; pos <= 3; pos++)
                    SendGameDataToClient(ConnTable[rid][pos], game_data_to_send, 26);
            }
        }
    }
}

void GameServer::SendLoginSuccessToClient(bufferevent *p_bev)
{
    portocol.SendDataToClient(p_bev, LOGIN_CALLBACK, nullptr, 0);
}

void GameServer::SendLoginFailToClient(bufferevent *p_bev)
{
    portocol.SendDataToClient(p_bev, LOGIN_FAIL_CALLBACK,nullptr, 0);
}

void GameServer::SendGameStartToClient(bufferevent *p_bev)
{
    portocol.SendDataToClient(p_bev, GAME_START_CALLBACK, nullptr, 0);
}

void GameServer::SendGameDataToClient(bufferevent *p_bev, char *game_data, unsigned short len)
{
    portocol.SendDataToClient(p_bev, GAME_MSG_CALLBACK, game_data, len);
}

void GameServer::MainLoop()
{
    event_base_dispatch(portocol.Base); //主循环
}
