TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CFLAGS += -std=gnu11
INCLUDEPATH += include
LIBS += -lGL  -ldl -lSDL2
LIBS += -lglfw # -lX11 -lpthread -lXrandr -lXi -ldl
SOURCES += main.c \
#src/glad.c
