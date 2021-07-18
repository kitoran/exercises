TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ../sound/audio.cpp \


INCLUDEPATH += ../sound

LIBS += -lsndfile -lrt -lasound -lexplain
