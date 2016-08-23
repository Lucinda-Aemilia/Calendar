#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T22:41:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calendar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calendarbuttondelegate.cpp

HEADERS  += mainwindow.h \
    calendarbuttondelegate.h

FORMS    += mainwindow.ui
