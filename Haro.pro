QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += multimedia
QT += sql

INCLUDEPATH += src/include

SOURCES += main.cpp \
    src/calendar.cpp \
    src/calendar_day.cpp \
    src/resource.cpp \
    src/systemtray.cpp \
    src/db.cpp \
    src/dresswin.cpp \
    src/haro.cpp \
    src/setwin.cpp

HEADERS += src/include/db.h \
           src/include/calendar.h \
           src/include/calendar_day.h \
           src/include/dresswin.h \
           src/include/haro.h \
           src/include/resource.h \
           src/include/setwin.h \
           src/include/systemtray.h

FORMS += src/include/dresswin.ui \
         src/include/calendar.ui \
         src/include/calendar_day.ui \
         src/include/haro.ui \
         src/include/setwin.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

