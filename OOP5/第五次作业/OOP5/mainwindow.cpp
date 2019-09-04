#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(700, 520);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(210,204,176));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    InitWindow();
    status = STATUS_READY;
}

MainWindow::~MainWindow()
{

}

// Start按键槽函数
void MainWindow::on_start() {
    // 判断当前运行状态，修改按键显示信息
    if (status == STATUS_READY) {
        btn_start->setText("Pause");
        status = STATUS_RUNNING;
    }
    else if (status == STATUS_RUNNING) {
        btn_start->setText("Continue");
        status = STATUS_PAUSE;
    }
    else if (status == STATUS_PAUSE) {
        btn_start->setText("Pause");
        status = STATUS_RUNNING;
    }
    // 将信息传递给棋盘对象
    emit StatusChanged(status);
}

// Stop按键槽函数
void MainWindow::on_stop() {
    if (status) {
        btn_start->setText("Start");
        status = STATUS_READY;
        emit StatusChanged(status);
    }
}

// Print槽函数
// 对当前窗口截图，输出至pdf
void MainWindow::on_print() {
    QString filename = QFileDialog::getSaveFileName(this, "Save result as pdf", ".", ".pdf");
    if (filename == "") {
        return;
    }
    if (!filename.contains(".pdf", Qt::CaseSensitive)) {
        filename = filename + ".pdf";
    }
    QPainter text_painter;
    QPrinter text_printer;
    text_printer.setOutputFormat(QPrinter::PdfFormat);
    text_printer.setOutputFileName(filename);
    text_painter.begin(&text_printer);
    QPixmap pixmap = QPixmap::grabWidget(this, rect());
    QRect rect = text_painter.viewport();
    int factor = rect.width() / pixmap.width();
    text_painter.scale(factor, factor);
    text_painter.drawPixmap(10, 10, pixmap);
//    text_printer.newPage();
//    for (int i = 0; i < 5; i++)
//    text_painter.drawText(10, i * 30, "hello world");
    text_painter.end();
    qDebug() << "printed";
}

// 槽函数：根据棋盘类中的按键响应，修改工具栏中初始位置信息
void MainWindow::updateOrigination(QString text) {
    Q_UNUSED(text)
    int X = cmb_X->currentIndex();
    int Y = cmb_Y->currentIndex();
    emit OriginationChanged(QPoint(X, Y));
}

// 槽函数：更新工具栏中初始位置信息
// 修改QComboBox的值
void MainWindow::updateIndex(QPoint index) {
    cmb_X->setCurrentIndex(index.x());
    cmb_Y->setCurrentIndex(index.y());
}

// 槽函数：响应滑动条，修改速度显示值
void MainWindow::speedChanging(int spd) {
    QString num;
    if (spd == sld_speed->maximum()) {
        num = "Max";
    }
    else {
        num = QString::number(spd);
    }
    lab_speed->setText("Speed:  " + num);
}

// 初始化窗口
void MainWindow::InitWindow() {
    lab_title = new QLabel(this);
    lab_XY = new QLabel(this);
    lab_X = new QLabel(this);
    lab_Y = new QLabel(this);
    cmb_X = new QComboBox(this);
    cmb_Y = new QComboBox(this);
    lab_speed = new QLabel(this);
    sld_speed = new QSlider(Qt::Horizontal ,this);
    btn_start = new QToolButton(this);
    btn_stop = new QToolButton(this);
    btn_save = new QToolButton(this);
    btn_load = new QToolButton(this);
    btn_print = new QToolButton(this);
    currentboard = new ChessBoard(this);

    lab_title = new QLabel(this);
//    lab_title->resize(100, 50);
    QPixmap pixw_size;
    pixw_size.load(":/image/logo");
    pixw_size = pixw_size.scaled(160,80,Qt::KeepAspectRatio);
    lab_title->setPixmap(pixw_size);

    lab_XY->setText("The origination:");
    lab_X->setText("X:");
    lab_Y->setText("Y:");
    for (int i = 0; i < 8; i++) {
        cmb_X->addItem(QString('A' + i));
        cmb_Y->addItem(QString('1' + i));
    }
    lab_speed->setText("Speed:  ");
    sld_speed->setMaximum(100);
    sld_speed->setMinimum(10);
    btn_start->setText("Start");
    btn_stop->setText("Stop");
    btn_save->setText("Save");
    btn_load->setText("Load");
    btn_print->setText("Print");

    connect(cmb_X, &QComboBox::currentTextChanged, this, &MainWindow::updateOrigination);
    connect(cmb_Y, &QComboBox::currentTextChanged, this, &MainWindow::updateOrigination);
    connect(this, &MainWindow::OriginationChanged, currentboard, &ChessBoard::setOrigination);
    connect(currentboard, &ChessBoard::KnightMoved, this, &MainWindow::updateIndex);
    updateOrigination("init");
    connect(sld_speed, &QSlider::valueChanged, currentboard, &ChessBoard::setSpeed);
    connect(sld_speed, &QSlider::valueChanged, this, &MainWindow::speedChanging);
    sld_speed->setValue(50);
    connect(btn_start, &QToolButton::clicked, this, &MainWindow::on_start);
    connect(btn_stop, &QToolButton::clicked, this, &MainWindow::on_stop);
    connect(btn_save, &QToolButton::clicked, currentboard, &ChessBoard::saveStatus);
    connect(btn_load, &QToolButton::clicked, currentboard, &ChessBoard::loadStatus);
    connect(btn_print, &QToolButton::clicked, this, &MainWindow::on_print);
    connect(this, &MainWindow::StatusChanged, currentboard, &ChessBoard::updateStatus);
    setStyle();
    LayoutSet();
}

// 配置布局管理器
void MainWindow::LayoutSet() {
    lyt_func = new QVBoxLayout;
    lyt_func->addWidget(lab_title);
    lyt_func->addWidget(lab_XY);
    lyt_func->addWidget(lab_X);
    lyt_func->addWidget(cmb_X);
    lyt_func->addWidget(lab_Y);
    lyt_func->addWidget(cmb_Y);
    lyt_func->addWidget(lab_speed);
    lyt_func->addWidget(sld_speed);

    lyt_func->addStretch();

    lyt_func->addWidget(btn_start);
    lyt_func->addWidget(btn_stop);
    lyt_func->addWidget(btn_save);
    lyt_func->addWidget(btn_load);
    lyt_func->addWidget(btn_print);
    lyt_main = new QHBoxLayout(this);
    lyt_main->addLayout(lyt_func);
    lyt_main->addStretch();
    lyt_main->addWidget(currentboard);
}

// 配置部分控件样式表
void MainWindow::setStyle() {
    currentboard->setStyleSheet("QWidget{border-top-left-radius:5px;"
                                "border-top-right-radius:5px;"
                                "border-bottom-left-radius:5px;"
                                "border-bottom-right-radius:5px; }");
    const char btn_style[] = "QToolButton{ color: white; background: #CD853F;"
                             "font-size: 20px; width: 100px; padding: 5px; border-radius: 10px; }"
                             "QToolButton:hover{ background: #CD9B1D; }";
    btn_start->setStyleSheet(btn_style);
    btn_stop->setStyleSheet(btn_style);
    btn_save->setStyleSheet(btn_style);
    btn_load->setStyleSheet(btn_style);
    btn_print->setStyleSheet(btn_style);
}
