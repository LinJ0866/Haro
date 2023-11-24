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

#include"dresswin.h"
#include"setwin.h"
#include"musicwin.h"
#include "db.h"

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

    DressWin *dressWindow;//换装窗口指针
    SetWin *setWindow;//设置窗口指针

    QPoint moveLeftTop;//坐标点

    vector<QPixmap> movement;//表情图片容器
    vector<QPixmap> spMovement;//特殊动作图片容器
    vector<QPixmap> flyMovement;//拖动动作图片容器

    vector<int>faceNum;//每个表情对应帧数与起始位置

    vector<QPixmap> body,ears1,ears2;//各部件对应图片容器

    QPixmap eyes,stripe;//眼睛和眼部遮罩

    QLabel *bodyImage,*earsImage,*eyesImage,*stripeImage;//各部件对应标签

    QTimer *timer;//定时器

    void loadConfigData(); // 加载config数据

    int size;//体型大小
    int btnSize;//按钮大小
    int bodyNum,earsNum;//身体和耳朵对应装扮序号
    int coordX,coordY;//桌面坐标

    QList<QPushButton*> btns;
    int btnStatus=0; //按钮显示状态  0：主菜单隐藏 1：主菜单显示
    QList<QHash<QString, QString>> menu_btns;

    int face;//表情序号
    int faceSum;//表情数量

    int spMove;//特殊动作序号

    int draggingCount = 0;
    int flyMove=0;

    QSystemTrayIcon* pSystemTray;//系统托盘

    void mouseMoveEvent(QMouseEvent *event); //鼠标移动事件-虚函数
    void mousePressEvent(QMouseEvent *event); //鼠标点击事件-虚函数
    void mouseReleaseEvent(QMouseEvent *event); //鼠标释放时间-虚函数

    void hideMenuBtns();
    void hideMenuBtns(int status);
    void refreshAppearence();
    void refreshAppearence(QString name);

    void onBtnsClick();
    QList<int> menuBtnIdxs = {4,2,1,3};
public:
    Haro(QWidget *parent = nullptr);
    ~Haro();  

    void eyesMovementLoad();//眼部动作载入
    void eyesMovement();//眼部动作表情

    inline void imageSet(QLabel *image,QPixmap map);//设置各部位图片

    void imageLoad();//各部位图片载入

    void initBtn();//初始化按钮
    void renderBtn(); //渲染按钮

    void initSystemTray();//初始化系统托盘
    void restoreWindows();

    void closeBtnPush();//点击关闭按钮事件
    void dressBtnPush();//点击装扮按钮事件
    void minBtnPush();//点击最小化按钮事件
    void setBtnPush();//点击设置按钮事件

    void specialMovementLoad();//特殊动作载入
    void flyMovementLoad();//拖动动作载入

    void specialMovement();//特殊动作事件

    void updateConfigData(QString key, int value); // 存储config数据
    void restorePosition();
public slots:
    void systemTrayPush(QSystemTrayIcon::ActivationReason reason);//点击系统托盘事件

    void updateDressing(QString key, int value);
    void updateSize(int value);
};

#endif // HARO_H
