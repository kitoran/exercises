#-------------------------------------------------
#
# Project created by QtCreator 2021-02-23T16:54:40
#
#-------------------------------------------------

QT       -= core
QMAKE_CXXFLAGS += -std=gnu++2a
QMAKE_CFLAGS += -std=gnu11 -Wstrict-overflow=5
#CONFIG+=c++17
#CONFIG+=link_pkgconfig
#PKGCONFIG += gtk+-3.0 gdk-3.0
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = additive
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lGL -lGLU -lasound

#unix:!macx: LIBS += -L$$PWD/../../library/build-guiS-Desktop-Debug/gui/ -lgui

#INCLUDEPATH += $$PWD/../gui
#DEPENDPATH += $$PWD/../gui

#unix:!macx: PRE_TARGETDEPS += $$PWD/../../library/build-guiS-Desktop-Debug/gui/libgui.a


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CFLAGS += -Werror=implicit-function-declaration -Wno-unused-variable -Wno-parentheses
INCLUDEPATH += ../sound ../../library/ambiguousArray ../../library/dynamicArray \
               ../../library/newFile ../../library/guiS/gui
LIBS += -lsndfile -lssl -lcrypto -lexplain -lX11 -lXrender
SOURCES *= \
    ../sound/audio.c \
    alsathread.c \
    globals.c \
    graph.c \
    main.c \
    mathext.c \
    soundext.c \
    spectrogram.c \
    stft.c \
    storearray.c \
    synthesis.c \
    complex_fft.c \
    ../../library/newFile/newfile.c \
    intpool.c \
    actions.c \
    ui.c

HEADERS += \
    graph.h \
    stft.h \
    ../sound/audio.h \
    alsathread.h \
    synthesis.h \
    mathext.h \
    storearray.h \
    globals.h \
    spectrogram.h \
    soundext.h \
    stb_ds.h \
    intpool.h \
    fft.h \
    fftrec.h \
    actions.h \
    ui.h

SOURCES += \
    fftrec.c \
    fft.c

#INCLUDEPATH += $$PWD/../../library/settingsS/settings
#SOURCES += \
#    $$PWD/../../library/guiS/gui/gui.c \
#    $$PWD/../../library/guiS/gui/gridlayout.c \
#    $$PWD/../../library/guiS/gui/persistent.c

#include(../../library/loadImage/loadImage.pri)
include(../../library/guiS/gui.pri)
include(../../library/settingsS/settings/settings.pri)
#include(../../library/stb/stb_image.pri)

DEFINES += MY_PATH=$$PWD

#HEADERS += \
#    $$PWD/../../library/guiS/gui/gui.h \
#    $$PWD/../../library/guiS/gui/persistent.h \
#    $$PWD/../../library/guiS/gui/gridlayout.h
#include(../../library/guiS/gui/gui.pro)
#PRE_TARGETDEPS += ../../library/build-guiS-Desktop-Debug/gui/libgui.a
#QMAKE_EXTRA_TARGETS += versionTarget
#versionTarget.target = ../../library/build-guiS-Desktop-Debug/gui/libgui.a
#versionTarget.depends = FORCE
##win32: versionTarget.commands = cd $$PWD; python.exe ./version_getter.py -p $$TARGET
##versionTarget.commands = cd $$PWD/../../library/guiS; qmake guiS.pro; cd ../build-guiS-Desktop-Debug/; make build
#versionTarget.commands = cd ../build-guiS-Desktop-Debug/; make build

#INCLUDEPATH +=     ../../stb
#SOURCES += \
#    $$PWD/../../library/settingsS/settings/settings.c\
#   ../../stb/stb_ds.c

#HEADERS += \
#    $$PWD/../../library/settingsS/settings/settings.h\
#    $$PWD/../../stb/stb_ds.h
