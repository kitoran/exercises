TEMPLATE = lib
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += clap/include
TARGET = retune

DESTDIR =  C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug
TARGET_EXT = .clap
#install.path = C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug

#install.file = retune.clap

#install.name = retune.clap
#install.extra = cp C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug\retune.clap.dll C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug\retune.clap
#INSTALLS += install
include(../../library/extmath/extmath.pri)
include(../../library/misc/misc.pri)
include(../../library/stb/stb_ds.pri)
SOURCES += \
    fftRetuner.cpp \
    mainRetuner.c
#        ../additive/fft.c \
#        ../additive/fftrec.c \
#        ../additive/stft.c \
QMAKE_CFLAGS += /we4013 /we4024 /we4020 /we4047 /we4133 /O2 /fp:fast
QMAKE_CXXFLAGS += /O2 /fp:fast
HEADERS += \
    fftRetuner.h
#    ../additive/fft.h \
#    ../additive/fftrec.h \
 #    ../additive/stft.h
windows {
QMAKE_CFLAGS += /Zc:preprocessor  /std:c11 /we4013 /wd4838


}
