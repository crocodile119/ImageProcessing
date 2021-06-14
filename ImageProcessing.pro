#-------------------------------------------------
#
# Project created by QtCreator 2018-06-29T18:47:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessing
TEMPLATE = app

RC_ICONS = ImageProcessing.ico

CONFIG += c++11
CONFIG += ImageProcessing.exe.manifest
QT += help

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
    display.cpp \
    commands.cpp \
    imagepan.cpp \
    histogram.cpp \
    equalizer.cpp \
    ImageProcessing.cpp \
    imagefiltering.cpp \
    controldock.cpp \
    previewcommands.cpp \
    showhistogram.cpp \
    scene.cpp \
    roidialog.cpp \
    mygraphicsrectitem.cpp \
    informationpanel.cpp \
    helpbrowser.cpp

HEADERS  += \
    mainwindow.h \
    display.h \
    commands.h \
    imagepan.h \
    histogram.h \
    equalizer.h \
    ImageProcessing.h \
    imagefiltering.h \
    controldock.h \
    previewcommands.h \
    showhistogram.h \
    scene.h \
    roidialog.h \
    mygraphicsrectitem.h \
    informationpanel.h \
    helpbrowser.h

LIBS += -LC:\Users\giord\opencv-build\install\x64\vc16\lib \
#-lopencv_core3414 -lopencv_imgproc3414 -lopencv_highgui3414 -lopencv_imgcodecs3414 -lopencv_videoio3414 -lopencv_video3414 -lopencv_calib3d3414 -lopencv_photo3414 -lopencv_features2d3414
-lopencv_core3414d -lopencv_imgproc3414d -lopencv_highgui3414d -lopencv_imgcodecs3414d -lopencv_videoio3414d -lopencv_video3414d -lopencv_calib3d3414d -lopencv_photo3414d -lopencv_features2d3414d

INCLUDEPATH += C:\Users\giord\opencv-build\install\include
DEPENDPATH += C:\Users\giord\opencv-build\install\include

FORMS += \
    showhistogram.ui \
    imagepan.ui \
    form.ui \
    roidialog.ui

RESOURCES += \
    imageprocessing.qrc
