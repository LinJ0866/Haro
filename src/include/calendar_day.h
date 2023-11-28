#ifndef CALENDAR_DAY_H
#define CALENDAR_DAY_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QIcon>
#include <QDebug>

#include "resource.h"

namespace Ui {
class Calendar_day;
}

class DayItemClass {
public:
    QString title;
    int type;
};


class Calendar_day : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar_day(QWidget *parent = 0);
    ~Calendar_day();

    void set(QString date, bool isShow, bool isWeekend, bool isToday);
    void reset();
    void addDayItem(QString title, int type);

private:
    Ui::Calendar_day *ui;

    QString title;
    QVector<DayItemClass> items;
};




#endif // CALENDAR_DAY_H
