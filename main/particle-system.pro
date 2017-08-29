#-------------------------------------------------
#
# Project created by QtCreator 2017-07-26T19:10:00
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

LIBS += -lglut -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = particle-system
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    particlewidget.cpp \
    util.cpp \
    camera.cpp \
    compositionwidget.cpp \
    solid.cpp \
    cameradragger.cpp \
    CurveEditor/curveeditor/GlobalData.cpp \
    CurveEditor/curveeditor/graphiceditor.cpp \
    CurveEditor/curveeditor/SplineDisplayerWidget.cpp \
    CurveEditor/libspline/aaCurve.cpp \
    CurveEditor/libspline/spline.cpp \
    editor.cpp \
    turbulence.cpp \
    colorwidget.cpp \
    gradientdescriber.cpp

HEADERS += \
    mainwindow.h \
    particlewidget.h \
    particle.h \
    particlesystemfeatures.h \
    util.h \
    camera.h \
    compositionwidget.h \
    solid.h \
    cameradragger.h \
    CurveEditor/curveeditor/GlobalData.h \
    CurveEditor/curveeditor/graphiceditor.h \
    CurveEditor/curveeditor/SplineDisplayerWidget.h \
    CurveEditor/libspline/aaCurve.h \
    CurveEditor/libspline/spline.h \
    editor.h \
    turbulence.h \
    colorwidget.h \
    gradientdescriber.h

FORMS += \
        mainwindow.ui \
    CurveEditor/curveeditor/graphiceditor.ui \
    editor.ui

DISTFILES += \
    ../shaders/ParticleFragment.glsl \
    ../shaders/ParticleVertex.glsl \
    ../shaders/quadFrag.glsl \
    ../shaders/quadVertex.glsl
