TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += X
guiPers = true;
include(../../library/gui/gui.pri)
include(../../library/misc/misc.pri)
include(../../library/stb/stb_ds.pri)
QMAKE_CFLAGS += -Wno-pointer-sign
QMAKE_CFLAGS +=  -Ofast
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
