#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T12:33:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = example
TEMPLATE = app


SOURCES += main.cpp\
        Player.cpp \
    QlibVLC.cpp

HEADERS  += Player.h \
    QlibVLC.h

FORMS    += Player.ui

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../../usr/lib/ -lvlc

INCLUDEPATH += $$PWD/../../../../../../usr/include
DEPENDPATH += $$PWD/../../../../../../usr/include

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../../usr/lib/libvlc.a

win32: LIBS += -L$$PWD/C:/Qt/Qt5.2.0/5.2.0/mingw48_32/lib/ -llibvlc

INCLUDEPATH += $$PWD/C:/Qt/Qt5.2.0/5.2.0/mingw48_32/include
DEPENDPATH += $$PWD/C:/Qt/Qt5.2.0/5.2.0/mingw48_32/include
