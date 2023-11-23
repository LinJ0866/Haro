#include "db.h"

Db::Db()
{

}

// private

// 创建数据表
void Db::checkAndCreateTables()
{
    QSqlQuery sql_query;
    QStringList table_name = {"config"};
    QStringList create_sqls = {
        "create table IF NOT EXISTS config (coordinate_x int, coordinate_y int, scale int, dress_head int, dress_clothes int, button1_id uint64, button2_id uint64, button3_id uint64, button4_id uint64)"
    };
    for(int i = 0;i < create_sqls.size(); i++) {
        this->queryExec(create_sqls[i]);
    }
}

// 错误输出
void Db::errorDb(QSqlDatabase *db) {
    qDebug() << " ERROR: " << db->lastError();
}

void Db::errorDb(QSqlDatabase *db, QString desc) {
    qDebug() << desc << " ERROR: " << db->lastError();
}

void Db::errorSql(QSqlQuery *sql_query) {
    qDebug() << " ERROR: " << sql_query->lastError();
}

void Db::errorSql(QSqlQuery *sql_query, QString desc) {
    qDebug() << desc << " ERROR: " << sql_query->lastError();
}

//执行sql语句，不获取结果
bool Db::queryExec(QString sqlStr)
{
    if (this->status != 1) {
        qDebug() << "[db] Fail to init Db";
        return false;
    }

    QSqlQuery query;
    query.prepare(sqlStr);
    if(!query.exec(sqlStr))
    {
        errorSql(&query);
        return false;
    }
    return true;
}
//执行sql语句，并获取结果
bool Db::queryExec(QString queryStr, QList<QHash<QString,QString>> &data)
{
    if (this->status != 1) {
        qDebug() << "[db] Fail to init Db";
        return false;
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if(!query.exec(queryStr))
    {
        errorSql(&query);
        return false;
    }
    QSqlRecord rec = query.record();
    while(query.next())
    {
        QHash<QString,QString> rowData;
        for(int i =0;i<rec.count();i++)
        {
            QVariant::Type ty =  query.value(i).type();
            if( QVariant::Type::Date == ty)
            {
                QDate temp = query.value(i).toDate();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd");
            }
            else if( QVariant::Type::Time == ty)
            {
                QTime temp = query.value(i).toTime();
                rowData[rec.fieldName(i)]=temp.toString("hh:mm:ss");
            }
            else if( QVariant::Type::DateTime == ty)
            {
                QDateTime temp = query.value(i).toDateTime();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd hh:mm:ss");
            }
            else
                rowData[rec.fieldName(i)]=query.value(i).toString();
        }
        data.append(rowData);
    }
    return true;
}
//执行sql语句，不获取结果，带报错
bool Db::queryExec(QString sqlStr, QString err)
{
    if (this->status != 1) {
        qDebug() << "[db] Fail to init Db";
        return false;
    }

    QSqlQuery query;
    query.prepare(sqlStr);
    if(!query.exec(sqlStr))
    {
        errorSql(&query, err);
        return false;
    }
    return true;
}
//执行sql语句，并获取结果，带报错
bool Db::queryExec(QString queryStr, QList<QHash<QString,QString>> &data, QString err)
{
    if (this->status != 1) {
        qDebug() << "[db] Fail to init Db";
        return false;
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if(!query.exec(queryStr))
    {
        errorSql(&query, err);
        return false;
    }
    QSqlRecord rec = query.record();
    while(query.next())
    {
        QHash<QString,QString> rowData;
        for(int i =0;i<rec.count();i++)
        {
            QVariant::Type ty =  query.value(i).type();
            if( QVariant::Type::Date == ty)
            {
                QDate temp = query.value(i).toDate();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd");
            }
            else if( QVariant::Type::Time == ty)
            {
                QTime temp = query.value(i).toTime();
                rowData[rec.fieldName(i)]=temp.toString("hh:mm:ss");
            }
            else if( QVariant::Type::DateTime == ty)
            {
                QDateTime temp = query.value(i).toDateTime();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd hh:mm:ss");
            }
            else
                rowData[rec.fieldName(i)]=query.value(i).toString();
        }
        data.append(rowData);
    }
    return true;
}

// public

// 连接数据库
bool Db::connect()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString("./data.db"));
    db.setUserName("haro");
    db.setPassword("haro@2023");

    if(!db.open()){
        this->errorDb(&db, "[db] Can not find or create the database.");
        return false;
    }
    this->status = 1;
    this->checkAndCreateTables();
    qDebug() << "[db] init success";
    return true;
}

// 关闭连接
bool Db::close()
{
    QSqlDatabase::database().close();
    return true;
}

//获取数据
bool Db::getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    data.clear();
    QList<QHash<QString,QString>> dataList;
    if(!getData(tableName,dataList,sqlWhere))
    {
        return false;
    }
    if(dataList.count() > 0)
    {
        data = dataList[0];
    }
    return true;
}
//获取数据
bool Db::getData(QString tableName,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    QString queryStr="select * from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(queryStr,data))
    {
        return  false;
    }
    return true;
}

//增加
bool Db::addData(QString tableName,QHash<QString,QString> data)
{
    if(data.isEmpty())
        return false;
    QString queryStr="insert into "+tableName+" ";
    QString fieldStr="(",valueStr="values(";
    QHash<QString,QString>::iterator it;
    for ( it = data.begin(); it != data.end(); ++it )
    {
        fieldStr+=it.key()+",";
        valueStr+="'"+it.value()+"',";
    }
    fieldStr=fieldStr.left(fieldStr.length()-1);
    valueStr=valueStr.left(valueStr.length()-1);
    fieldStr+=")";
    valueStr+=")";
    queryStr+=fieldStr+" "+valueStr;
    if(!queryExec(queryStr))
    {
        return false;
    }
    return true;
}
//删除
bool Db::delData( QString tableName, QString sqlWhere)
{
    QString queryStr="delete from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(queryStr))
    {
        return false;
    }
    return true;
}
//修改
bool Db::updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    QString queryStr="update "+tableName+" ";
    QHash<QString,QString>::iterator it;
    QString setStr="set ";
    for ( it = data.begin(); it != data.end(); ++it )
    {
        setStr+=it.key()+"='"+it.value()+"'";
        setStr+=",";
    }
    setStr=setStr.left(setStr.length()-1);
    queryStr+=setStr;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(queryStr))
    {
        return false;
    }
    return true;
}
