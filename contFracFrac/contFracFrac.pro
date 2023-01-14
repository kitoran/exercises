TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += X
CONFIG += SDL
#guiPers = true;
include(../../library/gui/gui.pri)
include(../../library/misc/misc.pri)
include(../../library/stb/stb_ds.pri)
linux {
QMAKE_CFLAGS += -Wno-pointer-sign
}

windows {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    INCLUDEPATH += C:\src\SDL2-devel-2.26.1-VC\SDL2-2.26.1\include C:\src\SDL2-devel-2.26.1-VC\SDL2_ttf-2.20.1\include
    CONFIG -= console
    LIBS += /lKernel32
#    -lSDL2main -lShell32
#QMAKE_LFLAGS += user32.lib
    SDL2.dll.path = C:\src\SDL2-devel-2.26.1-VC\SDL2-2.26.1\
    INSTALLS += SDL2.dll
#    QMAKE_CXXFLAGS += /D__cplusplus #wtf why was this not defined in the first place
}
QMAKE_CFLAGS +=  -Ofast
DEFINES += MY_PATH=\\\"$$PWD\\\"

SOURCES += main.c
