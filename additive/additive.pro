#-------------------------------------------------
#
# Project created by QtCreator 2021-02-23T16:54:40
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++17
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = additive
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lGL -lGLU -lasound

QMAKE_CXXFLAGS += -Wno-unuse-variable
INCLUDEPATH += qt-curve-editor/src/cetest
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wno-sign-compare
INCLUDEPATH += ../sound
LIBS += -lsndfile
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    graph.cpp \
    stft.cpp \
    ../sound/audio.cpp \
    alsathread.cpp \
    synthesis.cpp \
    mathext.cpp

HEADERS += \
        mainwindow.h \
    graph.h \
    stft.h \
    ../sound/audio.h \
    alsathread.h \
    synthesis.h \
    mathext.h

FORMS += \
        mainwindow.ui

RESOURCES +=
