#include "variables.h"

const unsigned short PORT = 28125;
const short PLAYER_SPEED = 200;
const short WINDOW_X = 400; //游戏界面的宽度
const short PLAT_LEN = 80; //弹球平台的宽度
const short BALL_SPEED_X_AT_BEGINNING = 0;
const short BALL_SPEED_Y_AT_BEGINNING = 120;

timeval RefreshInterval = {0, 100000};
const int RestartWaitCnt = 10;
