#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::GameInit()
{
    LoadImageBeforeGame();
    InitGraphics();

    client.InitClient(); //初始化TCP相关内容，并连接服务器
    Period = PERIOD_NOT_LOGGED_IN; //最开始时，为尚未完成登录的状态

    //初始化定时器。用于查询TCP返回信息
    GameTimer = new QTimer(this);
    connect(GameTimer, SIGNAL(timeout()), this, SLOT(OnGameTimerTriggered()));
    GameTimer->start(50);
}

void MainWindow::LoadImageBeforeGame()
{
    ImgLeftPlayer = QImage(":/Imgs/left.png");
    ImgTopPlayer = QImage(":/Imgs/top.png");
    ImgRightPlayer = QImage(":/Imgs/right.png");
    ImgBottomPlayer = QImage(":/Imgs/bottom.png");
    ImgBall = QImage(":/Imgs/ball.png");
}

void MainWindow::InitGraphics()
{
    scene_game = new QGraphicsScene;

    //窗口大小为固定值，不可以更改
    this->setFixedSize(550, 472);
    this->setFocusPolicy(Qt::StrongFocus);

    //初始化菜单
    ui->comboBox->addItem(QString::fromStdWString(L"左"));
    ui->comboBox->addItem(QString::fromStdWString(L"上"));
    ui->comboBox->addItem(QString::fromStdWString(L"右"));
    ui->comboBox->addItem(QString::fromStdWString(L"下"));

    //绑定按钮事件
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Login()));

    //graphicsView对齐方式靠左靠上
    ui->graphicsView_2->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView_2->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::GameStart()
{
    ui->label_12->hide();
    for (int t = 0; t <= 3; t++) //用于记录上一个时刻，四个玩家是否已经出局
        LastTimePlayerAlive[t] = true;
}

void MainWindow::UpdateGameDataAndGraphics(GameData game_data)
{
    scene_game->clear(); 
    scene_game->setSceneRect(0, 0, 420, 420);
    qDebug() << "Ball pos: x = " << game_data.ball_x << ", y = " << game_data.ball_y;
    //qDebug() << "top player score: " << game_data.top_player_score << ", bottom player score: " << game_data.bottom_player_score;
    if (game_data.game_pause == false){
        isPause = false;
        //更新球的位置
        QGraphicsPixmapItem *pixmap_ball = new QGraphicsPixmapItem;
        pixmap_ball->setPixmap(QPixmap::fromImage(ImgBall));
        pixmap_ball->setPos(QPointF(game_data.ball_x, game_data.ball_y));
        scene_game->addItem(pixmap_ball);
        //绘制玩家位置, 更新是否已出局的标志
        if (game_data.left_player_alive == true){
            QGraphicsPixmapItem *pixmap_lp = new QGraphicsPixmapItem;
            pixmap_lp->setPixmap(QPixmap::fromImage(ImgLeftPlayer));
            pixmap_lp->setPos(QPointF(0, 10 + game_data.left_player_pos));
            scene_game->addItem(pixmap_lp);
            if (Pos == 0) {
                Score = game_data.left_player_score;
                isEliminated = false;
            }
        }else{
            if (Pos == 0) {
                Score = game_data.left_player_score;
                isEliminated = true;
            }
        }
        if (game_data.top_player_alive == true){
            QGraphicsPixmapItem *pixmap_tp = new QGraphicsPixmapItem;
            pixmap_tp->setPixmap(QPixmap::fromImage(ImgTopPlayer));
            pixmap_tp->setPos(QPointF(10 + game_data.top_player_pos, 0));
            scene_game->addItem(pixmap_tp);
            if (Pos == 1) {
                Score = game_data.top_player_score;
                isEliminated = false;
            }
        }else{
            if (Pos == 1) {
                Score = game_data.top_player_score;
                isEliminated = true;
            }
        }
        if (game_data.right_player_alive == true){
            QGraphicsPixmapItem *pixmap_rp = new QGraphicsPixmapItem;
            pixmap_rp->setPixmap(QPixmap::fromImage(ImgRightPlayer));
            pixmap_rp->setPos(QPointF(412, 10 + game_data.right_player_pos));
            scene_game->addItem(pixmap_rp);
            if (Pos == 2){
                Score = game_data.right_player_score;
                isEliminated = false;
            }
        }else{
            if (Pos == 2){
                Score = game_data.right_player_score;
                isEliminated = true;
            }
        }
        if (game_data.bottom_player_alive == true){
            QGraphicsPixmapItem *pixmap_bp = new QGraphicsPixmapItem;
            pixmap_bp->setPixmap(QPixmap::fromImage(ImgBottomPlayer));
            pixmap_bp->setPos(QPointF(10 + game_data.bottom_player_pos, 412));
            scene_game->addItem(pixmap_bp);
            if (Pos == 3){
                Score = game_data.bottom_player_score;
                isEliminated = false;
            }
        }else{
            if (Pos == 3){
                Score = game_data.bottom_player_score;
                isEliminated = true;
            }
        }
        //更新玩家分数
        ui->label_8->setText(QString::number(game_data.left_player_score));
        ui->label_9->setText(QString::number(game_data.top_player_score));
        ui->label_10->setText(QString::number(game_data.right_player_score));
        ui->label_11->setText(QString::number(game_data.bottom_player_score));
        //展示游戏场景
        ui->graphicsView_2->setScene(scene_game);
        //ui->graphicsView_2->setAlignment();
        //玩家出局需要有文字提醒
        if (isEliminated)
        {
            ui->label_12->setText(QString::fromStdWString(L"您已经出局"));
            ui->label_12->show();
        }else
            ui->label_12->hide();
    }else{
        isPause = true;
        //因为有玩家刚刚出局，导致游戏需要暂停
        if (game_data.left_player_alive == false && LastTimePlayerAlive[0] == true)
            ui->label_12->setText(QString::fromStdWString(L"左侧玩家出局\n准备重新开始"));
        else if (game_data.top_player_alive == false && LastTimePlayerAlive[1] == true)
            ui->label_12->setText(QString::fromStdWString(L"上方玩家出局\n准备重新开始"));
        else if (game_data.right_player_alive == false && LastTimePlayerAlive[2] == true)
            ui->label_12->setText(QString::fromStdWString(L"右侧玩家出局\n准备重新开始"));
        else if (game_data.bottom_player_alive == false && LastTimePlayerAlive[3] == true)
            ui->label_12->setText(QString::fromStdWString(L"下方玩家出局\n准备重新开始"));

        ui->label_12->show();
    }
}

void MainWindow::GameOver(GameData game_data)
{
    bool is_win = false;
    Period = PERIOD_GAME_OVER;
    scene_game->clear(); //清空游戏界面
    //判断玩家是否获胜，并显示出来
    if (game_data.left_player_alive == true && Pos == 0)
        is_win = true;
    else if (game_data.top_player_alive == true && Pos == 1)
        is_win = true;
    else if (game_data.right_player_alive == true && Pos == 2)
        is_win = true;
    else if (game_data.bottom_player_alive == true && Pos == 3)
        is_win = true;

    if (is_win)
        ui->label_12->setText(QString::fromStdWString(L"恭喜您获胜了\n您的分数为") + QString::number(Score));
    else
        ui->label_12->setText(QString::fromStdWString(L"游戏结束\n您的分数为") + QString::number(Score));
    ui->label_12->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //首先获取准备移动的方向
    char dirc = -100;
    if (Pos == 0 || Pos == 2){ //左右两边的玩家操作上下键
        if (event->key() == Qt::Key_Up && event->isAutoRepeat() == false)
            dirc = MOVING_LEFT_OR_TOP;
        else if (event->key() == Qt::Key_Down && event->isAutoRepeat() == false)
            dirc = MOVING_RIGHT_OR_BOTTOM;
    }else if (Pos == 1 || Pos == 3){ //上下两边的玩家操作左右键
        if (event->key() == Qt::Key_Left && event->isAutoRepeat() == false)
            dirc = MOVING_LEFT_OR_TOP;
        else if (event->key() == Qt::Key_Right && event->isAutoRepeat() == false)
            dirc = MOVING_RIGHT_OR_BOTTOM;
    }
    //然后判断这个按键信息能否发送出去。判断标准是按键信息正确, 游戏正在运行中，没有暂停，且玩家没有出局
    if (dirc == MOVING_LEFT_OR_TOP || dirc == MOVING_RIGHT_OR_BOTTOM){
        if (Period == PERIOD_IN_GAME && isEliminated == false && isPause == false){
            client.SendOperation(RoomId, Pos, dirc);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    //首先判断按键的意图是否为“停止移动”
    char dirc = -100;
    if (Pos == 0 || Pos == 2){ //左右两边的玩家操作上下键
        if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) && event->isAutoRepeat() == false)
            dirc = MOVING_STOP;
    }else if (Pos == 1 || Pos == 3){ //上下两边的玩家操作左右键
        if ((event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) && event->isAutoRepeat() == false)
            dirc = MOVING_STOP;
    }
    //然后判断这个按键信息能否发送出去。判断标准是按键信息正确, 游戏正在运行中，没有暂停，且玩家没有出局
    if (dirc == MOVING_STOP){
        if (Period == PERIOD_IN_GAME && isEliminated == false && isPause == false){
            client.SendOperation(RoomId, Pos, dirc);
        }
    }
}

void MainWindow::Login()
{
    //请求登录
    qDebug() << "您选择的房间号是" << ui->spinBox->value() << ", 位置为" << ui->comboBox->currentIndex() << ".\n";
    RoomId = ui->spinBox->value();
    Pos = ui->comboBox->currentIndex();
    Period = PERIOD_LOGGING_IN;
    ui->pushButton->setEnabled(false); //连入游戏之后，就不能重新登录了
    client.SendLogin(RoomId, Pos);
}

void MainWindow::OnGameTimerTriggered()
{
    if (Period == PERIOD_NOT_LOGGED_IN)  //根据当前游戏的阶段来决定，每隔一段时间执行什么内容
    {
        //还没登录上，就什么都不做
    }
    else if (Period == PERIOD_LOGGING_IN) //正在登录中，检查登录的完成情况
    {
        int login_res = client.GetLoginStatus();
        switch(login_res)
        {
        case -1: //登录失败了。退回到未登录的状态
            qDebug() << "登录失败了.\n";
            Period = PERIOD_NOT_LOGGED_IN;
            ui->pushButton->setEnabled(true);
            break;
        case 1: //登录已经完成，进入下一状态
            qDebug() << "登录成功，等待游戏开始.\n";
            Period = PERIOD_WAITING_GAME_START;
            break;
        case 0: //还没登录完成。继续等待
        default:
            break;
        }
    }
    else if (Period == PERIOD_WAITING_GAME_START) //登录成功, 等待游戏开始的状态
    {
        int start_res = client.GetGameStartStatus();
        switch(start_res)
        {
        case 1: //游戏已经开始
            qDebug() << "游戏已经开始.\n";
            GameStart();
            Period =  PERIOD_IN_GAME;
            break;
        case 0: //游戏尚未开始，需要继续等待
        default:
            break;
        }
    }
    else if (Period == PERIOD_IN_GAME)
    {
        GameData gdata;
        int game_res = client.GetGameData(&gdata);
        if (game_res == 1)
        {
            if (gdata.game_alive == true){ //首先判断游戏是否已经结束
                UpdateGameDataAndGraphics(gdata);
            }
            else{
                GameOver(gdata);
            }
        }
    }
    else if (Period == PERIOD_GAME_OVER)
    {
        //游戏已经结束了，什么都不做
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
