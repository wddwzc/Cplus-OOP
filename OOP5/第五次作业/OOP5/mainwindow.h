#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessborad.h"
#include <QMainWindow>
#include <QToolButton>
#include <QComboBox>
#include <QLabel>
#include <QSlider>
#include <QFileDialog>
#include <QPrinter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>

// 算法运行状态
#define STATUS_READY    0
#define STATUS_RUNNING  1
#define STATUS_PAUSE    2

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void OriginationChanged(QPoint);
    void StatusChanged(int _status);

private slots:
    void on_start();
    void on_stop();
    void on_print();
    void updateOrigination(QString text);
    void updateIndex(QPoint);
    void speedChanging(int spd);

private:
    int status;

    QLabel *lab_title;
    QLabel *lab_XY;
    QLabel *lab_X;
    QLabel *lab_Y;
    QComboBox *cmb_X;
    QComboBox *cmb_Y;
    QLabel *lab_speed;
    QSlider *sld_speed;
    QToolButton *btn_start;
    QToolButton *btn_stop;
    QToolButton *btn_save;
    QToolButton *btn_load;
    QToolButton *btn_print;
    ChessBoard *currentboard;
    QHBoxLayout *lyt_main;
    QVBoxLayout *lyt_func;

    void InitWindow();
    void LayoutSet();
    void setStyle();

};

#endif // MAINWINDOW_H
