#ifndef HARO_H
#define HARO_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QtGlobal>
#include <QLabel>
#include <QPoint>
#include <QTimer>
#include <QPushButton>
#include <QCalendarWidget>
#include <QFile>
#include <QDataStream>
#include <QSystemTrayIcon>

#include "resource.h"
#include "systemtray.h"
#include "dresswin.h"
#include "setwin.h"
#include "db.h"
#include "calendar.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Haro; }
QT_END_NAMESPACE

class Haro : public QMainWindow
{
    Q_OBJECT
private:
    Ui::Haro *ui;

    Db *db; //数据库
    systemtray *systray;
    DressWin *dressWindow;//换装窗口指针
    SetWin *setWindow;//设置窗口指针
    Calendar* calendar;

    QLabel *bodyImage,*earsImage,*eyesImage,*stripeImage;//各部件对应标签

    // int btnSize;//按钮大小
    int windowSize;//体型大小
    int bodyDressIdx, earsDressIdx;//身体和耳朵对应装扮序号
    int coordX,coordY;//桌面坐标
    QPoint moveLeftTop;//坐标点

    int earMoveIdx = 0;
    int eyeMoveIdx = 0;
    int eyeMoveKind = -1;
    int tempEyeMoveIdx;
    int tempEyeMoveKind;
    int eyeMoveMax = -1;
    int eyeErrorIdx = 0;
    int eyeTextIdx = 0;
    QString eyeTextContent;

    QHash<QString,QString> timelineItem;

    int strideThreshold = 170;
    int EAR_SWITCH_MAX_COUNT = 40;
    int draggingCount = 0; // 拖动抖动

    QTimer *timer, *timer1s;//定时器
    int secondCount = 0;
    QString eyeClockText = "--:--";

    const int btnNum = 7;
    QList<QPushButton*> btns;
    const QList<int> menuBtnIdxs = {4,2,1,3};
    int btnStatus=0; //按钮显示状态  0：主菜单隐藏 1：主菜单显示
    QList<QHash<QString, QString>> menu_btns;

    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件-虚函数
    void mousePressEvent(QMouseEvent *event); //鼠标点击事件-虚函数
    void mouseReleaseEvent(QMouseEvent *event); //鼠标释放时间-虚函数

    void earsMovement();
    void eyesMovement();
    void clockInterrupt();

    void bindTimerSlots();
    void bindSlots();

    void HaroUiUpdate(QLabel *image, QString imageUrl, bool isInit=false); //设置各部位图片。0,1,2,3分别为body,ear,eyes,stripe

    void hideMenuBtns();
    void hideMenuBtns(int status);

    void initBtn();//初始化按钮
    void renderBtn(); //渲染按钮
    void onBtnsClick();
    void closeBtnPush();//点击关闭按钮事件
    void dressBtnPush();//点击装扮按钮事件
    void minBtnPush();//点击最小化按钮事件
    void setBtnPush();//点击设置按钮事件
    void TimeBoardBtnPush();
    void timeLineStartPush();
    void timeLineEndPush();

    void timeLineCheckUnfinishedId();
    void timeLineCreate(int workType, QString workDesc);
    void timeLineUpdate();
    void timeLineEnd(int workScore);

    void loadConfigData(); // 加载config数据
    void updateConfigData(QString key, int value); // 存储config数据

private slots:
    void updateDressing(QString key, int value);
    void updateSize(int value);
    void exit();
    void restoreWindows();
    void restorePosition();
public:
    Haro(QWidget *parent = nullptr);
    ~Haro();  

    void sendText(QString text);
};

#endif // HARO_H
