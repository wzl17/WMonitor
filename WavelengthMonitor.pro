QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts core network serialport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttons.cpp \
    chartview.cpp \
    globals.cpp \
    jsondata.cpp \
    lasercontrol.cpp \
    main.cpp \
    mainwindow.cpp \
    shutters.cpp \
    udpsocket.cpp

HEADERS += \
    buttons.h \
    chartview.h \
    globals.h \
    jsondata.h \
    lasercontrol.h \
    mainwindow.h \
    shutters.h \
    udpsocket.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/ -lNIDAQmx

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

RESOURCES +=
