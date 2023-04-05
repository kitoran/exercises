TEMPLATE = lib
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += clap/include
TARGET = tweak
TARGET_EXT = .clap
install.path = C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug

install.file = tweak.clap

install.name = tweak.clap
install.extra = cp C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug\tweak.clap.dll C:\src\exercises\build-qmake-tweakpartials-Desktop_Qt_6_4_1_MSVC2019_64bit-Debug\debug\tweak.clap
INSTALLS += install
include(../../library/extmath/extmath.pri)
include(../../library/stb/stb_ds.pri)
SOURCES += \
    fft.cpp \
    main.c
#        ../additive/fft.c \
#        ../additive/fftrec.c \
#        ../additive/stft.c \
QMAKE_CFLAGS += /we4013 /we4024 /we4020 /we4047 /we4133 /O3 /fp:fast
QMAKE_CXXFLAGS += /fp:strict /O3 /fp:fast
HEADERS += \
#    ../additive/fft.h \
#    ../additive/fftrec.h \
 \#    ../additive/stft.h
    fft.h
