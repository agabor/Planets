#-------------------------------------------------
#
# Project created by QtCreator 2015-12-10T21:21:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = planets
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp \
    dustfield.cpp \
    gravityfield.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    dustfield.h \
    gravityfield.h

FORMS    += mainwindow.ui
