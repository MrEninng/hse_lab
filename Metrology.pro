#-------------------------------------------------
#
# Project created by QtCreator 2015-10-22T00:49:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Metrology
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    intervalstable.cpp

HEADERS  += mainwindow.h \
    intervalstable.h

FORMS    += mainwindow.ui \
    intervalstable.ui
CONFIG   += c++11
