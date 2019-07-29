#-------------------------------------------------
#
# Project created by QtCreator 2019-07-29T22:49:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client_Windows2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    portocol.cpp \
    variables.cpp

HEADERS  += mainwindow.h \
    client.h \
    portocol.h \
    variables.h

FORMS    += mainwindow.ui

RESOURCES += \
    graphics.qrc
