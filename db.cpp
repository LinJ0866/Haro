#include "db.h"

Db::Db()
{

}

// 连接数据库
void Db::connect()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString("./data.db"));
    db.setUserName("haro");
    db.setPassword("haro@2023");

    if(!db.open()){
        qDebug() << "[db init] Can not find the database, try to create a new one. Error: " << db.lastError();


    }
    qDebug() << "[db init] success";
}

// 关闭连接
void Db::close()
{
    QSqlDatabase::database().close();
}

// 创建数据表
void Db::create()
{
    QSqlQuery sql_query;
    QString create_sql = "create table student (id int primary key, name varchar(30), age int)";
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }

}
