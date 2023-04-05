TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(../../library/stb/stb_ds.pri)
include(../../library/misc/misc.pri)
SOURCES += \
        main.c
