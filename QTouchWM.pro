#-------------------------------------------------
#
# Project created by QtCreator 2012-07-05T15:00:04
#
#-------------------------------------------------

QT       += core gui declarative

TARGET = QTouchWM
TEMPLATE = app

LIBS += -lX11


SOURCES += main.cpp\
    wmapp.cpp \
    qwindow.cpp \
    desktop.cpp \
    toppanel.cpp

HEADERS  += \
    wmapp.h \
    qwindow.h \
    desktop.h \
    toppanel.h

OTHER_FILES += \
    desktop.qml \
    Tab.qml \
    Settings.qml \
    Panel.qml

RESOURCES +=
