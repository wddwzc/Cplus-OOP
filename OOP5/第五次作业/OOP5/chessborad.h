#ifndef CHESSBORAD_H
#define CHESSBORAD_H

#include <fstream>
#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

#include "algthread.h"

// 棋盘类型
class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    ChessBoard(QWidget *parent = 0);
    ~ChessBoard();

signals:
    void KnightMoved(QPoint);

public slots:
    void setOrigination(QPoint apoint);
    void setSpeed(int valspd);
    void updateStatus(int status);
    void saveStatus();
    void loadStatus();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onDisplay(int index);

private:
    // 骑士图标
    QLabel *lab_knight;
    // 骑士位置索引
    QPoint index_knight;
    // 算法线程
    ALGThread *algThread;
    // 运行速度
    int actionSpeed;
    // 当前递归步数索引
    int current_index;
    // 当前运行状态
    int current_status;

    QPoint getPoseFromIndex(int index_x, int index_y);
    QPoint getIndexFromPose(int x, int y);
    // 通过动画或直接移动骑士图标
    void moveKnight(QPoint target, int spd = 10);
};

#endif // CHESSBORAD_H
