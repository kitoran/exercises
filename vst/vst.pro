#-------------------------------------------------
#
# Project created by QtCreator 2021-01-14T22:22:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = vst
TEMPLATE = lib

DEFINES += VST_LIBRARY DEVELOPMENT


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += VST_SDK/VST3_SDK/ VST_SDK/VST3_SDK/pluginterfaces/vst/
SOURCES += \
    controller.cpp \
    simple.cpp

HEADERS += \
    simple.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
