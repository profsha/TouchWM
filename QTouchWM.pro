#-------------------------------------------------
#
# Project created by QtCreator 2012-07-05T15:00:04
#
#-------------------------------------------------

QT       += core gui

TARGET = QTouchWM
TEMPLATE = app

LIBS += -lX11


SOURCES += main.cpp\
    wmapp.cpp \
    qwindow.cpp \
    qrunner.cpp \
    panel.cpp

HEADERS  += \
    wmapp.h \
    qwindow.h \
    qrunner.h \
    panel.h
