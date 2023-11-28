#include <QDesktopWidget>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QtCore>

#include "widget.h"
#include "ui_widget.h"
//#include "floatlabelshower.h"
//#include "themechange.h"

#define DayViewer(x, y) (ui->frameDayContent_##y##x)

Widget::Widget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // FloatLabelShower::instance().setContent(this);

    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::Tool);
    setWindowFlag(Qt::WindowStaysOnBottomHint);
    setAttribute(Qt::WA_TranslucentBackground);

    updateDateTimeTitle();

    m_dtTimerID = startTimer(1000);
    m_synTimerID = startTimer(30000); //30秒同步一次

    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(readOutput()));

    syncData();

    init();
    setWindowIcon(QIcon(":/icons/10 11@2x.png"));
    ui->labelTitleVersion->setText("桌面日历待办列表");
}

Widget::~Widget()
{
    syncData();
    delete ui;
}

void Widget::updateDateTimeTitle()
{
    const QDateTime dt = QDateTime::currentDateTime();

    QString week;
    switch (dt.date().dayOfWeek()) {
    case Qt::Monday:
        week = QStringLiteral("星期一");
        break;
    case Qt::Tuesday:
        week = QStringLiteral("星期二");
        break;
    case Qt::Wednesday:
        week = QStringLiteral("星期三");
        break;
    case Qt::Thursday:
        week = QStringLiteral("星期四");
        break;
    case Qt::Friday:
        week = QStringLiteral("星期五");
        break;
    case Qt::Saturday:
        week = QStringLiteral("星期六");
        break;
    case Qt::Sunday:
        week = QStringLiteral("星期天");
        break;
    }
    ui->labelDateTime->setText(dt.toString("yyyy年MM月dd日 ") + week);
}

void Widget::showEvent(QShowEvent* event)
{
    //获取可用桌面大小
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();

    const QSize size(deskRect.width() * 3 / 4, deskRect.height() - 20);

    setGeometry(deskRect.width() - size.width() - 10, (deskRect.height() - size.height()) / 2, size.width(), size.height());
}

void Widget::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_dtTimerID) {
        updateDateTimeTitle();
    } else if (event->timerId() == m_synTimerID) {
        syncData();
    }
}

void Widget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Widget::init()
{
    const QDate currentDt = QDate::currentDate();
    const int week = currentDt.dayOfWeek();

    //设置中间行
    for (int i = 1; i <= 7; i++) {
        DayViewer* dv = getDayViewer(i, 3);
        if (dv) {
            const QDate dt = currentDt.addDays(i - week);
            dv->setDate(dt);
        }
    }

    //1 ~ 2排
    for (int i = 1; i <= 7; i++) {
        for (int n = 1; n <= 2; n++) {
            DayViewer* dv = getDayViewer(i, n);
            if (dv) {
                const QDate dt = currentDt.addDays(i - week - (3 - n) * 7);
                dv->setDate(dt);
            }
        }
    }

    //4 ~ 5排
    for (int i = 1; i <= 7; i++) {
        for (int n = 4; n <= 5; n++) {
            DayViewer* dv = getDayViewer(i, n);
            if (dv) {
                const QDate dt = currentDt.addDays(i - week + (n - 3) * 7);
                dv->setDate(dt);
            }
        }
    }
}

DayViewer* Widget::getDayViewer(int x, int y)
{
    if (x == 1 && y == 1) {
        return DayViewer(1, 1);
    } else if (x == 1 && y == 2) {
        return DayViewer(1, 2);
    } else if (x == 1 && y == 3) {
        return DayViewer(1, 3);
    } else if (x == 1 && y == 4) {
        return DayViewer(1, 4);
    } else if (x == 1 && y == 5) {
        return DayViewer(1, 5);
    }

    else if (x == 2 && y == 1) {
        return DayViewer(2, 1);
    } else if (x == 2 && y == 2) {
        return DayViewer(2, 2);
    } else if (x == 2 && y == 3) {
        return DayViewer(2, 3);
    } else if (x == 2 && y == 4) {
        return DayViewer(2, 4);
    } else if (x == 2 && y == 5) {
        return DayViewer(2, 5);
    }

    else if (x == 3 && y == 1) {
        return DayViewer(3, 1);
    } else if (x == 3 && y == 2) {
        return DayViewer(3, 2);
    } else if (x == 3 && y == 3) {
        return DayViewer(3, 3);
    } else if (x == 3 && y == 4) {
        return DayViewer(3, 4);
    } else if (x == 3 && y == 5) {
        return DayViewer(3, 5);
    }

    else if (x == 4 && y == 1) {
        return DayViewer(4, 1);
    } else if (x == 4 && y == 2) {
        return DayViewer(4, 2);
    } else if (x == 4 && y == 3) {
        return DayViewer(4, 3);
    } else if (x == 4 && y == 4) {
        return DayViewer(4, 4);
    } else if (x == 4 && y == 5) {
        return DayViewer(4, 5);
    }

    else if (x == 5 && y == 1) {
        return DayViewer(5, 1);
    } else if (x == 5 && y == 2) {
        return DayViewer(5, 2);
    } else if (x == 5 && y == 3) {
        return DayViewer(5, 3);
    } else if (x == 5 && y == 4) {
        return DayViewer(5, 4);
    } else if (x == 5 && y == 5) {
        return DayViewer(5, 5);
    }

    else if (x == 6 && y == 1) {
        return DayViewer(6, 1);
    } else if (x == 6 && y == 2) {
        return DayViewer(6, 2);
    } else if (x == 6 && y == 3) {
        return DayViewer(6, 3);
    } else if (x == 6 && y == 4) {
        return DayViewer(6, 4);
    } else if (x == 6 && y == 5) {
        return DayViewer(6, 5);
    }

    else if (x == 7 && y == 1) {
        return DayViewer(7, 1);
    } else if (x == 7 && y == 2) {
        return DayViewer(7, 2);
    } else if (x == 7 && y == 3) {
        return DayViewer(7, 3);
    } else if (x == 7 && y == 4) {
        return DayViewer(7, 4);
    } else if (x == 7 && y == 5) {
        return DayViewer(7, 5);
    }

    return nullptr;
}

void Widget::on_pushButtonClose_clicked(bool checked)
{
    if (QMessageBox::Yes == QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确认退出程序吗?"), QMessageBox::Yes | QMessageBox::No)) {
        qApp->quit();
    }
}

//void Widget::on_pushButtonRefresh_clicked(bool checked)
//{
//    //    if (QMessageBox::Yes == QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("是否同步数据?"), QMessageBox::Yes | QMessageBox::No)) {
//    syncData();
//    FloatLabelShower::instance().showLabel(QStringLiteral("同步数据..."));
//    //    }
//}

//void Widget::on_pushButtonReloadStyles_clicked(bool checked)
//{
//    ThemeInstance()->loadStyle();
//    FloatLabelShower::instance().showLabel(QStringLiteral("更新样式..."));
//}

void Widget::readOutput()
{
    QByteArray qba = process->readAllStandardOutput();
    QTextCodec* pTextCodec = QTextCodec::codecForName("System");
    QString str = pTextCodec->toUnicode(qba);
    qDebug() << str;
}

void Widget::syncData()
{
    if (process) {
        //绑定反馈值
        QString cmd = QStringLiteral("%1/scripts/update.bat")
                          .arg(QCoreApplication::applicationDirPath())
                          .toUtf8()
                          .data();

        qDebug() << "CMD:" << cmd;
        //执行命令
        process->start(cmd);
    }
}
