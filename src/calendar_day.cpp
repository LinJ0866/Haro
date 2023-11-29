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
    title = date;
    if (isWeekend) {
        setStyleSheet("background-color: rgb(255,245,245);");
    } else {
        setStyleSheet("background-color: #fff;");
    }
    if (isToday) {
        title += "  今天";
        setStyleSheet("background-color: #F3F8FF;");
        ui->dateLabel->setStyleSheet("font: 150 12pt \"微软雅黑\";color: #4896FA;");
    }

    ui->dateLabel->setText(title);

    if (items.length() > 0) renderLabel(ui->label1, items[0]);
    if (items.length() > 1) renderLabel(ui->label2, items[1]);
    if (items.length() > 2) {
        ui->label3->setText(QString("...   <font color = #4896FA>+%1</font>").arg(items.length()-2));
        ui->label3->setVisible(true);
    }
}
//color: rgb(0, 0, 0);\nfont: 8pt "微软雅黑";
void Calendar_day::reset() {
    items.clear();
    setStyleSheet("background-color: rgb(245, 245, 245);");
    ui->dateLabel->setText("");
    ui->dateLabel->setStyleSheet("font: 12pt \"微软雅黑\";color: #000;");
    ui->label1->setText("");
    ui->label1->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
    ui->label2->setText("");
    ui->label2->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
    ui->label3->setText("");
    ui->label3->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;");
}

void Calendar_day::addDayItem(QList<QHash<QString,QString>> dayItems) {
    for(int i=0; i<dayItems.length(); i++) {
        DayItemClass item;
        item.title = dayItems[i]["work_desc"];
        item.workType = dayItems[i]["work_type"].toInt();
        item.start_time = dayItems[i]["start_time"];
        item.end_time = dayItems[i]["end_time"];
        item.score = dayItems[i]["work_score"].toInt();
        items.push_back(item);
    }

}

void Calendar_day::renderLabel(QLabel *label, DayItemClass dayItem) {
    if (dayItem.workType == 0) {
        label->setText(QString("🦾 %1\n       %2-%3").arg(dayItem.title.mid(0, 10)).arg(dayItem.start_time).arg(dayItem.end_time));
        if (dayItem.score == 5) {
            label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(254,240,240);");
        } else if (dayItem.score == 4) {
            label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(253,235,221);");
        } else if (dayItem.score == 3) {
            label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(222,251,241);");
        } else if (dayItem.score == 2) {
            label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(230,244,255);");
        } else {
            label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(238,237,255);");
        }
    } else {
        label->setText(QString("🎮 %1\n       %2-%3").arg(dayItem.title.mid(0, 10)).arg(dayItem.start_time).arg(dayItem.end_time));
        label->setStyleSheet("font: 8pt \"微软雅黑\";color: #000;background-color: rgb(251,234,204);");
    }
}
