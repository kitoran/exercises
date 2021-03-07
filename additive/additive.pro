#-------------------------------------------------
#
# Project created by QtCreator 2021-02-23T16:54:40
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = additive
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lGL -lGLU

INCLUDEPATH += qt-curve-editor/src/cetest
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wno-sign-compare

LIBS += -lsndfile
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qt-curve-editor/src/cetest/cetest.cpp \
    qt-curve-editor/src/curveeditor/GlobalData.cpp \
    qt-curve-editor/src/curveeditor/graphiceditor.cpp \
    qt-curve-editor/src/curveeditor/SplineDisplayerWidget.cpp \
    qt-curve-editor/src/libspline/aaCurve.cpp \
    qt-curve-editor/src/libspline/spline.cpp \
    graph.cpp \
    stft.cpp

HEADERS += \
        mainwindow.h \
    qt-curve-editor/src/cetest/cetest.h \
    qt-curve-editor/src/curveeditor/GlobalData.h \
    qt-curve-editor/src/curveeditor/graphiceditor.h \
    qt-curve-editor/src/curveeditor/SplineDisplayerWidget.h \
    qt-curve-editor/src/libspline/aaCurve.h \
    qt-curve-editor/src/libspline/spline.h \
    graph.h \
    stft.h

FORMS += \
        mainwindow.ui \
    qt-curve-editor/src/cetest/cetest.ui \
    qt-curve-editor/src/curveeditor/graphiceditor.ui

RESOURCES += \
    qt-curve-editor/src/cetest/cetest.qrc
