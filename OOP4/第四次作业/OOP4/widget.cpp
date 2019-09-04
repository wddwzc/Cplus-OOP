#include "widget.h"

LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) { }

LineEdit::~LineEdit() { }

// 重写按键事件，响应回车键
void LineEdit::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        // 检测到回车时忽略该事件，交由父窗口处理，切换focus
        event->ignore();
    }
    else {
        QLineEdit::keyPressEvent(event);
    }
}

// 重写鼠标事件，相应鼠标左键单击，清空当前输入
void LineEdit::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        this->clear();
    }
}

// 注册界面构造并初始化
RegistDialog::RegistDialog(QWidget *parent)
    : QDialog(parent) {
    this->setMinimumSize(400, 300);
    lab_id = new QLabel(this);
    lab_name = new QLabel(this);
    lab_password = new QLabel(this);
    lab_grade = new QLabel(this);
    lab_motto = new QLabel(this);
    lab_id->setText(tr("   ID   "));
    lab_name->setText(tr("  Name  "));
    lab_password->setText(tr("Password"));
    lab_grade->setText(tr(" Grade  "));
    lab_motto->setText(tr("  Sign  "));
    ledt_id = new LineEdit(this);
    ledt_id->setText(QString::fromStdString("请输入六位数ID"));
    ledt_name = new LineEdit(this);
    ledt_name->setText(QString::fromStdString("请以非数字开头且不含空格"));
    ledt_password = new LineEdit(this);
    ledt_password->setText(QString::fromStdString("不能为空"));
    ledt_grade = new LineEdit(this);
    ledt_grade->setText(QString::fromStdString("选填"));
    ledt_motto = new LineEdit(this);
    ledt_motto->setText(QString::fromStdString("选填"));
    btn_affirm = new QToolButton(this);
    btn_affirm->setText(tr("OK"));
    btn_cancel = new QToolButton(this);
    btn_cancel->setText(tr("Cancel"));
    lyt_main = new QGridLayout(this);
    connect(btn_affirm, &QToolButton::clicked, this, &RegistDialog::AffirmRegist);
    connect(btn_cancel, &QToolButton::clicked, this, &RegistDialog::QuitDialog);
    lyt_main->addWidget(lab_id, 0, 0, 1, 1);
    lyt_main->addWidget(lab_name, 1, 0, 1, 1);
    lyt_main->addWidget(lab_password, 2, 0, 1, 1);
    lyt_main->addWidget(lab_grade, 3, 0, 1, 1);
    lyt_main->addWidget(lab_motto, 4, 0, 1, 1);
    lyt_main->addWidget(ledt_id, 0, 1, 1, 3);
    lyt_main->addWidget(ledt_name, 1, 1, 1, 3);
    lyt_main->addWidget(ledt_password, 2, 1, 1, 3);
    lyt_main->addWidget(ledt_grade, 3, 1, 1, 3);
    lyt_main->addWidget(ledt_motto, 4, 1, 1, 3);
    lyt_main->addWidget(btn_affirm, 5, 2, 1, 1);
    lyt_main->addWidget(btn_cancel, 5, 3, 1, 1);

}

RegistDialog::~RegistDialog() { }

// 确认键槽函数，提交注册信息
void RegistDialog::AffirmRegist() {
    if (ledt_grade->text() == "选填") {
        ledt_grade->setText("");
    }
    else {
        userdata.grade = ledt_grade->text();
    }
    if (ledt_motto->text() == "选填") {
        ledt_motto->setText("");
    }
    else {
        userdata.motto = ledt_motto->text();
    }
    userdata.id = ledt_id->text();
    userdata.name = ledt_name->text();
    userdata.pwd = ledt_password->text();
    // 返回Accepted信息，供父窗口判别
    this->accept();
}

// 退出键槽函数，不提交注册信息
void RegistDialog::QuitDialog() {
    // 返回Rejected信息，供父窗口判别
    this->reject();
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(400, 300);
    InitWindow();
    LayoutSet();
}

Widget::~Widget()
{

}

// 初始化窗口控件及变量
void Widget::InitWindow() {
    str_id = "";
    str_name = "";

    // 登陆界面初始化
    lab_title = new QLabel(this);
    lab_title->setFixedHeight(40);
    lab_title->setFont(QFont("Microsoft YaHei", 20, 50, false));
    lab_title->setText(tr("用户登陆"));
    ledt_ID = new LineEdit(this);
    ledt_ID->setText(tr("请输入ID或用户名"));
    ledt_pwd = new LineEdit(this);
    ledt_pwd->setEchoMode(QLineEdit::Password);
    lab_ID = new QLabel(this);
    lab_ID->setText(tr("   ID:   "));
    lab_pwd = new QLabel(this);
    lab_pwd->setText(tr("PassWord:"));
    btn_login = new QToolButton(this);
    btn_login->setText(tr("Login"));
    btn_regist = new QToolButton(this);
    btn_regist->setText(tr("Regist"));
    connect(btn_login, &QToolButton::clicked, this, &Widget::UserLogin);
    connect(btn_regist, &QToolButton::clicked, this, &Widget::UserRegist);

    // 在线界面初始化，控件隐藏
    lab_online = new QLabel(this);
    lab_online->setFixedHeight(40);
    lab_online->setFont(QFont("Microsoft YaHei", 20, 50, false));
    lab_online->setText(tr("On Line"));
    lab_luser = new QLabel(this);
    lab_luser->setFixedHeight(20);
    lab_grade = new QLabel(this);
    lab_grade->setText(tr("Grade "));
    lab_motto = new QLabel(this);
    lab_motto->setText(tr("Sign  "));
    ledt_grade = new QLineEdit(this);
    ledt_grade->setEnabled(false);
    ledt_motto = new QLineEdit(this);
    ledt_motto->setEnabled(false);
    btn_logout = new QToolButton(this);
    btn_logout->setText(tr("Logout"));
    connect(btn_logout, &QToolButton::clicked, this, &Widget::UserLogout);
    lab_online->hide();
    lab_luser->hide();
    lab_grade->hide();
    lab_motto->hide();
    ledt_grade->hide();
    ledt_motto->hide();
    btn_logout->hide();
}

// 检测ID、用户名、密码有效性
int Widget::isValid(QString user, int flag) {
    // QString转为char*类型
    QByteArray ba = user.toLatin1();
    const char *s = ba.data();
    char first = ba.at(0);
    // 根据首字符是否为数字区分ID和用户名
    bool idflag = (first >= '0' && first <= '9');
    bool nameflag = !idflag;
    int idcount = 0;
    if (!flag) {
        // flag=0时，检测ID和用户名
        while(*s) {
            if (idflag && *s >= '0' && *s <= '9') {
                ++idcount;
            }
            else {
                // ID要求为六位数字，当出现非数字时为非法
                idflag = false;
            }
            if (*s == ' ') {
                // 用户名要求不含空格，当出现空格时为非法
                nameflag = false;
            }
            ++s;
        }
        // 根据标志位返回有效性
        if (idcount == 6 && idflag) {
            qDebug() << "ID";
            return 1;
        }
        else if (nameflag) {
            qDebug() << "name";
            return 2;
        }
        else {
            qDebug() << "invalid";
            return 0;
        }
    }
    else {
        // flag=1，检测密码合法性
        // 密码没有格式要求，不为空即可
        if (user.isEmpty()) {
            qDebug() << "pwd invalid";
            return 0;
        }
        else {
            return 3;
        }
    }
}

// 重写按键事件，响应回车键
void Widget::keyPressEvent(QKeyEvent *event) {
    // 父窗口检测到回车时，切换focus或触发登陆信号
    if (event->key() == Qt::Key_Return)
    {
        if (focusWidget() == ledt_ID) {
            ledt_pwd->setFocus();
        }
        else if (focusWidget() == ledt_pwd) {
            emit btn_login->clicked();
        }
    }
}

// 用户登陆槽函数
void Widget::UserLogin() {
    QString user = ledt_ID->text();
    QString pwd = ledt_pwd->text();
    User userdata;
    // 判断ID或用户名和密码合法性
    if (isValid(user) && isValid(pwd, 1)) {
        // 密码MD5加密
        EncryptionMD5(pwd);
        userdata.id = user;
        userdata.pwd = pwd;
    }
    else {
        QMessageBox::warning(this, tr("error"), tr("您的账户或密码格式错误！"));
        return;
    }
    // 创建数据库链接
    DBHandle dbhandle;
    // 判断登陆信息是否匹配
    if (dbhandle.ConnectDB(userdata)) {
        // 切入在线界面，设置控件属性
        lab_luser->setText(userdata.id + "-" + userdata.name);
        ledt_grade->setText(userdata.grade);
        ledt_motto->setText(userdata.motto);
        ChangeWindow();
    }
    else {
        QMessageBox::warning(this, "Warning", "您的账号或密码错误！");
    }
}

// 用户登出槽函数
void Widget::UserLogout() {
    // 切回到登陆界面
    ChangeWindow(1);
}

//用户注册槽函数
void Widget::UserRegist() {
    RegistDialog dialog(this);
    // 注册界面返回Accepted消息时，处理注册信息
    if (dialog.exec() == QDialog::Accepted) {
        User userdata = dialog.userdata;
        // 检测id、用户名、密码合法性
        if (isValid(userdata.id) == 1 && isValid(userdata.name) == 2 && isValid(userdata.pwd, 1) == 3) {
            DBHandle dbhandle;
            // 查询该id是否已存在
            if (!dbhandle.ConnectDB(userdata)) {
                // 不存在则可以注册，密码加密
                EncryptionMD5(userdata.pwd);
                // 注册信息插入数据库
                if (dbhandle.ConnectDB(userdata, 1)) {
                    QMessageBox::information(this, "Tips", "Success!!!");
                }
            }
            else {
                QMessageBox::warning(this, "Warning", "该账号已存在");
            }
        }
        else {
            QMessageBox::warning(this, "Warning", "The format of your data is wrong!");
        }
    }
}

// 布局管理
void Widget::LayoutSet() {
    lyt_row0 = new QHBoxLayout;
    lyt_row0->addStretch();
    lyt_row0->addWidget(lab_title);
    lyt_row0->addWidget(lab_online);
    lyt_row0->addStretch();
    lyt_row1 = new QHBoxLayout;
    lyt_row1->addWidget(lab_ID);
    lyt_row1->addWidget(ledt_ID);
    lyt_row1->addWidget(lab_luser, 0, Qt::AlignCenter);
    lyt_row2 = new QHBoxLayout;
    lyt_row2->addWidget(lab_pwd);
    lyt_row2->addWidget(ledt_pwd);
    lyt_row2->addWidget(lab_grade);
    lyt_row2->addWidget(ledt_grade);
    lyt_row3 = new QHBoxLayout;
    lyt_row3->addWidget(btn_login);
    lyt_row3->addWidget(btn_regist);
    lyt_row3->addWidget(lab_motto);
    lyt_row3->addWidget(ledt_motto);
    lyt_main = new QVBoxLayout(this);
    lyt_main->addLayout(lyt_row0);
    lyt_main->addLayout(lyt_row1);
    lyt_main->addLayout(lyt_row2);
    lyt_main->addLayout(lyt_row3);
    lyt_main->addWidget(btn_logout, 0, Qt::AlignCenter);
}

// 对字符串进行MD5码加密
void Widget::EncryptionMD5(QString &src) {
    QString strPwd = src;
    if(strPwd == nullptr)
        return;
    QByteArray bytePwd = strPwd.toLatin1();
    QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
    src = bytePwdMd5.toHex();
}

// 更新窗口布局，完成登陆界面和在线界面的切换
void Widget::ChangeWindow(int flag) {
    if (flag) {
        lab_title->show();
        ledt_ID->show();
        ledt_pwd->show();
        lab_ID->show();
        lab_pwd->show();
        btn_login->show();
        btn_regist->show();

        lab_online->hide();
        lab_luser->hide();
        lab_grade->hide();
        lab_motto->hide();
        ledt_grade->hide();
        ledt_motto->hide();
        btn_logout->hide();
    }
    else {
        lab_online->show();
        lab_luser->show();
        lab_grade->show();
        lab_motto->show();
        ledt_grade->show();
        ledt_motto->show();
        btn_logout->show();

        lab_title->hide();
        ledt_ID->hide();
        ledt_pwd->hide();
        lab_ID->hide();
        lab_pwd->hide();
        btn_login->hide();
        btn_regist->hide();
    }
}
