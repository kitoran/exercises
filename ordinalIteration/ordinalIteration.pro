TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lXext
#DEFINES += SDL
#CONFIG += SDL

#include(../midieditor/library/gui-X/gui.pri)
#include(../../library/stb/stb_ds.pri)
#include(../../library/misc/misc.pri)
CONFIG += X
include(../../library/gui/gui.pri)
QMAKE_CFLAGS += -Wno-pointer-sign -Wno-unused-parameter -Wno-unused-function
#QMAKE_CFLAGS +=  -Ofast
QMAKE_CFLAGS +=  -O0
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
