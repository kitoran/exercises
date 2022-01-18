TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    audio.c \
    midi.cpp

LIBS += -lasound
INCLUDEPATH += /usr/share/gettext ./alsa-utils/include/
DEFINES += HAVE_CONFIG_H SYS_LOCKFILE=\"\\\"/var/lock/asound.state.lock\\\"\"
QMAKE_CFLAGS += -DHAVE_CONFIG_H -I. -I../include    -D_GNU_SOURCE -D__USE_GNU -DSYS_ASOUNDRC=\"/var/lib/alsa/asound.state\" -DSYS_LOCKFILE=\"/var/lock/asound.state.lock\" -DSYS_PIDFILE=\"/var/run/alsactl.pid\"
HEADERS += \
    audio.h \
    midi.h
QMAKE_CFLAGS += -std=c99
