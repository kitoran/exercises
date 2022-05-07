QT -= gui core

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += ASMJIT_STATIC

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    parse.cpp \
    translate.cpp \
    assembler.cpp \
    supercombine.cpp

HEADERS += \
    parse.h \
    translate.h \
    assembler.h

DISTFILES += \

LIBS += -lrt -lexplain
QMAKE_CXXFLAGS += -std=gnu++17
