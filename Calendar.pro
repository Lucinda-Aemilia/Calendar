#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T22:41:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calendar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    eventdbcontract.cpp \
    sqleventmodel.cpp \
    event.cpp \
    eventcalendar.cpp \
    cacheeventmodel.cpp \
    createneweventdialog.cpp \
    calendareventfilewidget.cpp \
    vieweventdialog.cpp \
    file.cpp \
    dragfilecombobox.cpp \
    calendartableeventbutton.cpp

HEADERS  += mainwindow.h \
    eventdbcontract.h \
    sqleventmodel.h \
    event.h \
    eventcalendar.h \
    cacheeventmodel.h \
    createneweventdialog.h \
    calendareventfilewidget.h \
    vieweventdialog.h \
    file.h \
    dragfilecombobox.h \
    calendartableeventbutton.h

FORMS    += mainwindow.ui \
    createneweventdialog.ui \
    calendareventfilewidget.ui \
    vieweventdialog.ui \
    dragfilecombobox.ui \
    calendartableeventbutton.ui

TRANSLATIONS += cn.ts

DISTFILES += \
    TransparentStyle.qml

RESOURCES += \
    resources.qrc
