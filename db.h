#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QDate>

#include <string>

namespace Ui {
    class Db;
}

// 封装参考自：https://blog.csdn.net/hss2799/article/details/114400176

class Db
{
public:
    Db();

    bool connect();
    bool close();

    bool getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere=""); //获取数据
    bool getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool addData(QString tableName,QHash<QString,QString> data); //增加
    bool delData(QString tableName,QString sqlWhere); //删除
    bool updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere=""); //修改
private:
    int status=-1; // 数据库连接状态，已连接为1，未连接为-1
    // 打印错误
    void errorDb(QSqlDatabase *db);
    void errorDb(QSqlDatabase *db, QString desc);
    void errorSql(QSqlQuery *sql_query);
    void errorSql(QSqlQuery *sql_query, QString desc);
    // 执行sql语句
    bool queryExec(QString sqlStr);//执行sql语句，不获取结果
    bool queryExec(QString sqlStr, QList<QHash<QString,QString>> &data);// 执行sql语句，并获取结果
    bool queryExec(QString sqlStr, QString err);//执行sql语句，不获取结果
    bool queryExec(QString sqlStr, QList<QHash<QString,QString>> &data, QString err);// 执行sql语句，并获取结果

    void checkAndCreateTables();
};


#endif // DB_H
