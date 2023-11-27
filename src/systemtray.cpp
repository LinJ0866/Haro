#include "systemtray.h"

systemtray::systemtray(QApplication *parent)
{
    this->parent = parent;

    this->setIcon(QIcon(HaroIcon::getIcon(HaroIcon::Icon)));
    this->setToolTip("你好，我是学术妲己Haro~");

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayPush(QSystemTrayIcon::ActivationReason)));

    // 托盘菜单
    QAction *menuDisplay = new QAction("显示Haro", this);
    QAction *menuRestore = new QAction("重置位置（找不到Haro时点击这个）", this);
    QAction *menuExit = new QAction("退出", this);
    connect(menuDisplay, &QAction::triggered, this, &systemtray::display);
    connect(menuRestore, &QAction::triggered, this, &systemtray::restorePosition);
    connect(menuExit, &QAction::triggered, this, &systemtray::exit);

    menu = new QMenu();
    menu->addAction(menuDisplay);
    menu->addAction(menuRestore);
    menu->addAction(menuExit);
    this->setContextMenu(menu);

    this->show();
}


void systemtray::exit() {
    emit exitSignal();
}

void systemtray::restorePosition() {
    emit restorePositionSignal();
}
void systemtray::display() {
    emit displaySignal();
}

void systemtray::systemTrayPush(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        this->display();
    }
}

void systemtray::sendNotification(QString title, QString content)
{
    this->showMessage(title, content, QSystemTrayIcon::NoIcon, 1000);
}
