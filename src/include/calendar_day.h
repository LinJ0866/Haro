#ifndef CALENDAR_DAY_H
#define CALENDAR_DAY_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QIcon>
#include <QDebug>
#include <QLabel>

#include "resource.h"

namespace Ui {
class Calendar_day;
}

class DayItemClass {
public:
    QString title;
    int workType;
    QString start_time;
    QString end_time;
    int score;
};


class Calendar_day : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar_day(QWidget *parent = 0);
    ~Calendar_day();

    void set(QString date, bool isShow, bool isWeekend, bool isToday);
    void reset();
    void addDayItem(QList<QHash<QString,QString>> dayItems);
private:
    Ui::Calendar_day *ui;

    QString title;
    QVector<DayItemClass> items;

    void renderLabel(QLabel *label, DayItemClass dayItem);
};




#endif // CALENDAR_DAY_H
