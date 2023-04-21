QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MsgBusCheckThread.cpp \
    StreamMgr.cpp \
    main.cpp \
    GstreamerPlayer.cpp

HEADERS += \
    Common.h \
    GstreamerPlayer.h \
    MsgBusCheckThread.h \
    StreamMgr.h

FORMS += \
    GstreamerPlayer.ui

INCLUDEPATH += \
    $$PWD/../include \
    $$PWD/../include/gstreamer-1.0 \
    $$PWD/../include/glib-2.0/ \
    $$PWD/../include/glib-2.0/include \
    $$PWD/../lib/glib-2.0/include \
    $$PWD/../include/orc-0.4 \


win32: LIBS += -L$$PWD/../lib/ -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lintl -lgstvideo-1.0 -lgstbase-1.0

DESTDIR += \
    $$PWD/../bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
