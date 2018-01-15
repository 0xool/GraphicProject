#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T13:23:29
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LisansProjectQT
TEMPLATE = app
QMAKE_MAC_SDK = macosx10.13
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    mesh.cpp \
    openglview.cpp \
    openglwidget.cpp \
    outershape.cpp \
    innershape.cpp \
    projectmanager.cpp \
    outershapeuserinput.cpp \
    outershapeinputopengl.cpp \
    node.cpp \
    anealalgorithm.cpp

HEADERS  += mainwindow.h \
    mesh.h \
    openglview.h \
    openglwidget.h \
    outershape.h \
    innershape.h \
    innershape.h \
    projectmanager.h \
    outershapeuserinput.h \
    outershapeinputopengl.h \
    node.h \
    anealalgorithm.h

FORMS    += mainwindow.ui \
    openglview.ui \
    outershapeuserinput.ui
