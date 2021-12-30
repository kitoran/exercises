#-------------------------------------------------
#
# Project created by QtCreator 2021-02-23T16:54:40
#
#-------------------------------------------------

QT       -= core
QMAKE_CXXFLAGS += -std=gnu++2a
QMAKE_CFLAGS += -std=gnu11 -Wstrict-overflow=5
#CONFIG+=c++17
CONFIG+=link_pkgconfig
PKGCONFIG += gtk+-3.0
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = additive
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lGL -lGLU -lasound

QMAKE_CXXFLAGS += -Wno-unused-variable
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wno-sign-compare
INCLUDEPATH += ../sound ../../library/ambiguousArray ../../library/dynamicArray \
               ../../library/newFile
LIBS += -lsndfile -lssl -lcrypto -lexplain
SOURCES += \
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
    stb_ds.c \
    mainwindow.c \
    complex_fft.c \
    ../../library/newFile/newfile.c \
    intpool.c

HEADERS += \
        mainwindow.h \
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
    ui_mainwindow.h \
    intpool.h

FORMS += \
        mainwindow.ui

OTHER_FILES += \
    fftrec.c \
    fft.c

RESOURCES +=

DISTFILES += \
    mainwindowg.glade
