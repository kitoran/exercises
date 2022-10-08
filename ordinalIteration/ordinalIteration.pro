TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lXext

include(../../library/gui-X/gui.pri)
#include(../../library/misc/misc.pri)
QMAKE_CFLAGS += -Wno-pointer-sign -Wno-unused-parameter -Wno-unused-function
#QMAKE_CFLAGS +=  -Ofast
QMAKE_CFLAGS +=  -O0
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
