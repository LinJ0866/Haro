#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QString>

#include <resource.h>

class systemtray : public QSystemTrayIcon
{
    Q_OBJECT

public:
    systemtray(QApplication *parent = nullptr);

//public slots:
signals:
    void exitSignal();
    void restorePositionSignal();
    void displaySignal();

private:
    QApplication* parent;
    QMenu *menu;

private slots:
    void exit();
    void restorePosition();
    void display();
    void systemTrayPush(QSystemTrayIcon::ActivationReason reason);

public slots:
    void sendNotification(QString title, QString content);
};

#endif // SYSTEMTRAY_H
