#include "dbhnd.h"

DBHandle::DBHandle() {

}

DBHandle::~DBHandle() {

}

// 连接数据库 flag=0搜索当前信息，flag=1插入新信息
bool DBHandle::ConnectDB(User &userdata, int flag) {
    // 创建数据库链接类型
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "MyAccessDB");
//    qDebug() << QDir::currentPath();
    const QString strName(QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=%1")
                          .arg(QDir::currentPath() + "/userdata.accdb"));
    // 设置数据库驱动及源文件信息
    db.setDatabaseName(strName);
    if (!db.open()) {
        qDebug() << db.lastError().text();
        return false;
    }
//    qDebug() << "connected success";
    // 创建数据库查询对象
    QSqlQuery query(db);
    if (flag == 0) {
        // 设置查询表名
        query.exec("select * from userdata");
        while(query.next()) {
            if (userdata.id == query.value(1).toString() || userdata.id == query.value(2).toString()) {
                if (userdata.pwd == query.value(3).toString()) {
                    userdata.id = query.value(1).toString();
                    userdata.name = query.value(2).toString();
                    userdata.pwd = query.value(3).toString();
                    userdata.grade = query.value(4).toString();
                    userdata.motto = query.value(5).toString();
                    return true;
                }
            }
        }
        return false;
    }
    else if (flag == 1) {
        // 设置查询表关键字
        query.prepare("insert into userdata (userid, username, password, grade, sign) "
                      "VALUES (:userid, :username, :password, :grade, :sign)");
        query.bindValue(":userid", userdata.id);
        query.bindValue(":username", userdata.name);
        query.bindValue(":password", userdata.pwd);
        query.bindValue(":grade", userdata.grade);
        query.bindValue(":sign", userdata.motto);
        query.exec();
        return true;
    }
    return false;
}
