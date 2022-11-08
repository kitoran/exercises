TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

guiPers = true;
CONFIG += X
include(../../library/gui/gui.pri)
#include(../../library/guiS/gui.pri)
include(../../library/misc/misc.pri)
include(../../library/loadImage/loadImage.pri)
QMAKE_CFLAGS += -Wno-pointer-sign -fno-PIC
#QMAKE_CFLAGS +=  -Ofast
QMAKE_CFLAGS +=  -O0
QMAKE_CFLAGS += -Werror=implicit-function-declaration
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
