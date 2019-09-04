#ifndef DBHND_H
#define DBHND_H

#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QtDebug>
#include <iostream>
#include <QMessageBox>
#include <QDir>

// 用户信息类型
class User {
public:
    QString id;
    QString name;
    QString pwd;
    QString grade;
    QString motto;
};

// 数据库连接类型
class DBHandle
{
public:
    DBHandle();
    ~DBHandle();

public slots:
    // 连接数据库，并根据flag执行对应操作
    bool ConnectDB(User &userdata, int flag = 0);

};

#endif // DBHND_H
