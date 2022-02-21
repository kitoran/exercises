TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

guiPers = true;
include(../../library/guiS/gui.pri)
QMAKE_CFLAGS += -Ofast -Wno-pointer-sign
DEFINES += MY_PATH=$$PWD

SOURCES += main.c
