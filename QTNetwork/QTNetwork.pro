#-------------------------------------------------
#
# Project created by QtCreator 2021-01-23T11:05:10
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT       += charts
QT       += websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTNetwork
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += E:/opencv/opencv/rebuild/install/include
INCLUDEPATH += E:/opencv/opencv/rebuild/install/include/opencv
INCLUDEPATH += E:/opencv/opencv/rebuild/install/include/opencv2

LIBS+=E:/opencv/opencv/rebuild/install/x86/mingw/lib/libopencv_face330.dll.a
LIBS+=E:/opencv/opencv/rebuild/install/x86/mingw/lib/libopencv_core330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_highgui330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_imgproc330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_video330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_objdetect330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_imgcodecs330.dll.a
LIBS+=E:/opencv/opencv/install/install/x86/mingw/lib/libopencv_videoio330.dll.a

SOURCES += \
        main.cpp \
        widget.cpp \
    server.cpp \
    mysocket.cpp \
    mythread.cpp \
    manager.cpp \
    entrance.cpp \
    face_train.cpp \
    myrunnable.cpp \
    myudpserver.cpp \
    ChartView.cpp \
    myplacex.cpp \
    webserver.cpp

HEADERS += \
        widget.h \
    server.h \
    mysocket.h \
    mythread.h \
    manager.h \
    entrance.h \
    face_train.h \
    myrunnable.h \
    myudpserver.h \
    ChartView.h \
    myplacex.h \
    webserver.h

FORMS += \
        widget.ui \
    manager.ui \
    entrance.ui \
    myplacex.ui
