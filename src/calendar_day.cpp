#include "calendar_day.h"
#include "ui_calendar_day.h"

Calendar_day::Calendar_day(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calendar_day)
{
    ui->setupUi(this);

    setAttribute(Qt::WidgetAttribute::WA_StyledBackground);
    reset();
}

Calendar_day::~Calendar_day()
{
    delete ui;
}

void Calendar_day::set(QString date, bool isShow, bool isWeekend, bool isToday) {
    if (isShow != true) {
        return;
    }
    if (isWeekend) {
        setStyleSheet("background-color: rgb(255,245,245);");
    } else {
        setStyleSheet("background-color: #fff;");
    }
    if (isToday) {
        setStyleSheet("background-color: #F3F8FF;");
        ui->dateLabel->setStyleSheet("font: 150 12pt \"微软雅黑\";color: #4896FA;");
    }

    title = date;
    ui->dateLabel->setText(date);
    ui->dateLabel->setVisible(true);
//    ui->label1->setStyleSheet("background-color: rgb(229, 1, 229)");
//    ui->label1->setText("测试示例 10:00-11:00");
//    ui->label1->setVisible(true);
}
//color: rgb(0, 0, 0);\nfont: 8pt "微软雅黑";
void Calendar_day::reset() {
    setStyleSheet("background-color: rgb(245, 245, 245);");
    ui->dateLabel->setVisible(false);
    ui->dateLabel->setStyleSheet("font: 12pt \"微软雅黑\";color: #000;");
    ui->label1->setVisible(false);
    ui->label1->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
    ui->label2->setVisible(false);
    ui->label2->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
    ui->label3->setVisible(false);
    ui->label3->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
}

void Calendar_day::addDayItem(QString title, int type) {
    DayItemClass item;
    item.title = title;
    item.type = type;
    items.push_back(item);
}
