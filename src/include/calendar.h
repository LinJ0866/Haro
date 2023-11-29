#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QIcon>
#include <QDate>
#include <QDebug>

#include "resource.h"
#include "calendar_day.h"
#include "db.h"

using namespace std;

namespace Ui {
class Calendar;
}

class Calendar : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    ~Calendar();

    void updateTime();
    void render();
    void getDb(Db *db);
private:
    Ui::Calendar *ui;
    Calendar_day* getdayItem(int y, int x);
    Db *db;
    int month, year;
    int nowMonth, nowDay;
private slots:
    void goPrev();
    void goAfter();
};




#endif // CALENDAR_H
