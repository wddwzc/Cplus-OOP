#include "chessborad.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(500, 500);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(232,220,198));//QColor(3,46,85)
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // 初始化骑士图标
    lab_knight = new QLabel(this);
    lab_knight->resize(58, 58);
    QPixmap pixw_size;
    pixw_size.load(":/image/knight");
    pixw_size = pixw_size.scaled(58,58,Qt::KeepAspectRatio);
    lab_knight->setPixmap(pixw_size);
    index_knight = QPoint(0, 0);
    setOrigination(index_knight);
    actionSpeed = 90;

    algThread = NULL;
    current_index = -1;
    current_status = 0;

//    QFile file(":/image/knight1");
//    file.open(QIODevice::ReadOnly);
//    QDataStream in(&file);

//    // Read and check the header
//    quint32 magic;
//    in >> magic;
//    if (magic != 0x89504e47)
//    {
//        qDebug() << "test";
//        //文件类型为 png
//    }

}

ChessBoard::~ChessBoard() {

}

// 槽函数：设置初始位置，由窗口对象传入位置
void ChessBoard::setOrigination(QPoint apoint) {
    moveKnight(apoint, actionSpeed);
    index_knight = apoint;
}

// 槽函数：设置运行速度
void ChessBoard::setSpeed(int valspd) {
    actionSpeed = valspd;
    if (algThread) {
        algThread->set_velocity(actionSpeed);
    }
}

// 槽函数：响应Start按键，更新运行状态
void ChessBoard::updateStatus(int _status) {
    qDebug() << _status;
    current_status = _status;
    if (_status == 0) {
        algThread->stopThread();
        algThread->wait();
        delete algThread;
        algThread = NULL;
        current_index = 0;
        this->update();
    }
    else if (_status == 1) {
        if (!algThread) {
            this->update();
            algThread = new ALGThread;
            connect(algThread, &ALGThread::updateDisplay, this, &ChessBoard::onDisplay);
            algThread->setStartPara(index_knight.x(), index_knight.y());
            algThread->set_velocity(actionSpeed);
            algThread->start();
        }
        else {
            algThread->setcontinue();
        }
    }
    else if (_status == 2) {
        algThread->setpause();
    }
}

// 保存当前 运行状态
void ChessBoard::saveStatus() {
    // 检测是否存在有效信息，没有则跳出
    if (algThread && current_index > 0) {
        algThread->setpause();
        // 用户给定文件名并保存
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"));
        if (fileName == "") {
            return;
        }
        std::ofstream outf(fileName.toStdString());
        outf << current_index << " "
             << algThread->x_indexes[current_index] << " "
             << algThread->y_indexes[current_index] << std::endl;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                outf << algThread->path[j][i] << " ";
            }
            outf << std::endl;
        }
        outf.close();
        QMessageBox::information(this, "Tips", "Success to save!");
    }
    else {
        QMessageBox::information(this, "Warning", "Nothing to save!");
    }
}

// 加载运行状态
void ChessBoard::loadStatus() {
    if (algThread) {
        algThread->setpause();
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    if (fileName == "") {
        return;
    }
    std::ifstream inf(fileName.toStdString());
    int loadpath[64], tempx, tempy;
    inf >> current_index >> tempx >> tempy;
    index_knight = QPoint(tempx, tempy);
//    qDebug() << current_index << index_knight.x() << index_knight.y();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            inf >> loadpath[i * 8 + j];
//            qDebug() << loadpath[i * 8 + j];
//            std::cout << loadpath[i*8+j];
        }
//        std::cout << std::endl;
    }
    inf.close();
//    qDebug() << "loadpath" << loadpath[index_knight.x() * 8 + index_knight.y()];
    // 加载完成后，如果已有算法线程运行，先将其关闭
    if (algThread) {
        algThread->stopThread();
        algThread->wait();
        delete algThread;
    }
    //创建新线程，设置读取的断点数据
    algThread = new ALGThread;
    connect(algThread, &ALGThread::updateDisplay, this, &ChessBoard::onDisplay);
    algThread->setStartPara(index_knight.x(), index_knight.y(), current_index, loadpath);
    algThread->set_velocity(actionSpeed);
    algThread->start();
    algThread->setpause();
    QMessageBox::information(this, "Tips", "Success to load!");
}

// 重写鼠标点击事件
void ChessBoard::mousePressEvent(QMouseEvent *event) {
    // 响应左键点击
    if (event->button() == Qt::MouseButton::LeftButton) {
        // 如果算法未运行则移动骑士图标
        if (!algThread || algThread->isStop()) {
            QPoint index = getIndexFromPose(event->x(), event->y());
            moveKnight(index, actionSpeed);
            emit KnightMoved(index);
        }
    }
    else {
        event->ignore();
    }
}

// 重写窗口绘制事件
void ChessBoard::paintEvent(QPaintEvent *event) {
    // 绘制边框
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(232,220,198)));
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 15, 15);
    // 绘制棋盘格
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if((i + j) % 2 == 0)
                painter.setBrush(QBrush(QColor(247,237,202)));
            else
                painter.setBrush(QBrush(QColor(231,200,156)));
            painter.drawRect(18 + 58 * i, 18 + 58 * j, 58, 58);
        }
    }
    // 绘制棋盘坐标
    QFont font("Arial", 10, QFont::Bold,false);
    painter.setFont(font);
    painter.setPen(QColor(90, 90, 90));
    for(int j=0; j<8; j++) {
        painter.drawText(18 + 58 * j + 24, 495, QString('A' + j));
        painter.drawText(18 + 58 * j + 24, 15, QString('A' + j));
    }
    for(int i=0; i<8; i++) {
        painter.drawText(5, 18 + 58 * i + 30, QString('1' + i));
        painter.drawText(485, 18 + 58 * i + 30, QString('1' + i));
    }
    // 绘制当前运行状态
    if(current_index > 0 && algThread)
    {
        int x1,y1,x2,y2;
        //先画标志框，代表哪些格子已经被走过
        painter.setBrush(QBrush(QColor(100,100,100)));
        painter.setPen(Qt::transparent);
        for(int i=0; i<current_index; i++)
        {
            x2 = algThread->x_indexes[i];
            y2 = algThread->y_indexes[i];
            painter.drawRect(18 + 58 * x2, 18 + 58 * y2, 58, 58);
        }

        //再画轨迹线
        painter.setPen(QPen(Qt::black,2));
        for(int i=1; i<=current_index; i++)
        {
            x1 = algThread->x_indexes[i-1];
            y1 = algThread->y_indexes[i-1];
            x2 = algThread->x_indexes[i];
            y2 = algThread->y_indexes[i];
            painter.setPen(QPen(Qt::black,2));
            painter.drawLine(18 + 58 * x1 + 29, 18 + 58 * y1 + 29,
                             18 + 58 * x2 + 29, 18 + 58 * y2 + 29);
        }
        //棋子显示到最新的地方
        int xx = algThread->x_indexes[current_index];
        int yy = algThread->y_indexes[current_index];
//        qDebug() << "pos:" << xx << " " << yy;
        index_knight = QPoint(xx, yy);
        moveKnight(index_knight, actionSpeed);
    }
    // 绘制其它控件
    QWidget::paintEvent(event);
}

// 槽函数：更新迭代步数
void ChessBoard::onDisplay(int index) {
    current_index = index;
//    qDebug() << current_index;
    this->update();
}

// 根据索引获取像素坐标
QPoint ChessBoard::getPoseFromIndex(int index_x, int index_y) {
    return QPoint(18 + 58 * index_x, 18 + 58 * index_y);
}

// 根据像素坐标获取索引
QPoint ChessBoard::getIndexFromPose(int x, int y) {
    int xindex = index_knight.x(), yindex = index_knight.y();
    for (int i = 0; i <= 8; i++) {
        if (i > 0 && x < (18 + 58 * i)) {
            xindex = i - 1;
            break;
        }
    }
    for (int i = 0; i <= 8; i++) {
        if (i > 0 && y < (18 + 58 * i)) {
            yindex = i - 1;
            break;
        }
    }
    return QPoint(xindex, yindex);
}

// 通过动画或直接的方式移动骑士图标
void ChessBoard::moveKnight(QPoint target, int spd) {
//    qDebug() << spd;
    if (spd == 100) {
        lab_knight->move(getPoseFromIndex(target.x(), target.y()));
    }
    else {
        QPropertyAnimation *pPosAnimation = new QPropertyAnimation(lab_knight, "pos");
        pPosAnimation->setDuration(300 - 3 * spd);
        pPosAnimation->setStartValue(lab_knight->pos());
        pPosAnimation->setEndValue(getPoseFromIndex(target.x(), target.y()));
        pPosAnimation->setEasingCurve(QEasingCurve::Linear);
        pPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
