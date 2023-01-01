TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp

CONFIG += c++20
QMAKE_CXXFLAGS += -std=gnu++20
