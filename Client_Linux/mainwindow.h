#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>

#include "variables.h"
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void GameInit();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;

    QTimer* GameTimer; //游戏定时器
    QGraphicsScene *scene_game;
    QImage ImgLeftPlayer;
    QImage ImgTopPlayer;
    QImage ImgRightPlayer;
    QImage ImgBottomPlayer;
    QImage ImgBall;

    Client client;
    int Period; //当前游戏阶段
    unsigned short RoomId = MAX_ROOM_NUM; //房间编号
    unsigned char Pos = 4; //位置（0左1上2右3下）
    short Score; //
    bool isEliminated; //是否已经被淘汰
    bool isPause; //游戏是否暂停
    bool LastTimePlayerAlive[4]; //上一个时刻，四个玩家是否已经出局

    void LoadImageBeforeGame(); //加载图片
    void InitGraphics(); //初始化主界面
    void GameStart(); //游戏开始时，要调整图形界面的一些显示
    void UpdateGameDataAndGraphics(GameData game_data); //更新游戏数据和界面
    void GameOver(GameData game_data); //游戏结束的处理

public slots:
    void Login(); //登录
    void OnGameTimerTriggered();
};

#endif // MAINWINDOW_H
