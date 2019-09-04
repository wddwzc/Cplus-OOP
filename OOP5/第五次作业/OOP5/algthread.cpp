#include "algthread.h"
#include <QDebug>

ALGThread::ALGThread()
    : startx(0), starty(0) , algSpeed(50)
{
    isStopped = false;
    isPause = false;
    x_indexes.resize(64);
    y_indexes.resize(64);
    step = 1;
}

ALGThread::~ALGThread() {
    qDebug() << "destroy";
}

void ALGThread::run() {
    isStopped = false;
    path[startx][starty] = step;
    BackTrace(startx, starty, step);   // 调用递归回溯函数
}

// 设置算法线程运行的初始参数
void ALGThread::setStartPara(int _startx, int _starty, int now_step, int *repath)
{
    startx = _startx;
    starty = _starty;
    step = now_step + 1;
    // 如果存在传入棋盘参数，对应状态加载功能
    if (repath) {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                path[i][j] = repath[j * 8 + i];
    }
    else {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                path[i][j] = 0;
    }
}

//设置速度，其实就是线程sleep的时间
void ALGThread::set_velocity(int _velocity)
{
    algSpeed = _velocity;
}

void ALGThread::setpause()  //设置暂停
{ isPause = true; }

void ALGThread::setcontinue()  //设置继续
{ isPause = false; }

void ALGThread::stopThread() {
    isPause = false;
    isStopped = true;
}

bool ALGThread::isStop()
{ return isStopped; }

//判断当前棋格是否可落子
int ALGThread::canJump(int x, int y)
{
    if (x >= 0 && x < 8 && y >= 0 && y < 8 && path[x][y] == 0)
        return 1;
    return 0;
}

static bool cmp(struct node a, struct node b)
{
    if(a.value < b.value)
    {
        return true;
    }
    return false;
}

// 计算周围节点可行价值
vector<node> ALGThread::sort_Step(int r , int c)
{
    vector<node> outway(8);
    for(int i = 0 ; i < 8 ; i++)
    {
        outway[i].value = 0;
        outway[i].index = i;
    }
    int nextr, nextc;
    for(int i = 0 ; i < 8 ; i++)
    {
        nextr = r + next_jump[i][0];
        nextc = c + next_jump[i][1];
        // 判断该点是否为可行点，如果不可行则价值为默认值0
        if(nextr >= 0 && nextc >= 0 && nextr < 8 && nextc < 8 && path[nextr][nextc] == 0)
        {
            // 遍历其周围点，每存在一个可行点价值加一
            for(int j = 0 ; j < 8 ; j++)
            {
                int a = nextr + next_jump[j][0];
                int b = nextc + next_jump[j][1];
                if(a >= 0&& b >= 0 && a < 8 && b < 8 && path[a][b] == 0)
                    outway[i].value++;
            }
        }
    }
    // 通过添加谓词，对价值进行降序排序
    stable_sort(outway.begin(),outway.end(),cmp);
    return outway;
}

// 递归回溯函数
// x、y：当前层棋子落地的位置 _step:当前递归层数
void ALGThread::BackTrace(int x, int y, int _step)
{
    if (isStopped)  return;
    if (_step>=64)  // 递归层数达到64即找到周游路径，跳出函数
    {
        qDebug()<<"over";
        isStopped = true;
        isPause = true;
        return;
    }
    else  // 递归主体
    {
        vector<node> outway(8);
        outway = ALGThread::sort_Step(x, y);    // 获取周围八点可行价值降序序列
        // 遍历八个方向
        for (auto node : outway)
        {
            int next_x = x + next_jump[node.index][0];
            int next_y = y + next_jump[node.index][1];
            if (canJump(next_x, next_y))
            {
                _step++;
                // 置标志位，尝试将棋子落在此位置
                path[next_x][next_y] = _step;
                updateIndexes();
                // 发送界面显示更新信号
                emit updateDisplay(_step - 1);
                // 控制算法运行速度
                msleep(1600 - 15 * algSpeed);
                // 标志位控制算法暂停
                while(isPause) {
                    msleep(1000);
                }
                // 继续下一层递归
                BackTrace(next_x, next_y, _step);
                if(!isStopped) {
                    // 若递归跳出，且算法还未结束，尝试失败，恢复标志位
                    _step--;
                    path[next_x][next_y] = 0;
                    updateIndexes();
                    qDebug() << "back";
                    emit updateDisplay(_step - 1);
                    msleep(1600 - 15 * algSpeed);
                }
                else {
                    return;
                }
            }
        }
    }
}

// 根据棋盘信息更新显示数据
void ALGThread::updateIndexes() {
//    std::cout << "update:" << std::endl;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if(path[j][i] != 0)
            {
                x_indexes[path[j][i] - 1] = j;
                y_indexes[path[j][i] - 1] = i;
            }
//            std::cout << path[j][i];
        }
//        std::cout << std::endl;
    }
}
