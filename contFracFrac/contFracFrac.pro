TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

guiPers = true;
include(../../library/gui-X/gui.pri)
include(../../library/misc/misc.pri)
QMAKE_CFLAGS += -Wno-pointer-sign
QMAKE_CFLAGS +=  -Ofast
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
