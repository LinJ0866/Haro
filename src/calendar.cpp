#include "calendar.h"
#include "ui_calendar.h"

#define dayItem(x,y) (ui->day##x##_##y)

//1990年1月1号是星期一
int nyear(int year)   //判断是闰年还是平年
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return 366;
    else
        return 365;
}

int nmonth(int y, int m)        //根据年份和月份判断是哪一个月的天数
{
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
        return 31;
    else if (nyear(y) == 366 && m == 2)
        return 29;
    else if (nyear(y) == 365 && m == 2)
        return 28;
    else
        return 30;
}

int GetDays(int year, int month)  //输入年份和月份确定到1990.1.1总共多少天
{
    int i = 0;
    int sum = 0;
    if (year>1990)
    for (i = 1990; i < year; i++)
        sum += nyear(i);
    if (month>1)
    for (i = 1; i < month; i++)
    {
        sum += nmonth(year, i);
    }

    return sum;
}

Calendar::Calendar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Calendar)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowIcon(QIcon(HaroIcon::getIcon(HaroIcon::IconName::Cabinet))); //设置窗口图标

    connect(ui->pushButton, &QPushButton::clicked, this, &Calendar::goPrev);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Calendar::goAfter);
}

Calendar::~Calendar()
{
    delete ui;
}

void Calendar::getDb(Db *db) {
    this->db = db;
}

void Calendar::updateTime() {
    QDate dateTime= QDateTime::currentDateTime().date();
    year = dateTime.year();
    nowMonth = dateTime.month();
    month = nowMonth;
    nowDay = dateTime.day();
}

void Calendar::render() {
    // 重置
    for(int i=1; i<7; i++) {
        for (int j=1; j<8; j++) {
            getdayItem(i,j)->reset();
        }
    }
    ui->monthLabel->setText(QString::number(month)+"月");
    ui->yearLabel->setText(QString::number(year)+"年");


    int i, k=1;
    int sum = GetDays(year, month);
    int day = (sum + 1) % 7;  //得出一个月第一行前要空几格
    if (day == 0) day = 7;
    int daycount = nmonth(year, month);   //算出这一个月的天数

    int today = -1;

    if(month == nowMonth) today = nowDay;

    for (i = 1; i <= daycount; i++)
    {
        QList<QHash<QString,QString>> dayItem;
        // qDebug()<<i<<k<<day;

        db->getData("timelines", dayItem, QString("where is_delete == 0 and year == %1 and month == %2 and day == %3").arg(year).arg(month).arg(i));
        getdayItem(k,day)->addDayItem(dayItem);

        getdayItem(k,day++)->set(QString::number(i), true, day>6, i==today);
        if (day == 8) {
            k++;
            day = 1;
        }
    }

    ui->remind->setText(QString("本月共专注工作 <font color = #4896FA >%1</font>次，总时长达 <font color = #4896FA >%2</font>分钟，计算专注率可达 <font color = #4896FA >%3</font>%，再接再厉！").arg(0).arg(0).arg(0));

}

void Calendar::goPrev() {
    month--;
    if (month == 0) {
        month = 12;
        year --;
    }

    this->render();

}
void Calendar::goAfter() {
    month++;
    if (month == 13) {
        month = 1;
        year ++;
    }
    render();
}

Calendar_day* Calendar::getdayItem(int y, int x)
{
    if (x == 1 && y == 1) {
        return dayItem(1, 1);
    } else if (x == 1 && y == 2) {
        return dayItem(2, 1);
    } else if (x == 1 && y == 3) {
        return dayItem(3, 1);
    } else if (x == 1 && y == 4) {
        return dayItem(4, 1);
    } else if (x == 1 && y == 5) {
        return dayItem(5, 1);
    } else if (x == 1 && y == 6) {
        return dayItem(6, 1);
    }

    else if (x == 2 && y == 1) {
        return dayItem(1, 2);
    } else if (x == 2 && y == 2) {
        return dayItem(2, 2);
    } else if (x == 2 && y == 3) {
        return dayItem(3, 2);
    } else if (x == 2 && y == 4) {
        return dayItem(4, 2);
    } else if (x == 2 && y == 5) {
        return dayItem(5, 2);
    } else if (x == 2 && y == 6) {
        return dayItem(6, 2);
    }

    else if (x == 3 && y == 1) {
        return dayItem(1, 3);
    } else if (x == 3 && y == 2) {
        return dayItem(2, 3);
    } else if (x == 3 && y == 3) {
        return dayItem(3, 3);
    } else if (x == 3 && y == 4) {
        return dayItem(4, 3);
    } else if (x == 3 && y == 5) {
        return dayItem(5, 3);
    } else if (x == 3 && y == 6) {
        return dayItem(6, 3);
    }

    else if (x == 4 && y == 1) {
        return dayItem(1, 4);
    } else if (x == 4 && y == 2) {
        return dayItem(2, 4);
    } else if (x == 4 && y == 3) {
        return dayItem(3, 4);
    } else if (x == 4 && y == 4) {
        return dayItem(4, 4);
    } else if (x == 4 && y == 5) {
        return dayItem(5, 4);
    } else if (x == 4 && y == 6) {
        return dayItem(6, 4);
    }

    else if (x == 5 && y == 1) {
        return dayItem(1, 5);
    } else if (x == 5 && y == 2) {
        return dayItem(2, 5);
    } else if (x == 5 && y == 3) {
        return dayItem(3, 5);
    } else if (x == 5 && y == 4) {
        return dayItem(4, 5);
    } else if (x == 5 && y == 5) {
        return dayItem(5, 5);
    } else if (x == 5 && y == 6) {
        return dayItem(6, 5);
    }

    else if (x == 6 && y == 1) {
        return dayItem(1, 6);
    } else if (x == 6 && y == 2) {
        return dayItem(2, 6);
    } else if (x == 6 && y == 3) {
        return dayItem(3, 6);
    } else if (x == 6 && y == 4) {
        return dayItem(4, 6);
    } else if (x == 6 && y == 5) {
        return dayItem(5, 6);
    } else if (x == 6 && y == 6) {
        return dayItem(6, 6);
    }

    else if (x == 7 && y == 1) {
        return dayItem(1, 7);
    } else if (x == 7 && y == 2) {
        return dayItem(2, 7);
    } else if (x == 7 && y == 3) {
        return dayItem(3, 7);
    } else if (x == 7 && y == 4) {
        return dayItem(4, 7);
    } else if (x == 7 && y == 5) {
        return dayItem(5, 7);
    } else if (x == 7 && y == 6) {
        return dayItem(6, 7);
    }

    return nullptr;
}

