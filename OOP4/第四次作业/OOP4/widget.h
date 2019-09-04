#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QtDebug>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCryptographicHash>
#include <QFont>
#include <QDialog>
#include "dbhnd.h"

// 信息编辑模块
class LineEdit : public QLineEdit {
    Q_OBJECT

public:
    LineEdit(QWidget *parent);
    ~LineEdit();

protected:
    // 重写按键事件，响应回车键，切换focus
    virtual void keyPressEvent(QKeyEvent *event);
    // 重写鼠标事件，相应鼠标左键单击，清空当前输入
    virtual void mousePressEvent(QMouseEvent *event);
};

// 用户注册窗口
class RegistDialog : public QDialog
{
    Q_OBJECT
public:
    RegistDialog(QWidget *parent);
    ~RegistDialog();

    User userdata;
private:
    // 确认键槽函数，提交注册信息
    void AffirmRegist();
    // 退出键槽函数，不提交注册信息
    void QuitDialog();

    // 窗口控件
    QLabel *lab_id;
    QLabel *lab_name;
    QLabel *lab_password;
    QLabel *lab_grade;
    QLabel *lab_motto;
    LineEdit *ledt_id;
    LineEdit *ledt_name;
    LineEdit *ledt_password;
    LineEdit *ledt_grade;
    LineEdit *ledt_motto;
    QToolButton *btn_affirm;
    QToolButton *btn_cancel;
    QGridLayout *lyt_main;
};

// 用户登录主窗口
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public:
    // 初始化窗口控件及变量
    void InitWindow();
    // 检测ID、用户名、密码有效性
    int isValid(QString user, int flag = 0);

protected:
    // 重写按键事件，响应回车键
    virtual void keyPressEvent(QKeyEvent *event);

private slots:
    // 用户登陆槽函数
    void UserLogin();
    // 用户登出槽函数
    void UserLogout();
    //用户注册槽函数
    void UserRegist();

private:
    // 布局管理
    void LayoutSet();
    // 对字符串进行MD5码加密
    void EncryptionMD5(QString &src);
    // 更新窗口布局
    void ChangeWindow(int flag = 0);

    QLabel *lab_title;
    LineEdit *ledt_ID;
    LineEdit *ledt_pwd;
    QLabel *lab_ID;
    QLabel *lab_pwd;
    QToolButton *btn_login;
    QToolButton *btn_regist;

    QLabel *lab_online;
    QLabel *lab_luser;
    QLabel *lab_grade;
    QLabel *lab_motto;
    QLineEdit *ledt_grade;
    QLineEdit *ledt_motto;
    QToolButton *btn_logout;

    QVBoxLayout *lyt_main;
    QHBoxLayout *lyt_row0;
    QHBoxLayout *lyt_row1;
    QHBoxLayout *lyt_row2;
    QHBoxLayout *lyt_row3;
    QString str_id;
    QString str_name;
};

#endif // WIDGET_H
