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
    desktop.cpp \
    toppanel.cpp \
    client.cpp

HEADERS  += \
    wmapp.h \
    desktop.h \
    toppanel.h \
    client.h

OTHER_FILES += \
    desktop.qml \
    Tab.qml \
    Settings.qml \
    Panel.qml \
    Shortcut.qml \
    Xevents.txt \
    categories.txt

RESOURCES +=
