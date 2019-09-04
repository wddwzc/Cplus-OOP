#ifndef ALGTHREAD_H
#define ALGTHREAD_H

#include <vector>
#include <iostream>
#include <QThread>

using namespace std;

const int next_jump[8][2] = {{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};

// 棋盘格节点类型，用于计算可行价值
struct node
{
    int value;
    int index;
    node(){value = 0;index = 0;}
};

// 算法线程类型
class ALGThread : public QThread
{
    Q_OBJECT

public:
    explicit ALGThread();
    ~ALGThread();

public:
    //设置初始参数， now_step repath默认值对应默认开始，有传入之对应状态加载
    void setStartPara(int _startx, int _starty,  int now_step = 0, int *repath = NULL);
    void set_velocity(int _velocity);
    void setpause();
    void setcontinue();
    void stopThread();
    bool isStop();

    vector<int> x_indexes;  //保存棋子走过的轨迹，x坐标
    vector<int> y_indexes;  //保存棋子走过的轨迹，y坐标
    int path[8][8];

protected:
    void run();

private:
    bool isStopped;
    bool isPause;

    //算法初始位置x
    int startx;
    //算法初始位置y
    int starty;
    // 算法运行速度
    int algSpeed;
    // 当前迭代最大层数
    int step;

    // 判断索引是否有效
    int canJump(int x, int y);
    // 递归回溯函数
    void BackTrace(int x, int y, int _step);
    // 根据棋盘格信息更新输出数据
    void updateIndexes();
    // 计算周围节点可行价值
    vector<node> sort_Step(int c, int r);

signals:
    void updateDisplay(int);  //信号，通知界面显示当前结果
};

#endif // ALGTHREAD_H
