#ifndef DB_H
#define DB_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
    class Db;
}

class Db
{
public:
    Db();

    void connect();
    void close();
private:
    void create();
};

#endif // DB_H
